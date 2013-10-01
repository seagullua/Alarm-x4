#ifndef WAKEUPSCRIPT_H
#define WAKEUPSCRIPT_H
#include "Core/Script.h"
#include "Logic/Time.h"
#include "Logic/Alarm.h"
class FlowWidget;
class WakeUpScript : public Script
{
public:
    WakeUpScript();
private:
    class FlowDelegate;
    void onInit();

    void setQuickSnoozeBackBarrier();
    void quickSnooze();

    void step1_snooze();
    void step1_turnOffAlarm();

    void step2_final();


    void autoSnooze_delegate(float);

    void turnOffAutoSnooze();
    void performSnooze(const Alarm::SnoozeMode );
    void turnOffAlarm();

    //FlowWidget* _flow;
    Widget* _flow_helper;
    Widget* _main_widget;
    cocos2d::CCAction* _delayed_snooze;
};

#endif // WAKEUPSCRIPT_H
