#ifndef MTILEMANAGER_CREATETILEWITHDELAY_H
#define MTILEMANAGER_CREATETILEWITHDELAY_H
#ifndef MTILE_IMPL
#error "Include is forbidden"
#endif

#include "../MTileManager.h"
#include "MTileManager_MTileLayer.h"
#include "cocos2d.h"
using namespace cocos2d;

class MTileManager::CreateTileWithDelay : public CCObject
{
public:
    CreateTileWithDelay(MTile* tile, float delay,
                        MTileManager::Api* api)
        : _tile(tile), _delay(delay), _api(api)
    {

    }
    ~CreateTileWithDelay()
    {}
    void run()
    {
        this->autorelease();
        this->retain();
        _tile->retain();
        MTileManager::sharedManager()._main_layer->runAction(
                    CCSequence::createWithTwoActions(
                        CCDelayTime::create(_delay),
                        CCCallFunc::create(
                            this,
                            callfunc_selector(
                                CreateTileWithDelay::addTile))
                        ));
    }

private:
    void addTile()
    {
        _api->addTile(_tile);
        _tile->release();
        this->release();
    }

    MTile* _tile;
    MTileManager::Api* _api;
    float _delay;
};

#endif // MTILEMANAGER_CREATETILEWITHDELAY_H
