#ifndef MAINSCRIPT_H
#define MAINSCRIPT_H
#include "Core/Script.h"
#include "Logic/Time.h"
class MainScript : public Script
{
public:
    MainScript();
private:
    void onInit();

    void showMorningStuff();
    void hideMorningStuff();
    void step1_AlarmMenu();
    void step1_TurnOffAlarm();

    void step2_WakeUpSettings();
    void step2_GoOutSettings();

    void step3_OnWakeUpTimeChoosen(const Time);
    void step3_OnWakeUpAndGoOutTimeChoosen(const Time, const Time);
    void step3_OnCancel();

    void stepX_startFlow();

    void onLastBack();
    Widget* _menu_select;
    Widget* _alarm_menu;
    Widget* _clock_widget;
    Widget* _jokes_widget;
    Widget* _start_flow_widget;
    Widget* _weather_widget;

    Widget* _alarm_time_chooser;
    class AlarmSetCallback;
    class FlowDelegate;

};

#endif // MAINSCRIPT_H
