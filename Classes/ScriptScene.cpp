#include "ScriptScene.h"
#include "Core/MTileManager.h"


USING_NS_CC;


ScriptScene* ScriptScene::create(ScriptPtr script)
{
    ScriptScene *pRet = new ScriptScene();
    if (pRet && pRet->init(script))
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}


cocos2d::CCScene *ScriptScene::scene(ScriptPtr script)
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();

    // 'layer' is an autorelease object
    ScriptScene *layer = ScriptScene::create(script);

    // add layer as a child to scene
    scene->addChild(layer);

    MTileManager::sharedManager().switchToScene(scene);
    // return the scene
    return scene;
}


bool ScriptScene::init(ScriptPtr script)
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }

    MTileManager::sharedManager().runScript(script);

    return true;
}

