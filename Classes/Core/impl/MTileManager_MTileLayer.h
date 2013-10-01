#ifndef MTILEMANAGER_MTILELAYER_H
#define MTILEMANAGER_MTILELAYER_H

#ifndef MTILE_IMPL
#error "Include is forbidden"
#endif

#include "../MTileManager.h"
#include <queue>
#include <set>

class MTileManager::MTileLayer : public cocos2d::CCLayer
{
public:
    MTileLayer();
    MTile* itemForTouch(cocos2d::CCTouch *touch);
    void registerWithTouchDispatcher();

    bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    void ccTouchEnded(cocos2d::CCTouch *touch, cocos2d::CCEvent* event);
    void ccTouchCancelled(cocos2d::CCTouch *, cocos2d::CCEvent*);
    void ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent*);

    void addChild(cocos2d::CCNode *child);
    void removeChild(CCNode* child, bool cleanup);
    typedef std::set<MTile*> Set;
    Set _tiles;
    Set _tiles_being_destroyed;

    Set _animated_tiles;

    void delayedWidgetInitialize(Widget*ptr, float delay=0.3f);
    bool inTilesSet(MTile*);
    bool inAnimationSet(MTile*);
    bool inDestroySet(MTile* tile);
    void keyBackClicked();

private:
    void doDelayedWidgetInitialize(CCObject *);

    cocos2d::tCCMenuState m_eState;
    MTile* m_pSelectedItem;

};

#endif // MTILEMANAGER_MTILELAYER_H
