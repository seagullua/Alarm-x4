#include "IntentParams.h"
#include "AppDelegate.h"
#include "Core/Script.h"
#include "Scripts/WakeUpScript.h"
#include "ScriptScene.h"
using namespace cocos2d;
IntentParams::IntentParams()
    : _mode(LaunchMode)
{
}

IntentParams& IntentParams::getInstance()
{
    return _obj;
}
void IntentParams::justSetMode(const Mode m)
{
    _mode = m;
}

void IntentParams::setMode(const Mode m)
{
    bool reload_scene = false;
    if(_mode != m || m == IntentParams::WakeUpMode)
    {
        reload_scene = true;
    }
    _mode = m;

    if(reload_scene && AppDelegate::isRunning())
    {
        ScriptPtr main_script(0);
        Mode mode = _mode;

        if(mode == IntentParams::WakeUpMode)
            main_script = ScriptPtr(new WakeUpScript);
        else
            main_script = ScriptPtr(new MainScript);

        CCScene *pScene = ScriptScene::scene(main_script);
        CCDirector::sharedDirector()->replaceScene(pScene);
    }
}

IntentParams::Mode IntentParams::getMode() const
{
    return _mode;
}

IntentParams IntentParams::_obj;
