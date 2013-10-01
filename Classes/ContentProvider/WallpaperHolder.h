#ifndef WALLPAPERHOLDER_H
#define WALLPAPERHOLDER_H
#include "cocos2d.h"
#include "WallpaperProvider.h"
#include "LTask.h"

class WallpaperGetDelegate
{
public:
    WallpaperGetDelegate()
    {}
    virtual void wallpaperArrived(cocos2d::CCSprite*)
    {}
    virtual ~WallpaperGetDelegate()
    {}
};
typedef std::auto_ptr<WallpaperGetDelegate> WallpaperGetDelegatePtr;

class WallpaperHolder
{
public:
    static WallpaperHolder& sharedHolder();
    void setWallpaperProvider(WallpaperProviderPtr);
    void getWallpaper(WallpaperGetDelegatePtr dt);
private:
    class ProviderUpdateTask;
    class FlushTask : public LTask
    {
    private:
        void do_performTask(LTaskDelegatePtr delegate)
        {

            WallpaperHolder& holder = sharedHolder();

            holder.saveWallpapers();
            delegate->onTaskFinished();
        }
    };


    WallpaperHolder();
    ~WallpaperHolder()
    {}
    WallpaperHolder(WallpaperHolder&);
    WallpaperHolder& operator=(const WallpaperHolder&);

    void saveWallpapers();
    void loadWallpapers();
    void downloadWallpapers();
    void rebuildWallpapers();

    std::vector<std::string> _default;
    Wallpapers _downloaded;
    bool _wallpapers_dirty;
    WallpaperProviderPtr _provider;
    static std::string _wallpaper_file_name;

    static WallpaperHolder* _holder;
};

#endif // WALLPAPERHOLDER_H
