#ifndef ALARM_H
#define ALARM_H
#include "Time.h"
#include "AlarmServer.h"
class Alarm
{
public:
    enum Mode
    {
        NoSet,
        WakeUp,
        WakeUpAndGoOut,
        GoOut
    };
    enum SnoozeMode
    {
        ShortSnooze,
        LongSnooze
    };


    void setAlarm(const Time wake_up, const Time go_out);
    void setAlarm(const Time wake_up);

    void setSnooze(const SnoozeMode);

    Mode getMode() const;
    Time getWakeUpTime() const;
    Time getGoOutTime() const;

    void cancelAlarm();
    void onWakeUpAction();
    void onGoOut(unsigned int alarm_id);

    bool isMorning() const;
    unsigned int getSnoozeTimes() const;
private:
    friend class System;
    Alarm();
    void debugCheck();
    void cancelAllDelayedActions();
    void cancelAlarmTime();
    void commitWakeUpTime();

    void cancellAllGoOutAlarms();
    //Time getPrefetchTime(const Time launch_time);
    Time getLaunchTime(const Time wake_up);


    Mode _mode;
    Time _wake_up;
    Time _go_out;

    AlarmServer _server;

};

#endif // ALARM_H
