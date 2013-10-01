#define MTILE_IMPL
#include "MTileManager.h"
#include "ContentProvider/LTaskManager.h"
#include "impl/MTileManager_MTileLayer.h"
#include "Script.h"
#include "Widget.h"
using namespace cocos2d;

MTileManager* MTileManager::_manager = 0;




MTileManager::MTileManager()
    : _main_layer(0), _grid_sprite(0), _grid_shown(false), _actioner(0)
{
    _main_layer = new MTileLayer;
    _main_layer->autorelease();
    _main_layer->retain();

    CCNode* task_manager = &LTaskManager::sharedManager();
    _main_layer->addChild(task_manager);
    task_manager->schedule(schedule_selector(LTaskManager::updateEvent), 0.5f, kCCRepeatForever, 0.3f);

    _actioner = CCNode::create();
    _main_layer->addChild(_actioner);
}
void MTileManager::runAction(cocos2d::CCAction* action)
{
    _actioner->runAction(action);
}

void MTileManager::cancelAction(cocos2d::CCAction* action)
{
    _actioner->stopAction(action);
}

MTileManager::~MTileManager()
{
    _main_layer->release();
}

MTileManager& MTileManager::sharedManager()
{
    if(_manager == 0)
        _manager = new MTileManager;
    return *_manager;
}
void MTileManager::switchToScene(cocos2d::CCNode* parent)
{
    if(_main_layer)
    {
        _main_layer->removeFromParentAndCleanup(false);
        parent->addChild(_main_layer);
    }
}

void MTileManager::displayGrid(bool to_display)
{
    if(to_display != _grid_shown)
    {
        _grid_shown = to_display;
        if(!_grid_shown)
        {
            if(_grid_sprite)
                _grid_sprite->removeFromParent();
        }
        else
        {
            if(!_grid_sprite)
            {
                CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
                CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

                _grid_sprite = CCSprite::create("grid.png");
                _grid_sprite->setAnchorPoint(ccp(0,1));
                _grid_sprite->setPosition(ccp(origin.x, origin.y+visibleSize.height));
            }
            _main_layer->CCLayer::addChild(_grid_sprite, -1);
        }

    }
}

const MTileGrid& MTileManager::getGrid() const
{
    return _grid;
}

void MTileManager::runScript(ScriptPtr script)
{
    removeAllWidgets();
    _widgets_waiting_to_appear.clear();

    Script* ptr = script.get();
    _active_script = script;
    ptr->initialize(ExWApiPtr(new ExWApi(ptr)));

}

void MTileManager::removeAllTilesOfWidget(Widget* widget)
{
    MTilesSet set = _widgets_tiles[widget];
    for(MTilesSet::iterator i = set.begin();
        i != set.end();
        ++i)
    {
        widget->getApi()->removeTile(*i);
    }
}

void MTileManager::removeAllChildWidget(const WidgetSet& s)
{
    for(WidgetSet::iterator i = s.begin(); i!=s.end(); ++i)
    {
        _active_script->getExApi()->removeWidget(*i);
    }
}

void MTileManager::removeAllWidgets()
{
    while(_widgets.size() != 0)
    {
        _active_script->getExApi()->removeWidget(_widgets.begin()->first);
    }
}




