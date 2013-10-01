#ifndef CURRENTTIMETILE_H
#define CURRENTTIMETILE_H
#include "Core/MTile.h"
class CurrentTimeTile : public MTile
{
public:


    static CurrentTimeTile* create(const MTilePosition& position,
                             const MTileSize& size);
private:
    CurrentTimeTile();
    void init(const MTilePosition& pos, const MTileSize& size);

    void onInit();



    void reserveSpace();

    void rebuildAll(tm* time);
    void rebuildClock(tm* time);

    void tickUpdate(float);

    MTile* _main_tile;
    cocos2d::CCLabelTTF* _upper_label;
    cocos2d::CCLabelTTF* _hour_label;

    int _last_day;
    int _last_minute;

    void scheduleUpdate();
    void unscheduleUpdate();
    bool _scheduled;
};

#endif // CURRENTTIMETILE_H
