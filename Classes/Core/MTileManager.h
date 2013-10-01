#ifndef MTILEMANAGER_H
#define MTILEMANAGER_H
#include "MTile.h"
#include "MTileGrid.h"
#include "SpaceRequest.h"
#include <set>
#include "cocos2d.h"
#include <memory>
#include <stack>
#include "Anchor.h"

class Widget;
class Script;
class InverseAction;
typedef std::auto_ptr<Widget> WidgetPtr;
typedef std::auto_ptr<Script> ScriptPtr;
typedef std::auto_ptr<InverseAction> InverseActionPtr;

class MTileManager
{
    class MTileLayer;
    class AnimationFinalizer;
    class CreateTileWithDelay;
public:
    class Api;
    class WApi;
    class ExWApi;

    typedef Api* ApiPtr;
    typedef WApi* WApiPtr;
    typedef ExWApi* ExWApiPtr;

    typedef MTileLayer MLayer;
    typedef MTileManager::AnimationFinalizer AF;

    static MTileManager &sharedManager();


    const MTileGrid& getGrid() const;

    void runScript(ScriptPtr script);

    void switchToScene(cocos2d::CCNode *parent);
    void runAction(cocos2d::CCAction* action);
    void cancelAction(cocos2d::CCAction* action);
    void displayGrid(bool to_display);
private:


    MTileManager();
    ~MTileManager();


    cocos2d::CCNode* _actioner;
    MTileLayer* _main_layer;
    MTileGrid _grid;
    cocos2d::CCSprite* _grid_sprite;
    ScriptPtr _active_script;

    bool _grid_shown;
    static MTileManager* _manager;

    //Widget and all children
    typedef std::set<Widget*> WidgetSet;
    typedef std::map<Widget*, WidgetSet > WidgetMap;
    typedef std::set<MTile*> MTilesSet;
    typedef std::map<Widget*, MTilesSet> WidgetsTiles;

    WidgetMap _widgets;
    WidgetsTiles _widgets_tiles;

    WidgetSet _widgets_waiting_to_appear;
    void removeAllTilesOfWidget(Widget*);
    void removeAllChildWidget(const WidgetSet&);
    void removeAllWidgets();



};

class MTileManager::WApi
{
public:
    static const float NO_DELAY;
    static const float NORMAL_DELAY;
    Widget* addWidget(WidgetPtr widget, float delay=NORMAL_DELAY);
    void hideWidget(Widget* widget);
    void showWidget(Widget* widget);
    void addBackBarrier();
    void addBackCallback(cocos2d::CCObject*, cocos2d::SEL_CallFunc method);
    void noBackSectionStart();
    void noBackSectionEnd();

    void runScript(ScriptPtr script);
private:
    WApi(const WApi&);
    WApi& operator=(const WApi&);


    WApi(Script*);
    friend class MTileManager;
    bool _auto_back;


protected:
    void addInverseAction(InverseActionPtr action);
    virtual ~WApi();
    std::stack<InverseAction*> _back_stack;
    Script* _script;

};

class MTileManager::ExWApi : public WApi
{
public:
    void removeWidget(Widget* widget);
    void goBack();
    void addInverseAction(InverseActionPtr action);
private:
    ExWApi(Script*);
    ExWApi(const ExWApi&);
    ExWApi& operator=(const ExWApi&);
    ~ExWApi();

    friend class MTileManager;
    friend class Script;
};

class MTileManager::Api
{
public:




    void addTile(MTile* tile);
    void replaceTile(MTile* old_tile, MTile* new_tile);
    void moveTile(MTile*, const MTilePosition& );
    void resizeTile(MTile*, const MTileSize& );
    void removeTile(MTile*);
    void addWidget(WidgetPtr);

    bool reserveSpace(const MTileBlock&);

    MTileBlock reserveSpace(const Anchor::Horizontal horizontal,
                            const Anchor::Vertical vertical,
                            MTileSize max_size,
                            MTileSize min_size=MTileSize(0,0));
    MTileBlock reserveSpace(const SpaceRequest&);

    void cancelReservation();
private:
    Api(Widget*);
    Api(const Api&);
    Api& operator=(const Api&);
    ~Api();
    friend class MTileManager;
    friend class Widget;
    Widget* _parent;
};

#endif // MTILEMANAGER_H
