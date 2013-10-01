#include "VKRssJokeProvider.h"
#include "cocos2d.h"
#include "Extension/CCHttpRequest.h"
using namespace cocos2d;
VKRssJokeProvider::VKRssJokeProvider(const std::string& feed_url, bool rebuild_url)
    : _feed_url(feed_url), _rebuild_url(rebuild_url)
{}

void replaceStringInPlace(std::string& subject, const std::string& search,
                          const std::string& replace) {
    size_t pos = 0;
    while ((pos = subject.find(search, pos)) != std::string::npos) {
        subject.replace(pos, search.length(), replace);
        pos += replace.length();
    }
}

class VKRssJokesParser
{
public:
    VKRssJokesParser(const std::string& xml)
        : _xml(xml)
    {}
    bool parse()
    {
        _jokes.reserve(10);
        CCSAXParser parser;
        Parser p;
        parser.setDelegator(&p);
        replaceStringInPlace(_xml, "&lt;br&gt;", "\n");
        replaceStringInPlace(_xml, "&lt;br /&gt;", "\n");


        bool res = parser.parse(_xml.c_str(), _xml.size());
        _jokes = p._jokes;
        return res;
    }

    const Jokes& getJokes() const
    {
        return _jokes;
    }
    ~VKRssJokesParser()
    {}
private:
    class Parser : public CCSAXDelegator
    {
    public:
        Parser()
            : _item_found(false),
              _description(false)
        {

        }

        Jokes _jokes;
    private:
        bool _item_found;
        bool _description;
        std::string _buffer;
        void startElement(void *ctx, const char *name, const char **atts)
        {
            CC_UNUSED_PARAM(ctx);
            CC_UNUSED_PARAM(atts);
            std::string sName((char*)name);
            if( sName == "item" )
            {
                _item_found = true;
            }
            else if(_item_found && sName == "description")
            {
                _description = true;
            }
            else
            {
                _description = false;
            }
        }

        void endElement(void *ctx, const char *name)
        {
            CC_UNUSED_PARAM(ctx);

            std::string sName((char*)name);
            if( sName == "description" )
            {
                _description = false;
                if(_buffer.size())
                {
                    //No adds
                    if(_buffer.find("href=", 0) == std::string::npos)
                    {
                        replaceStringInPlace(_buffer, "&lt;", "<");
                        replaceStringInPlace(_buffer, "&gt;", ">");
                        replaceStringInPlace(_buffer, "&quot;", "\"");
                        replaceStringInPlace(_buffer, "&amp;", "&");
                        if(_buffer.size() < 300)
                            _jokes.push_back(_buffer);
                    }
                    _buffer.clear();
                }
            }
            else if (sName == "item")
            {
                _description = false;
            }

        }

        void textHandler(void *ctx, const char *ch, int len)
        {
            CC_UNUSED_PARAM(ctx);
            if(_description)
            {
                std::string joke((char*)ch,0,len);
                _buffer += joke;


            }
        }


    };

    VKRssJokesParser& operator=(const VKRssJokesParser&);
    VKRssJokesParser(VKRssJokesParser&);
    std::string _xml;
    Jokes _jokes;
};

class VKRssObtainer : public CCObject
{
public:
    VKRssObtainer(const std::string& rss_url, JokesDelegatePtr& delegate)
        : _rss_url(rss_url), _delegate(JokesDelegatePtr(delegate))
    {
        this->retain();
    }
    void obtain()
    {
        CCHttpRequest *requestor = CCHttpRequest::sharedHttpRequest();
        requestor->addGetTask(_rss_url, this,
                              callfuncND_selector(VKRssObtainer::onHttpRequestCompleted));
    }

private:
    void parse(const std::string& xml_data)
    {
        VKRssJokesParser parser(xml_data);
        if(parser.parse())
        {
            _delegate->onJokesRecieved(parser.getJokes());
        }
        else
        {
            _delegate->onJokesError(JokesDelegate::ParseError, "");
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
                _delegate->onJokesError(JokesDelegate::NetworkError, response->responseData);
            }
        }
        this->release();
    }
    JokesDelegatePtr _delegate;
    std::string _rss_url;
};

#include <fstream>

void VKRssJokeProvider::do_getJokes(JokesDelegatePtr delegate)
{
    std::string group_url = _feed_url;

    if(_rebuild_url)
    {
        //http://vkontakte-feed.appspot.com/feed/humor_ukraine/wall?show_photo=0
        std::string url = _feed_url;
        size_t last_slash = url.find_last_of('/');
        if(last_slash != std::string::npos)
        {
            std::string group_name = url.substr(last_slash+1);
            group_url = "http://vkontakte-feed.appspot.com/feed/"+
                    group_name+"/wall?show_photo=0";

        }
    }

    VKRssObtainer* obtainer = new VKRssObtainer(group_url, delegate);
    obtainer->autorelease();
    obtainer->obtain();
}

JokeProviderID VKRssJokeProvider::do_getProviderID() const
{
    return _feed_url;
}
