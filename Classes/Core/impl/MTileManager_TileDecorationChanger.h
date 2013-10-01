#ifndef MTILEMANAGER_TILEDECORATIONCHANGER_H
#define MTILEMANAGER_TILEDECORATIONCHANGER_H

#ifndef MTILE_IMPL
#error "Include is forbidden"
#endif

#include "../MTileManager.h"
using namespace cocos2d;

class TileDecorationChanger : public CCObject
{
public:
    static TileDecorationChanger* create(MTile* tile,
                                         const MTilePosition& new_pos,
                                         const MTileSize& new_size)
    {
        TileDecorationChanger* obj = new TileDecorationChanger(
                    tile, new_pos, new_size);
        if(obj)
        {
            obj->autorelease();
            obj->retain();
        }
        return obj;
    }
    void finalize()
    {
        _tile->setTilePosition(_new_pos);
        _tile->setTileSize(_new_size);
        this->release();
    }

private:
    TileDecorationChanger(MTile* tile,
                          const MTilePosition& new_pos,
                          const MTileSize& new_size)
        : _tile(tile), _new_pos(new_pos), _new_size(new_size)
    {}
    MTile* _tile;
    MTilePosition _new_pos;
    MTileSize _new_size;
};
typedef TileDecorationChanger TDC;

#endif // MTILEMANAGER_TILEDECORATIONCHANGER_H
