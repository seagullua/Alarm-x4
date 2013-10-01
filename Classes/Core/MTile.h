#ifndef TILE_H
#define TILE_H
#include "cocos2d.h"
#include "MTilePosition.h"
#include "MTileSize.h"
#include "Alignment.h"
#include "Logic/Lang.h"
#include <memory>

class MTile;
typedef void (cocos2d::CCObject::*SEL_TileHandler)(MTile*);
#define tile_selector(_SELECTOR) (SEL_TileHandler)(&_SELECTOR)

class MTile : public cocos2d::CCNode
{
public:

    static MTile* create(const char* background,
                        const MTilePosition&,
                        const MTileSize&,
                        cocos2d::CCObject *rec=0, SEL_TileHandler selector=0);

    const MTilePosition getTilePosition() const;
    void setTilePosition(const MTilePosition& pos);

    const MTileSize getTileSize() const;
    void setTileSize(const MTileSize& size);

    virtual cocos2d::CCSize getContentSize();
    cocos2d::CCSize getInnerContentSize();

    void onHoverBegin();
    void onHoverEnd();
    void onClick();
    bool isClickable() const;

    void addChild(CCNode* child, const Aligment::Horizontal horizontal,
                  const Aligment::Vertical vertical,
                  float margin_x=0, float margin_y=0);
    const cocos2d::CCRect& rect() const
    {
        return _rect;
    }
    void selfDestroy();
    ~MTile();
protected:
    void init(const char* background,
              const MTilePosition&,
              const MTileSize&,
              cocos2d::CCObject *rec,
              SEL_TileHandler selector);
    MTile();
private:



    void updatePosition();
    void updateSize();
    void updateRect();

    MTile(const MTile&);
    MTile& operator=(const MTile&);
    MTilePosition _my_pos;
    MTileSize _my_size;
    cocos2d::CCSprite* _main_texture;
    cocos2d::CCObject *_callback_obj;
    SEL_TileHandler _callback_ptr;
    cocos2d::CCRect _rect;

};

void addImageAndLabelToTile(
        MTile* tile, const char* label, const char* icon);

void addLabelToTile(MTile* tile, const char* labelt);

#endif // TILE_H
