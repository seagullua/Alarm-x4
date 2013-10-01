#include "main.h"
#include "../Classes/AppDelegate.h"
#include "CCEGLView.h"
#include "Logic/IntentParams.h"
#include "Logic/System.h"
#include "Logic/PrefetchAction.h"
USING_NS_CC;
enum Mode
{
    //IPhones
    IPhone3GS, //480x320
    IPhone4GS, //960x640
    IPhone5, //1136x640
    //IPad
    IPad2, //1024x768
    IPad4, //2048x1536
    //Android phones
    Galaxy_Ace2, //62% 800x480
    Galaxy_Mini2, //14% 480x320
    HTC_One_S, //6% 960x540
    Motorola_Droid, //5% 854x480
    Galaxy_Y, //5% 320x240
    //Android tablets
    Galaxy_Tab2_7, //74% 1024x600
    Galaxy_Tab2_10, //22% 1280x800
    Google_Nexus_10, //2560x1600

    Microsoft_Surface_10, //1366x768
    Microsoft_Surface_promo //603x338

};
/*int APIENTRY _tWinMain(HINSTANCE hInstance,
                       HINSTANCE hPrevInstance,
                       LPTSTR    lpCmdLine,
                       int       nCmdShow)
{*/

//#define INTERNET_PREFETCH


int CALLBACK WinMain(
        _In_  HINSTANCE,
        _In_  HINSTANCE,
        _In_  LPSTR,
        _In_  int
        )
{
#ifndef INTERNET_PREFETCH

    IntentParams::getInstance().setMode(IntentParams::WakeUpMode);
    //UNREFERENCED_PARAMETER(hPrevInstance);
    //UNREFERENCED_PARAMETER(lpCmdLine);

    Mode open_in_mode = Galaxy_Tab2_10;

    const float max_width = 1200;
    const float max_height = 600;
    bool zoom = true;

    float width = 0;
    float height = 0;
    float bottom_spacing = 0;



    switch(open_in_mode)
    {
    case IPhone3GS:
        width = 480;
        height = 320;
        break;
    case IPhone4GS:
        width = 960;
        height = 640;
        break;
    case IPhone5:
        width = 1136;
        height = 640;
        break;
    case IPad2:
        width = 1024;
        height = 768;
        break;
    case IPad4:
        width = 2048;
        height = 1536;
        break;
    case Galaxy_Ace2:
        width = 800;
        height = 480;
        break;
    case Galaxy_Mini2:
        width = 480;
        height = 320;
        break;
    case HTC_One_S:
        width = 960;
        height = 540;
        break;
    case Motorola_Droid:
        width = 854;
        height = 480;
        break;
    case Galaxy_Y:
        width = 320;
        height = 240;
        break;
    case Galaxy_Tab2_7:
        width = 1024;
        height = 600;
        bottom_spacing = 48;
        break;
    case Galaxy_Tab2_10:
        width = 1280;
        height = 800;
        bottom_spacing = 48;
        break;
    case Google_Nexus_10:
        width = 2560;
        height = 1600;
        bottom_spacing = 112;
        break;
    case Microsoft_Surface_10:
        width = 1366;
        height = 768;
        break;
    case Microsoft_Surface_promo:
        width = 603;
        height = 338;
        break;
    }




    AppDelegate app;
    CCEGLView* eglView = CCEGLView::sharedOpenGLView();
    height-=bottom_spacing;
    eglView->setFrameSize(width, height);

    float zoom_factor = 1;
    if(zoom)
    {
        float needed_zoom_factor = MIN(max_width/width, max_height/height);
        zoom_factor = MIN(zoom_factor, needed_zoom_factor);
    }
    eglView->setFrameZoomFactor(zoom_factor);
    return CCApplication::sharedApplication()->run();
#else
    PrefetchAction::performPrefetch();
#endif


}


