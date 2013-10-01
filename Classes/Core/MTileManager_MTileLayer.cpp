#define MTILE_IMPL
#include "impl/MTileManager_MTileLayer.h"
#include "Script.h"
#include "Widget.h"

#include "ContentProvider/JokesHolder.h"
#include "ContentProvider/BingWallpaperProvider.h"
#include "ContentProvider/WallpaperHolder.h"
#include "ContentProvider/VKRssJokeProvider.h"

using namespace cocos2d;

class WallpaperCallback : public WallpaperGetDelegate
{
public:
    WallpaperCallback(CCNode* parent)
        : _parent(parent)
    {
    }

    void wallpaperArrived(CCSprite * wallpaper)
    {
        CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
        CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
        CCSize wallpaper_size = wallpaper->getContentSize();
        float scale = MAX(visibleSize.height / wallpaper_size.height,
                          visibleSize.width / wallpaper_size.width);
        wallpaper->setScale(scale);
        wallpaper->setPosition(ccp(origin.x+visibleSize.width/2,
                                   origin.y+visibleSize.height/2));
        _parent->addChild(wallpaper);

    }

private:
    CCNode* _parent;
};

MTileManager::MTileLayer::MTileLayer()
    : m_pSelectedItem(0), m_eState(kCCMenuStateWaiting)
{
    setTouchMode(kCCTouchesOneByOne);
    setTouchEnabled(true);
    setKeypadEnabled(true);







    WallpaperHolder::sharedHolder().setWallpaperProvider(
                WallpaperProviderPtr(new BingWallpaperProvider));
    WallpaperHolder::sharedHolder().getWallpaper(
                WallpaperGetDelegatePtr(new WallpaperCallback(this)));

}
void MTileManager::MTileLayer::keyBackClicked()
{
    MTileManager& m = MTileManager::sharedManager();
    if(m._active_script.get())
        m._active_script->getExApi()->goBack();
}

bool MTileManager::MTileLayer::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
    CC_UNUSED_PARAM(event);
    if (m_eState != kCCMenuStateWaiting)
    {
        return false;
    }


    m_pSelectedItem = this->itemForTouch(touch);
    if (m_pSelectedItem)
    {
        m_eState = kCCMenuStateTrackingTouch;
        m_pSelectedItem->onHoverBegin();
        return true;
    }
    return false;
}

void MTileManager::MTileLayer::ccTouchEnded(CCTouch *touch, CCEvent* event)
{
    CC_UNUSED_PARAM(touch);
    CC_UNUSED_PARAM(event);
    CCAssert(m_eState == kCCMenuStateTrackingTouch, "[Menu ccTouchEnded] -- invalid state");
    if (m_pSelectedItem && inTilesSet(m_pSelectedItem))
    {
        m_pSelectedItem->onHoverEnd();
        m_pSelectedItem->onClick();
    }
    m_eState = kCCMenuStateWaiting;
}

void MTileManager::MTileLayer::ccTouchMoved(CCTouch* touch, CCEvent*)
{
    CCAssert(m_eState == kCCMenuStateTrackingTouch, "[Menu ccTouchMoved] -- invalid state");
    MTile *currentItem = this->itemForTouch(touch);
    if (currentItem != m_pSelectedItem)
    {
        if (m_pSelectedItem && inTilesSet(m_pSelectedItem))
        {
            m_pSelectedItem->onHoverEnd();
        }
        m_pSelectedItem = currentItem;
        if (m_pSelectedItem)
        {
            m_pSelectedItem->onHoverBegin();
        }
    }
}

void MTileManager::MTileLayer::ccTouchCancelled(CCTouch *, CCEvent*)
{
    CCAssert(m_eState == kCCMenuStateTrackingTouch, "[Menu ccTouchCancelled] -- invalid state");
    if (m_pSelectedItem && inTilesSet(m_pSelectedItem))
    {
        m_pSelectedItem->onHoverEnd();
    }
    m_eState = kCCMenuStateWaiting;
}
bool MTileManager::MTileLayer::inTilesSet(MTile* tile)
{
    return _tiles.find(tile) != _tiles.end();
}
bool MTileManager::MTileLayer::inAnimationSet(MTile* tile)
{
    return _animated_tiles.find(tile) != _animated_tiles.end();
}
bool MTileManager::MTileLayer::inDestroySet(MTile* tile)
{
    return _tiles_being_destroyed.find(tile) != _tiles_being_destroyed.end();
}



void MTileManager::MTileLayer::addChild(cocos2d::CCNode *child)
{

    CCLayer::addChild(child);
}
void MTileManager::MTileLayer::removeChild(CCNode* child, bool cleanup)
{
    MTile* tile = dynamic_cast<MTile*>(child);
    if(tile)
    {
        tile->release();
        _tiles.erase(tile);
        _animated_tiles.erase(tile);
        _tiles_being_destroyed.erase(tile);
    }
    CCLayer::removeChild(child, cleanup);
}


MTile* MTileManager::MTileLayer::itemForTouch(CCTouch *touch)
{
    CCPoint touchLocation = touch->getLocation();

    if (_tiles.size() > 0)
    {
        for(Set::iterator it = _tiles.begin(); it != _tiles.end(); ++it)
        {
            MTile* pChild = *it;
            if (pChild && pChild->isVisible() && pChild->isClickable())
            {
                CCPoint local = pChild->convertToNodeSpace(touchLocation);
                CCRect r = pChild->rect();
                r.origin = CCPointZero;

                if (r.containsPoint(local))
                {
                    return pChild;
                }
            }
        }
    }

    return NULL;
}


void MTileManager::MTileLayer::registerWithTouchDispatcher()
{
    CCDirector* pDirector = CCDirector::sharedDirector();
    pDirector->getTouchDispatcher()->addTargetedDelegate(this, kCCMenuHandlerPriority, true);
}


void MTileManager::MTileLayer::delayedWidgetInitialize(Widget* ptr, float delay)
{
    ptr->retain();
    this->runAction(CCSequence::createWithTwoActions(
                        CCDelayTime::create(delay),
                        CCCallFuncO::create(
                            this,
                            callfuncO_selector(MTileManager::MTileLayer::doDelayedWidgetInitialize),
                            ptr)));
}
#include "impl/InverseAction.h"
void MTileManager::MTileLayer::doDelayedWidgetInitialize(CCObject* ptr_o)
{
    MTileManager& m = MTileManager::sharedManager();

    Widget* ptr = dynamic_cast<Widget*>(ptr_o);
    //assert(ptr);

    WidgetSet::iterator it = m._widgets_waiting_to_appear.find(ptr);
    if(ptr && it != m._widgets_waiting_to_appear.end())
    {
        m._widgets_waiting_to_appear.erase(it);

        if(!ptr->isInitialized())
            ptr->initialize(ApiPtr(new Api(ptr)));
        else if(ptr->isHidden())
            ptr->awake();

        ptr->release();

        if(m._active_script->getApi()->_auto_back)
            m._active_script->getApi()->addInverseAction(InverseActionPtr(new HideWidget(ptr)));
    }
}

//void MTileManager::MTileLayer::everyFrame(float)
//{
//    while(!_widget_initialize_queue.empty())
//    {
//        Widget* ptr = _widget_initialize_queue.front();
//        _widget_initialize_queue.pop();


//    }
//}
