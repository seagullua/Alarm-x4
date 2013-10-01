#include "PrefetchAction.h"
#include "AppDelegate.h"
#include "ContentProvider/Weather/WeatherHolder.h"
#include "ContentProvider/JokesHolder.h"
#include "ContentProvider/LTaskManager.h"
using namespace Weather;
using namespace cocos2d;
PrefetchAction::PrefetchAction()
    : _waiting_delegates(0)
{
}
void PrefetchAction::performPrefetch()
{
    static PrefetchAction action;
    action.do_performPrefetch();
}

class PrefetchAction::WeatherDelegate : public WeatherHolder::Delegate
{
public:
    WeatherDelegate(PrefetchAction*)
        //: _parent(parent)
    {}

    void onForecastArrived(const Forecast & )
    {
        CCLog("Forecast prefetch finish");
       // _parent->delegateFinish();
    }

private:
    //PrefetchAction* _parent;
};

class PrefetchAction::JokesUpdate : public JokesUpdateDelegate
{
public:
    JokesUpdate(PrefetchAction*)
      //  : _parent(parent)
    {}

    void onUpdateFinished()
    {
        CCLog("Jokes prefetch finish");
        //_parent->delegateFinish();
    }

private:
    //PrefetchAction* _parent;
};
void PrefetchAction::do_performPrefetch()
{
//    bool is_runing = AppDelegate::isRunning();
//    AppDelegate* app = 0;
//    if(!is_runing)
//    {
//        app = new AppDelegate;
//        CCLog("new App created");
//    }
//    else
//    {
//        CCLog("Stand by mode");
//    }
//    AppDelegate::configureContentProviders();

    //Load weather
    _waiting_delegates++;
    WeatherHolder::getInstance().getForecast(
                WeatherHolder::DelegatePtr(new WeatherDelegate(this)));

    //Jokes prefetch
    _waiting_delegates++;
    JokesHolder::sharedHolder().setUpdateDelegate(
                JokesUpdateDelegatePtr(new JokesUpdate(this)));
    JokesHolder::sharedHolder().updateJokes();

//    if(!is_runing)
//    {
//        while(_waiting_delegates > 0)
//        {
//            CCDirector::sharedDirector()->getScheduler()->update(0.2f);
//            LTaskManager::sharedManager().updateEvent(0.2f);
//            //CCLog("Sleep 200");
//            sleep(0.2f);

//        }
//    }
//    CCLog("Purge file utilits");
//    CCFileUtils::sharedFileUtils()->purgeFileUtils();
//    CCLog("Delete app");
//    if(app)
//        delete app;
}

void PrefetchAction::delegateFinish()
{
    if(_waiting_delegates > 0)
    {
        _waiting_delegates--;
    }
}


