#include "BingWallpaperProvider.h"
#include "cocos2d.h"
#include "Extension/CCHttpRequest.h"
#include <sstream>
#include "json/jansson.h"
#include "Core/file_exists.h"
using namespace cocos2d;
BingWallpaperProvider::BingWallpaperProvider()
{
}

class BingMetaObtainer : public CCObject
{
public:
    BingMetaObtainer(const std::string& url, WallpaperDelegatePtr delegate,
                     unsigned int max_wallpapers)
        : _url(url), _delegate(WallpaperDelegatePtr(delegate)),
          _max_wallpapers(max_wallpapers)
    {
        this->retain();
    }
    void obtain()
    {
        CCHttpRequest *requestor = CCHttpRequest::sharedHttpRequest();
        requestor->addGetTask(_url, this,
                              callfuncND_selector(BingMetaObtainer::onHttpRequestCompleted));
    }

private:

    void parse(const std::string& json_data)
    {
        json_t* root = json_loads(json_data.c_str(), 0, 0);
        std::vector<std::string> urls;

        std::string base_name = CCFileUtils::sharedFileUtils()->getWriteablePath();

            if(!root)
            {
                parseError();
                return;
            }

            json_t* images = json_object_get(root, "images");
            if(!json_is_array(images))
            {
                parseError();
                json_decref(root);
                return;
            }

            unsigned int array_size = json_array_size(images);

            urls.reserve(array_size);
            for(unsigned int i = 0; i < array_size; i++)
            {
                json_t* it = json_array_get(images, i);
                if(!json_is_object(it))
                {
                    parseError();
                    json_decref(root);
                    return;
                }
                json_t* url = json_object_get(it, "url");
                if(!json_is_string(url))
                {
                    parseError();
                    json_decref(root);
                    return;
                }

                std::string url_part = json_string_value(url);
                std::string full_url = "http://www.bing.com"+url_part;

                std::string file_name = base_name + getFileNameFromUrl(url_part);
                if(!fileExists(file_name.c_str()))
                    urls.push_back(full_url);
                if(urls.size() >= _max_wallpapers)
                    break;
            }





        json_decref(root);

        if(urls.size() == 0)
            _delegate->onWallpapersRecieved(Wallpapers());
        else
        {
            CCHttpRequest *requestor = CCHttpRequest::sharedHttpRequest();
            requestor->addDownloadTask(
                        urls, this,
                        callfuncND_selector(BingMetaObtainer::onFilesArrived));
        }


    }
    void parseError()
    {
        _delegate->onWallpapersError(WallpaperDelegate::ParseError, "");
    }

    void onFilesArrived(cocos2d::CCObject *, void *data)
    {
        HttpResponsePacket *response = (HttpResponsePacket *)data;

        if (response->request->reqType == kHttpRequestDownloadFile)
        {
            if (response->succeed)
            {
                Wallpapers wp;
                wp.reserve(response->request->files.size());

                std::string base_name = CCFileUtils::sharedFileUtils()->getWriteablePath();

                std::vector<std::string>::iterator iter;
                for (iter = response->request->files.begin();
                     iter != response->request->files.end(); ++iter)
                {

                    std::string file_name = base_name + getFileNameFromUrl(*iter);
                    wp.push_back(Wallpaper(file_name));
                }
                _delegate->onWallpapersRecieved(wp);
            }
            else
            {
                _delegate->onWallpapersError(
                            WallpaperDelegate::NetworkError,
                            response->responseData);
            }
        }
    }

    void onHttpRequestCompleted(cocos2d::CCObject *, void *data)
    {
        HttpResponsePacket *response = (HttpResponsePacket *)data;

        if (response->request->reqType == kHttpRequestGet)
        {
            if (response->succeed)
            {
                parse(response->responseData);
            }
            else
            {
                _delegate->onWallpapersError(
                            WallpaperDelegate::NetworkError,
                            response->responseData);
            }
        }
        this->release();
    }
    WallpaperDelegatePtr _delegate;
    unsigned int _max_wallpapers;
    std::string _url;
};


void BingWallpaperProvider::do_getWallpapers(
        WallpaperDelegatePtr delegate,
        unsigned int max_wallpapers)
{
    std::stringstream url_stream;
    url_stream << "http://www.bing.com/HPImageArchive.aspx?format=js&idx=0&n=" << 10;
    std::string url = url_stream.str();

    BingMetaObtainer* obtainer = new BingMetaObtainer(url, delegate, max_wallpapers);
    obtainer->autorelease();
    obtainer->obtain();
}
