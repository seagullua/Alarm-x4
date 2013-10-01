#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "Scripts/MainScript.h"

class ScriptScene : public cocos2d::CCLayer
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init(ScriptPtr);

    // there's no 'id' in cpp, so we recommand to return the exactly class pointer
    static cocos2d::CCScene* scene(ScriptPtr);
    static ScriptScene* create(ScriptPtr);


};

#endif // __HELLOWORLD_SCENE_H__
