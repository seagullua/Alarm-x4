#ifndef MTILEMANAGER_ANIMATIONFINALIZER_H
#define MTILEMANAGER_ANIMATIONFINALIZER_H

#ifndef MTILE_IMPL
#error "Include is forbidden"
#endif

#include "../MTileManager.h"
using namespace cocos2d;


class MTileManager::AnimationFinalizer : public CCObject
{
public:


    static AF* create(MTile* tile)
    {
        AF* obj = new AnimationFinalizer(tile);
        if(obj)
        {
            obj->autorelease();
            obj->retain();
        }
        return obj;
    }

    void finalize()
    {
        MLayer* ml = MTileManager::sharedManager()._main_layer;
        ml->_animated_tiles.erase(_tile);
        ml->_tiles.insert(_tile);
        this->release();
    }

private:
    AnimationFinalizer(MTile* tile)
        : _tile(tile)
    {
        MLayer* ml = MTileManager::sharedManager()._main_layer;
        ml->_animated_tiles.insert(_tile);
        ml->_tiles.erase(_tile);
    }
    MTile* _tile;

};

#endif // MTILEMANAGER_ANIMATIONFINALIZER_H
