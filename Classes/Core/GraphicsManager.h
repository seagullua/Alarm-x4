#ifndef GRAPHICSMANAGER_H
#define GRAPHICSMANAGER_H
#include "cocos2d.h"
#include <memory>
#include <string>
/**
 * @brief Class which manages all graphics in game
 */
class GraphicsManager
{
public:

    class SpritesLoaderClass
    {
    public:
        /**
         * @brief Load sprite and add it to sprite sheet. Sprite will become child of sprite sheet
         * Sprite sheet will be injected to the scene for which Sprites Loader is used
         * @param File name
         * @return Sprite
         */
        cocos2d::CCSprite* loadSprite(const char *name);
        cocos2d::CCSpriteFrame* loadSpriteFrame(const char *name);
        /**
         * @brief Loads sprites which is not added to sprite sheet
         * @param File name
         * @return Sprite
         */
        cocos2d::CCSprite* loadSpriteWithoutSpriteSheet(const char * name);
        void setParent(cocos2d::CCNode*);
        void removeSprite(cocos2d::CCNode* sprite);
        void inject(bool real_inject=true);
        ~SpritesLoaderClass();
    private:
        SpritesLoaderClass(cocos2d::CCNode* parent, const char* texture_file);
        void prepareSpriteSheet();
        cocos2d::CCSpriteBatchNode* _sprite_sheet;
        bool _use_sprite_sheet;
        std::string _texture_file;
        cocos2d::CCNode* _parent;
        bool _injected;
        friend class GraphicsManager;
    };
    typedef std::auto_ptr<SpritesLoaderClass> SpritesLoader;
    /**
     * @brief Get sprites loader for a some object
     * @param Object for which sprites are loaded
     * @return SpritesLoader
     */
    static SpritesLoader getLoaderFor(cocos2d::CCNode* parent, const char *plist, const char *texture);
private:

    GraphicsManager();

};
typedef GraphicsManager::SpritesLoader SpritesLoader;
#endif // GRAPHICSMANAGER_H
