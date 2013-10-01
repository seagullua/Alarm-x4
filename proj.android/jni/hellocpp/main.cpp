#include "AppDelegate.h"
#include "platform/android/jni/JniHelper.h"
#include <jni.h>
#include <android/log.h>
#include "Logic/IntentParams.h"
#include "Logic/System.h"
#include "Logic/Location.h"
#include "Logic/PrefetchAction.h"
#include "Extension/Platform.h"
#include "Logic/Translations.h"
//#include "HelloWorldScene.h"

#define  LOG_TAG    "main"
#ifndef NDEBUG
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
#else
#define LOGD(...) /**/
#endif
using namespace cocos2d;

extern "C"
{

jint JNI_OnLoad(JavaVM *vm, void *reserved)
{
#ifndef NDEBUG
	LOGD("VM loaded");
#endif
    JniHelper::setJavaVM(vm);

    return JNI_VERSION_1_4;
}

void Java_org_cocos2dx_lib_Cocos2dxRenderer_nativeInit(JNIEnv*  env, jobject thiz, jint w, jint h)
{
#ifndef NDEBUG
	LOGD("Native init");
#endif
    if (!CCDirector::sharedDirector()->getOpenGLView())
    {
        CCEGLView *view = CCEGLView::sharedOpenGLView();
        view->setFrameSize(w, h);

        AppDelegate *pAppDelegate = new AppDelegate();
        CCApplication::sharedApplication()->run();
    }
    else
    {
        ccDrawInit();
        ccGLInvalidateStateCache();
        
        CCShaderCache::sharedShaderCache()->reloadDefaultShaders();
        CCTextureCache::reloadAllTextures();
        CCNotificationCenter::sharedNotificationCenter()->postNotification(EVNET_COME_TO_FOREGROUND, NULL);
        CCDirector::sharedDirector()->setGLDefaultValues(); 
    }
}

void Java_ua_in_chaika_Alarm_IntentParams_setMode(JNIEnv*  env, jobject thiz, jint param)
{
	IntentParams::getInstance().setMode(static_cast<IntentParams::Mode>(param));
}

void Java_ua_in_chaika_Alarm_Planer_internetPrefetchAction(JNIEnv*  env, jobject thiz)
{
#ifndef NDEBUG
	LOGD("Internet prefetch started");
#endif
	PrefetchAction::performPrefetch();
	LOGD("Internet prefetch done");
	//IntentParams::getInstance().setMode(static_cast<IntentParams::Mode>(param));
}

void Java_ua_in_chaika_Alarm_Planer_goOutAlarmTurnOff(JNIEnv*  env, jobject thiz)
{
	System::getAlarm().onGoOut(3);
}

int ValueStorage::get(const Key k)
{
	LOGD("get called");
	JniMethodInfo methodInfo;
	//jclass cl = JniHelper::getClassID("ua/in/chaika/Alarm/VStorage");
	if (! JniHelper::getStaticMethodInfo(methodInfo, "ua/in/chaika/Alarm/VStorage", "getInt", "(I)I"))
	{
		return 0;
	}
	
	jint res = methodInfo.env->CallStaticIntMethod(methodInfo.classID, methodInfo.methodID, k);
	methodInfo.env->DeleteLocalRef(methodInfo.classID);
	return res;
}

void ValueStorage::set(const Key k, const int value)
{

	JniMethodInfo methodInfo;
	if (! JniHelper::getStaticMethodInfo(methodInfo, "ua/in/chaika/Alarm/VStorage", "setInt", "(II)V"))
	{
		return;
	}

	methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, k, value);
	methodInfo.env->DeleteLocalRef(methodInfo.classID);
}

void DelayedActions::setActionTime(const Action a, const Time t)
{
	JniMethodInfo methodInfo;
	LOGD("set alarm called");
	if (! JniHelper::getStaticMethodInfo(methodInfo, "ua/in/chaika/Alarm/Planer", "setAlarm", "(III)V"))
	{
		return;
	}
	LOGD("-method found");
	methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, a, t.getHour(), t.getMinute());
	methodInfo.env->DeleteLocalRef(methodInfo.classID);
}
void DelayedActions::cancelAction(const Action a)
{
	JniMethodInfo methodInfo;
	if (! JniHelper::getStaticMethodInfo(methodInfo, "ua/in/chaika/Alarm/Planer", "cancelAlarm", "(I)V"))
	{
		return;
	}

	methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, a);
	methodInfo.env->DeleteLocalRef(methodInfo.classID);
}

double Location::getLongitude()
{
	JniMethodInfo methodInfo;
	if (! JniHelper::getStaticMethodInfo(methodInfo, "ua/in/chaika/Alarm/Locator", "getStaticLongitude", "()D"))
	{
		return 0;
	}

	jdouble x = methodInfo.env->CallStaticDoubleMethod(methodInfo.classID, methodInfo.methodID);
	methodInfo.env->DeleteLocalRef(methodInfo.classID);

    return x;
}

double Location::getLatitude()
{
    JniMethodInfo methodInfo;
	if (! JniHelper::getStaticMethodInfo(methodInfo, "ua/in/chaika/Alarm/Locator", "getStaticLatitude", "()D"))
	{
		return 0;
	}

	jdouble x = methodInfo.env->CallStaticDoubleMethod(methodInfo.classID, methodInfo.methodID);
	methodInfo.env->DeleteLocalRef(methodInfo.classID);

    return x;
}

void MusicBox::setMaxVolume()
{
}

void MusicBox::restoreVolume()
{
}



}

R::LangCode getDeviceLang()
{
	JniMethodInfo methodInfo;
	if (! JniHelper::getStaticMethodInfo(methodInfo, "ua/in/chaika/Alarm/IntentParams", "getLanguage", "()I"))
	{
		LOGD("Lang error");
		return R::EN;
	}

	jint x = methodInfo.env->CallStaticIntMethod(methodInfo.classID, methodInfo.methodID);
	methodInfo.env->DeleteLocalRef(methodInfo.classID);

	LOGD("Lang recieved %d", x);
    return static_cast<R::LangCode>(x);
}
