#ifndef BINGWALLPAPERPROVIDER_H
#define BINGWALLPAPERPROVIDER_H
#include "WallpaperProvider.h"
class BingWallpaperProvider : public WallpaperProvider
{
public:
    BingWallpaperProvider();
private:
    void do_getWallpapers(WallpaperDelegatePtr delegate,
                          unsigned int max_wallpapers);

};

#endif // BINGWALLPAPERPROVIDER_H
