#ifndef WALLPAPERPROVIDER_H
#define WALLPAPERPROVIDER_H
#include "Wallpaper.h"
#include <memory>
#include "cocos2d.h"
class WallpaperDelegate
{
public:
    WallpaperDelegate()
    {}

    enum Error
    {
        NetworkError,
        ParseError
    };

    virtual ~WallpaperDelegate()
    {}

    virtual void onWallpapersRecieved(const Wallpapers& wp)
    {
        for(unsigned int i=0; i<wp.size(); ++i)
        {
            cocos2d::CCLog(wp[i].getFileName().c_str());
        }
    }

    virtual void onWallpapersError(const Error type,
                                   const std::string& description)
    {
        type;
        cocos2d::CCLog(description.c_str());
        description;
    }
};

typedef std::auto_ptr<WallpaperDelegate> WallpaperDelegatePtr;
class WallpaperProvider
{
public:
    WallpaperProvider()
        : _last_request(0)
    {}
    void getWallpapers(WallpaperDelegatePtr delegate, unsigned int max_wallpapers = 10)
    {
        _last_request = time(NULL);
        do_getWallpapers(delegate, max_wallpapers);
    }
    const time_t getLastRequestTime() const
    {
        return _last_request;
    }

    virtual ~WallpaperProvider()
    {}
private:
    virtual void do_getWallpapers(WallpaperDelegatePtr delegate, unsigned int max_wallpapers) = 0;

    time_t _last_request;
    WallpaperProvider(const WallpaperProvider&);
    WallpaperProvider& operator=(const WallpaperProvider&);
};
typedef std::auto_ptr<WallpaperProvider> WallpaperProviderPtr;
#endif // WALLPAPERPROVIDER_H
