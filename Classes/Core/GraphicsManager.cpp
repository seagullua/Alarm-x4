#include "GraphicsManager.h"
using namespace cocos2d;
GraphicsManager::GraphicsManager()
{

}

SpritesLoader GraphicsManager::getLoaderFor(cocos2d::CCNode* parent,
                                            const char* plist,
                                            const char* texture)
{
    static GraphicsManager g;
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(plist);
    g;
    return SpritesLoader(new SpritesLoaderClass(parent, texture));
}

CCSprite* GraphicsManager::SpritesLoaderClass::loadSprite(const char * name)
{
    CCSprite* pSprite = CCSprite::createWithSpriteFrameName(name);
    prepareSpriteSheet();
    _sprite_sheet->addChild(pSprite);
    _use_sprite_sheet = true;
    return pSprite;
}
void GraphicsManager::SpritesLoaderClass::removeSprite(CCNode* sprite)
{
    CCAssert(dynamic_cast<CCSprite*>(sprite),"");
    if(_sprite_sheet)
        _sprite_sheet->removeChild(sprite, true);
}

void GraphicsManager::SpritesLoaderClass::prepareSpriteSheet()
{
    if(!_sprite_sheet)
    {
        _sprite_sheet = CCSpriteBatchNode::create(_texture_file.c_str());
        _sprite_sheet->setPosition(ccp(0,0));
    }
}

CCSpriteFrame* GraphicsManager::SpritesLoaderClass::loadSpriteFrame(const char *name)
{
    return CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(name);
}

cocos2d::CCSprite* GraphicsManager::SpritesLoaderClass::loadSpriteWithoutSpriteSheet(const char *name)
{
    return CCSprite::createWithSpriteFrameName(name);
}

GraphicsManager::SpritesLoaderClass::SpritesLoaderClass(cocos2d::CCNode* parent,
                                                        const char* texture)
    : _sprite_sheet(0),
      _parent(parent), _use_sprite_sheet(false),
      _texture_file(texture),
      _injected(false)

{

}
void GraphicsManager::SpritesLoaderClass::setParent(cocos2d::CCNode* p)
{
    _parent = p;
}

GraphicsManager::SpritesLoaderClass::~SpritesLoaderClass()
{
    inject(false);
}
void GraphicsManager::SpritesLoaderClass::inject(bool real_inject)
{
    if(!_injected)
    {
        _injected = true;
        if(_use_sprite_sheet || real_inject)
        {

            prepareSpriteSheet();
            assert(_parent);
            _parent->addChild(_sprite_sheet);
        }
    }
}
