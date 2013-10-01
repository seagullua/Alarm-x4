#include "AppDelegate.h"
#include "ScriptScene.h"
#include "AppMacros.h"
#include "Logic/IntentParams.h"
#include "Scripts/WakeUpScript.h"
#include "Logic/System.h"
#include "SimpleAudioEngine.h"
#include "ContentProvider/JokesHolder.h"
#include "ContentProvider/VKRssJokeProvider.h"
#include "ContentProvider/Weather/WeatherCoUaProvider.h"
#include "ContentProvider/Weather/WeatherHolder.h"
#include "Logic/PrefetchAction.h"
#include "Logic/Translations.h"
#include <cstdlib>
#include <ctime>
USING_NS_CC;
using namespace CocosDenshion;
AppDelegate::AppDelegate()

{
    _is_running = false;
}

AppDelegate::~AppDelegate() 
{
}

bool AppDelegate::_is_running = false;

bool AppDelegate::applicationDidFinishLaunching() {

    CCLog("Finished launching");
    srand(time(0));
    System::_ld.init();

    // initialize director
    CCDirector* pDirector = CCDirector::sharedDirector();
    CCEGLView* pEGLView = CCEGLView::sharedOpenGLView();

    pDirector->setOpenGLView(pEGLView);

    // Set the design resolution
    pEGLView->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, kResolutionNoBorder);

    CCSize frameSize = pEGLView->getFrameSize();

    // In this demo, we select resource according to the frame's height.
    // If the resource size is different from design resolution size, you need to set contentScaleFactor.
    // We use the ratio of resource's height to the height of design resolution,
    // this can make sure that the resource's height could fit for the height of design resolution.
    CCSize vsize = pEGLView->getVisibleSize();
    // if the frame's height is larger than the height of medium resource size, select large resource.
    float width_change = vsize.width / designResolutionSize.width;
    // if the frame's height is larger than the height of medium resource size, select large resource.
    CCFileUtils::sharedFileUtils()->setResourceDirectory(mediumResource.directory);
    pDirector->setContentScaleFactor(MIN(mediumResource.size.height/designResolutionSize.height/width_change, mediumResource.size.width/designResolutionSize.width/width_change));


    // turn on display FPS
    pDirector->setDisplayStats(false);

    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);

    // create a scene. it's an autorelease object
    ScriptPtr main_script(0);
    IntentParams::Mode mode = IntentParams::getInstance().getMode();

    if(mode == IntentParams::WakeUpMode)
        main_script = ScriptPtr(new WakeUpScript);
    else
        main_script = ScriptPtr(new MainScript);

    loadTranslation(getDeviceLang());
    configureContentProviders();
    //
    //JokesHolder::sharedHolder().updateJokes();




    CCScene *pScene = ScriptScene::scene(main_script);
    // run
    pDirector->runWithScene(pScene);

    _is_running = true;


    CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("choose.wav");
    return true;
}

void AppDelegate::configureContentProviders()
{
    CCLog("Jokes load");
    VKRssJokeProvider* feed = 0;
    Weather::City default_city = Weather::City("New York", 74223);
    if(R::Language::getLangCode()=="uk")
    {
        feed = new VKRssJokeProvider("http://vk.com/humor_ukraine");
        default_city = Weather::City("Київ", 23);
    }
    else if(R::Language::getLangCode()=="ru")
    {
        feed = new VKRssJokeProvider("http://vk.com/megaotriv");
        default_city = Weather::City("Киев", 23);
    }
    else
        feed = new VKRssJokeProvider("http://feeds.sydv.net/latest-bash-quotes", false);

    JokesHolder::sharedHolder().addJokeProvider(
                JokeProviderPtr(feed));

    CCLog("Weather load");
    Weather::WeatherHolder::getInstance().setProvider(
                Weather::WeatherProviderPtr(new Weather::WeatherCoUaProvider()));
    Weather::WeatherHolder::getInstance().setDefaultCity(default_city);
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    CCDirector::sharedDirector()->stopAnimation();
#ifndef WIN32
    //CCDirector::sharedDirector()->end();
#endif
    // if you use SimpleAudioEngine, it must be pause

    CCLog("hide");
    if(IntentParams::getInstance().getMode() == IntentParams::WakeUpMode)
    {
        CCLog("snooze");
        System::getAlarm().setSnooze(Alarm::ShortSnooze);
        SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
        //CCDirector::sharedDirector()->end();
    }

}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    CCDirector::sharedDirector()->startAnimation();
    CCLog("show");
    // if you use SimpleAudioEngine, it must resume here
    //SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}
