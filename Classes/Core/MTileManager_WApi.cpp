#define MTILE_IMPL
#include "MTileManager.h"
#include "Widget.h"
#include "impl/InverseAction.h"
#include "impl/MTileManager_MTileLayer.h"
#include "ScriptScene.h"
using namespace cocos2d;

class DelayedScriptRunner : public CCObject
{
public:
    DelayedScriptRunner(ScriptPtr script)
        : _script(script)
    {
        this->retain();
    }

    void runScript()
    {
        CCDirector::sharedDirector()->replaceScene(ScriptScene::scene(_script));
        this->release();
    }

private:
    ScriptPtr _script;
};

MTileManager::WApi::WApi(Script* script)
    : _script(script), _auto_back(true)
{

}

MTileManager::WApi::~WApi()
{}

void MTileManager::WApi::noBackSectionStart()
{
    _auto_back = false;
}

void MTileManager::WApi::noBackSectionEnd()
{
    _auto_back = true;
}

Widget *MTileManager::WApi::addWidget(WidgetPtr widget, float delay)
{
    MTileManager& m = MTileManager::sharedManager();
    Widget* ptr = widget.release();
    WidgetMap::iterator it = m._widgets.find(ptr);
    if(it == m._widgets.end())
    {
        //New value
        m._widgets[ptr] = WidgetSet();
    }

    m._main_layer->delayedWidgetInitialize(ptr,delay);
    m._widgets_waiting_to_appear.insert(ptr);


    return ptr;

}

const float MTileManager::WApi::NO_DELAY = 0.0001f;
const float MTileManager::WApi::NORMAL_DELAY = 0.2f;

void MTileManager::WApi::hideWidget(Widget* widget)
{
    MTileManager& m = MTileManager::sharedManager();
    WidgetMap::iterator it = m._widgets.find(widget);
    if(it != m._widgets.end())
    {
        m.removeAllChildWidget(it->second);
        it->second.clear();

        if(!widget->isHidden())
        {
            widget->suspend();
            m.removeAllTilesOfWidget(widget);
            widget->getApi()->cancelReservation();
            if(_auto_back)
                addInverseAction(InverseActionPtr(new ShowWidget(widget)));
        }
    }
}

void MTileManager::WApi::runScript(ScriptPtr script)
{
//    DelayedScriptRunner* runner = new DelayedScriptRunner(script);
//    runner->autorelease();

//    runner->retain();

//    MTileManager::sharedManager()._main_layer->runAction(
//                CCSequence::createWithTwoActions(
//                    CCDelayTime::create(0.3f),
//                    CCCallFunc::create(runner,
//                                       callfunc_selector(DelayedScriptRunner::runScript))));

    CCDirector::sharedDirector()->replaceScene(ScriptScene::scene(script));

}

void MTileManager::WApi::showWidget(Widget* widget)
{
    MTileManager& m = MTileManager::sharedManager();
    WidgetMap::iterator it = m._widgets.find(widget);
    if(it != m._widgets.end())
    {
        if(widget->isHidden() && widget->isInitialized())
        {
            widget->awake();
            if(_auto_back)
                addInverseAction(InverseActionPtr(new HideWidget(widget)));
        }
    }
}



void MTileManager::WApi::addBackBarrier()
{
    if(_auto_back)
        addInverseAction(InverseActionPtr(new BackBarrier));
}

void MTileManager::WApi::addBackCallback(cocos2d::CCObject* obj, cocos2d::SEL_CallFunc method)
{
    if(_auto_back)
        addInverseAction(InverseActionPtr(new BackCallback(obj, method)));
}

void MTileManager::ExWApi::removeWidget(Widget *widget)
{
    MTileManager& m = MTileManager::sharedManager();
    WidgetMap::iterator it = m._widgets.find(widget);
    if(it != m._widgets.end())
    {
        m.removeAllChildWidget(it->second);
        m._widgets.erase(it);
        if(!widget->isHidden())
        {
            widget->suspend();
            m.removeAllTilesOfWidget(widget);
            widget->getApi()->cancelReservation();
        }

        widget->autorelease();
        //delete widget;
    }
}

void MTileManager::ExWApi::goBack()
{

    bool back_found = false;
    while(!_back_stack.empty() && !back_found)
    {
        InverseAction* act = _back_stack.top();
        _back_stack.pop();
        if(act->runAction(this))
        {
            //Back stop
            back_found = true;
        }
        delete act;
    }

}

void MTileManager::WApi::addInverseAction(InverseActionPtr action)
{
    if(_auto_back)
    {
        InverseAction* act = action.release();
        _back_stack.push(act);
    }
}

void MTileManager::ExWApi::addInverseAction(InverseActionPtr action)
{
    WApi::addInverseAction(action);
}

MTileManager::ExWApi::ExWApi(Script* script)
    : WApi(script)
{}

MTileManager::ExWApi::~ExWApi()
{}

