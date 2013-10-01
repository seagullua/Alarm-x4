#include "MTile.h"
#include "MTileGrid.h"
using namespace cocos2d;

MTile* MTile::create(const char* background,
                     const MTilePosition& pos,
                     const MTileSize& size,
                     CCObject *rec,
                     SEL_TileHandler selector)
{
    MTile* obj = new MTile();
    if(obj)
    {
        obj->autorelease();
        obj->init(background, pos, size, rec, selector);
    }
    return obj;
}

const MTilePosition MTile::getTilePosition() const
{
    return _my_pos;
}
void MTile::setTilePosition(const MTilePosition& pos)
{
    _my_pos = pos;
    updatePosition();
}

const MTileSize MTile::getTileSize() const
{
    return _my_size;
}
void MTile::setTileSize(const MTileSize& size)
{
    _my_size = size;
    updateSize();
}

CCSize MTile::getContentSize()
{
    CCSize not_balanced(_my_size.width() * MTileGrid::getGridSize() +
                        (_my_size.width() - 1) * MTileGrid::getGridSpacing(),
                        _my_size.height() * MTileGrid::getGridSize() +
                        (_my_size.height() - 1) * MTileGrid::getGridSpacing());
    float scaled = CCDirector::sharedDirector()->getContentScaleFactor();
    return CCSize(not_balanced.width / scaled,
                  not_balanced.height / scaled);
}

CCSize MTile::getInnerContentSize()
{
    CCSize full_size = getContentSize();
    float scaled = CCDirector::sharedDirector()->getContentScaleFactor();
    return CCSize(full_size.width - MTileGrid::getPadding() * 2 / scaled,
                  full_size.height - MTileGrid::getPadding() * 2 / scaled);
}

void MTile::addChild(CCNode* child,
                     const Aligment::Horizontal horizontal,
                     const Aligment::Vertical vertical,
                     float margin_x, float margin_y)
{
    CCNode::addChild(child);
    float scaled = CCDirector::sharedDirector()->getContentScaleFactor();
    const float padding_s = MTileGrid::getPadding() / scaled;
    float x_cord = 0;
    float x_anchor = 0;

    float y_cord = 0;
    float y_anchor = 0;

    CCSize tile_size = this->getContentSize();
    //Horizontal placement
    if(horizontal == Aligment::Left)
    {
        x_anchor = 0;
        x_cord = padding_s + margin_x;
    }
    else if(horizontal == Aligment::Right)
    {
        x_anchor = 1;
        x_cord = tile_size.width - padding_s - margin_x;
    }
    else if(horizontal == Aligment::HCenter)
    {
        x_anchor = 0.5f;
        x_cord = (tile_size.width + margin_x) / 2;
    }
    else
    {
        x_anchor = 0;
        x_cord = margin_x;
    }

    //Vertical placement
    if(vertical == Aligment::Top)
    {
        y_anchor = 1;
        y_cord = tile_size.height - padding_s - margin_y;
    }
    else if(vertical == Aligment::Bottom)
    {
        y_anchor = 0;
        y_cord = padding_s + margin_y;
    }
    else if(vertical == Aligment::VCenter)
    {
        y_anchor = 0.5f;
        y_cord = (tile_size.height - margin_y) / 2;
    }
    else
    {
        y_anchor = 1;
        y_cord = tile_size.height - margin_y;
    }

    child->setPosition(ccp(x_cord, y_cord));
    child->setAnchorPoint(ccp(x_anchor, y_anchor));
}

MTile::~MTile()
{}

void MTile::init(const char* background,
                 const MTilePosition& pos,
                 const MTileSize& size,
                 CCObject *rec, SEL_TileHandler selector)
{
    _main_texture = CCSprite::create(background);
    _main_texture->setOpacity(230);
    _main_texture->setAnchorPoint(ccp(0,0));
    _main_texture->setPosition(ccp(0,0));
    CCNode::addChild(_main_texture);
    _my_pos = pos;
    _my_size = size;
    updateSize();
    updatePosition();
    _callback_obj = rec;
    _callback_ptr = selector;
}

void MTile::updatePosition()
{
    float scaled = CCDirector::sharedDirector()->getContentScaleFactor();
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

    float x_top = _my_pos.x() * MTileGrid::getGridSize() / scaled +
            (_my_pos.x() + 1) * MTileGrid::getGridSpacing() / scaled;
    float y_top = _my_pos.y() * MTileGrid::getGridSize() / scaled +
            (_my_pos.y() + 1) * MTileGrid::getGridSpacing() / scaled;
    CCSize my_real_size = this->getContentSize();
    this->setAnchorPoint(ccp(0.5, 0.5));
    this->setPosition(ccp(origin.x + x_top + my_real_size.width/2,
                          origin.y + visibleSize.height - y_top - my_real_size.height/2));
    updateRect();
}
void MTile::selfDestroy()
{
    this->removeFromParentAndCleanup(true);
}

void MTile::updateSize()
{
    CCSize real_size = this->getContentSize();
    CCSize texture_size = _main_texture->getContentSize();

    float scale_x = real_size.width / texture_size.width;
    float scale_y = real_size.height / texture_size.height;

    _main_texture->setScaleX(scale_x);
    _main_texture->setScaleY(scale_y);

    CCNode::setContentSize(this->getContentSize());
    //updateRect();
    updatePosition();
}
MTile::MTile()
    : _my_pos(0,0), _my_size(0,0), _callback_obj(0), _callback_ptr(0)

{}


void MTile::onHoverBegin()
{
    float scaled = CCDirector::sharedDirector()->getContentScaleFactor();
    this->stopAllActions();
    CCSize size = this->getContentSize();
    float target_width = size.width + MTileGrid::getGridSpacing() / scaled * 1.0f;
    float target_height = size.height + MTileGrid::getGridSpacing() / scaled * 1.0f;
    float scale = MIN(target_width / size.width,
                      target_height / size.height);
    this->runAction(
                CCScaleTo::create(
                    0.05f, scale));
}

void MTile::onHoverEnd()
{
    this->stopAllActions();
    this->runAction(CCScaleTo::create(0.05f, 1.0f));
}
bool MTile::isClickable() const
{
    return _callback_obj != 0;
}
#include "SimpleAudioEngine.h"
void MTile::onClick()
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("choose.wav");
    if(_callback_obj && _callback_ptr)
    {
        (_callback_obj->*_callback_ptr)(this);
    }
}
void MTile::updateRect()
{
    CCSize size = getContentSize();
    _rect = CCRect(this->getPositionX(), this->getPositionY(),
                   size.width, size.height);
}
#include "LabelFactory.h"
void addImageAndLabelToTile(
        MTile* tile, const char* labelt, const char* icon)
{
    float scaled = CCDirector::sharedDirector()->getContentScaleFactor();

    CCSprite* image = CCSprite::create(icon);
    CCSize image_size = image->getContentSize();
    float image_rect = MAX(image_size.height, image_size.width);

    CCSize tile_size = tile->getInnerContentSize();
    float image_scale = tile_size.height / image_rect;
    image->setScale(image_scale);

    float label_font = LF::MEDIUM_FONT;
    std::string label_str(labelt);
    if(label_str.size()>30)
        label_font *= 0.7f;

    float image_width = image_size.width*image_scale + MTileGrid::getPadding() / scaled;
    CCLabelTTF* label_ttf = LF::label(
                labelt, label_font,
                CCSize(tile_size.width - image_width,
                       tile_size.height),
                kCCTextAlignmentCenter,
                kCCVerticalTextAlignmentCenter);
    tile->addChild(label_ttf, Aligment::HCenter,
                   Aligment::VCenter,
                   image_width,
                   LF::vfix(label_font));

    tile->addChild(image, Aligment::Left, Aligment::VCenter);
}

void addLabelToTile(MTile* tile, const char* labelt)
{
    CCSize tile_size = tile->getInnerContentSize();
    float label_font = LF::MEDIUM_FONT;
    std::string label_str(labelt);
    if(label_str.size()>30)
        label_font *= 0.7f;

    CCLabelTTF* label_ttf = LF::label(
                labelt, label_font,
                CCSize(tile_size.width,
                       tile_size.height),
                kCCTextAlignmentLeft,
                kCCVerticalTextAlignmentCenter);
    tile->addChild(label_ttf, Aligment::HCenter,
                   Aligment::VCenter,
                   0,
                   LF::vfix(label_font));
}
