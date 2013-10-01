#include "WallpaperHolder.h"
#include "Core/Binary.h"
#include "cocos2d.h"
#include <algorithm>
#include <fstream>
#include "Core/file_exists.h"
#include "LTaskManager.h"

using namespace cocos2d;
std::string WallpaperHolder::_wallpaper_file_name;
WallpaperHolder* WallpaperHolder::_holder = 0;

bool operator<(const Wallpaper& a, const Wallpaper& b)
{
    return (a.getTimesShowed() < b.getTimesShowed()
            || (a.getTimesShowed() == b.getTimesShowed() && a.getRecieveTime() > b.getRecieveTime()));
}

InputBinaryStream& operator>>(InputBinaryStream& is, Wallpaper& w)
{
    std::string text;
    uint64_t time=0;
    uint32_t times_showed = 0;
    is >> text >> time >> times_showed;
    w.setFileName(text);
    w.setRecieveTime(time);
    w.setTimesShowed(times_showed);
    return is;
}

OutputBinaryStream& operator<<(OutputBinaryStream& os, const Wallpaper& w)
{
    os << w.getFileName();
    os << uint64_t(w.getRecieveTime());
    os << w.getTimesShowed();
    return os;
}

WallpaperHolder& WallpaperHolder::sharedHolder()
{
    if(!_holder)
    {
        _holder = new WallpaperHolder;
    }
    return *_holder;
}

WallpaperHolder::WallpaperHolder()
    : _wallpapers_dirty(false), _provider(0)
{
    std::string wpath = CCFileUtils::sharedFileUtils()->getWriteablePath();
    _wallpaper_file_name = wpath + "wallpapers.ad";
    loadWallpapers();

    _default.push_back("bing.jpg");
    _default.push_back("bing2.jpg");
}

void WallpaperHolder::saveWallpapers()
{
    if(_wallpapers_dirty)
    {
        _wallpapers_dirty = false;

        std::ofstream oss(_wallpaper_file_name.c_str(), std::ios::out | std::ios::binary);
        OutputBinaryStream os(oss,BinaryStream::MaxProtocolVersion);

        os << _downloaded;
    }
}

void WallpaperHolder::loadWallpapers()
{
    if(fileExists(_wallpaper_file_name.c_str()))
    {
        std::ifstream iss(_wallpaper_file_name.c_str(), std::ios::in | std::ios::binary);
        InputBinaryStream is(iss);

        is >> _downloaded;
        std::sort(_downloaded.begin(), _downloaded.end());
    }
}

class WallpaperHolder::ProviderUpdateTask : public LTask
{
public:
    ProviderUpdateTask(const unsigned int download_num)
        : _download_num(download_num)
    {}

    void do_performTask(LTaskDelegatePtr delegate)
    {
        WallpaperHolder& holder = WallpaperHolder::sharedHolder();
        if(holder._provider.get() == 0)
            delegate->onTaskFinished();
        else
        {
            holder._provider->getWallpapers(
                        WallpaperDelegatePtr(new Reciever(delegate)),
                        _download_num);
        }
    }

private:
    class Reciever : public WallpaperDelegate
    {
    public:
        Reciever(LTaskDelegatePtr delegate)
            : _task_delegate(delegate)
        {}
        virtual void onWallpapersRecieved(const Wallpapers& wp)
        {
            time_t arrival_scan = time(0);
            WallpaperHolder& holder = WallpaperHolder::sharedHolder();

            holder._downloaded.reserve(holder._downloaded.size() + wp.size());
            for(unsigned int i=0; i<wp.size(); ++i)
            {
                Wallpaper w(wp[i]);
                w.setRecieveTime(arrival_scan+i);
                w.setTimesShowed(0);
                holder._downloaded.push_back(w);
            }
            holder.rebuildWallpapers();
            holder._wallpapers_dirty = true;

            _task_delegate->onTaskFinished();
        }
        void onWallpapersError(const Error,
                               const std::string&)
        {
            _task_delegate->onTaskFinished();
        }
    private:
        LTaskDelegatePtr _task_delegate;
    };

    unsigned int _download_num;
};

void WallpaperHolder::rebuildWallpapers()
{
    std::sort(_downloaded.begin(), _downloaded.end());

    const unsigned int MAX_WALLPAPERS = 10;
    unsigned int first_found = _downloaded.size();
    if(_downloaded.size() > MAX_WALLPAPERS)
    {
        for(unsigned int i=MAX_WALLPAPERS; i<_downloaded.size(); ++i)
        {
            if(_downloaded[i].getTimesShowed() > 0)
            {
                first_found = i;
                break;
            }
        }
        if(first_found != _downloaded.size())
        {

            for(unsigned int i=first_found; i<_downloaded.size(); ++i)
            {
                remove(_downloaded[i].getFileName().c_str());
            }
            _downloaded.erase(_downloaded.begin() + first_found, _downloaded.end());
        }
    }
    _wallpapers_dirty = true;
    LTaskManager::sharedManager().addTask(LTaskPtr(new FlushTask));
}

void WallpaperHolder::downloadWallpapers()
{
    unsigned int first_found = _downloaded.size();
    const unsigned int MAX_WALLPAPERS = 10;
    const unsigned int MAX_PER_TIME = 2;
    for(unsigned int i=0; i<_downloaded.size(); ++i)
    {
        if(_downloaded[i].getTimesShowed() > 0)
        {
            first_found = i;
            break;
        }
    }
    if(first_found < MAX_WALLPAPERS)
    {
        unsigned int download_wp = MAX_WALLPAPERS - first_found;
        if(download_wp > 0 && download_wp <= MAX_WALLPAPERS)
        {
            if(download_wp > MAX_PER_TIME)
                download_wp = MAX_PER_TIME;
            LTaskManager::sharedManager().addTask(
                        LTaskPtr(
                            new ProviderUpdateTask(download_wp)));
        }
    }
}

void WallpaperHolder::setWallpaperProvider(WallpaperProviderPtr prov)
{
    _provider = prov;
}

void WallpaperHolder::getWallpaper(WallpaperGetDelegatePtr dt)
{
    downloadWallpapers();
    std::string file_name;
    if(_downloaded.size() > 0)
    {
        //static unsigned int last_showed = 0;
        unsigned int idx = 0;
        if(_downloaded[idx].getTimesShowed() == 1)
            idx = rand() % _downloaded.size();

        file_name = _downloaded[idx].getFileName();
        _downloaded[idx].incrementTimesShowed();
        rebuildWallpapers();
    }
    else
    {
        file_name = _default[rand() % _default.size()];
    }

    dt->wallpaperArrived(CCSprite::create(file_name.c_str()));

}
