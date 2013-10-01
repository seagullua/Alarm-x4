#include "Alarm.h"
#include "System.h"
#include "AlarmServer.h"
#include "TimeGuess.h"
Alarm::Alarm()
    : _mode(NoSet), _wake_up(0,0), _go_out(0,0)
{
    bool wake_up_set = _server.isTimeSet(AlarmServer::WakeUp);
    bool go_out_set = _server.isTimeSet(AlarmServer::GoOut);

    if(wake_up_set && go_out_set)
    {
        _mode = WakeUpAndGoOut;
        _wake_up = _server.getTime(AlarmServer::WakeUp);
        _go_out = _server.getTime(AlarmServer::GoOut);
    }
    else if(wake_up_set)
    {
        _mode = WakeUp;
        _wake_up = _server.getTime(AlarmServer::WakeUp);;
    }
    else if(go_out_set)
    {
        _mode = GoOut;
        _go_out = _server.getTime(AlarmServer::GoOut);
    }

    debugCheck();
}
#include <ctime>
#include <cassert>
void Alarm::debugCheck()
{
//    //TEST
//    time_t t = time(0);
//    _server.setULong(AlarmServer::LastAlarmTimeStamp, t);

//    long t_new = _server.getULong(AlarmServer::LastAlarmTimeStamp);
//    assert(t == t_new);
}

void Alarm::setAlarm(const Time wake_up, const Time go_out)
{
    _mode = WakeUpAndGoOut;
    _wake_up = wake_up;
    _go_out = go_out;

    cancelAlarmTime();
    cancelAllDelayedActions();
    _server.setTime(AlarmServer::WakeUp, _wake_up);
    _server.setTime(AlarmServer::GoOut, _go_out);


    commitWakeUpTime();

    TimeGuess::getInstance().addValue(TimeGuess::WakeUp, wake_up);
    Time to_prepare = go_out - wake_up;
    TimeGuess::getInstance().addValue(TimeGuess::TimeToPrepare, to_prepare);
    TimeGuess::getInstance().commit();
}
void Alarm::setSnooze(const SnoozeMode m)
{
    Time snooze_time(0,5);
    if(m == ShortSnooze)
        snooze_time = Time(0,2);

    Time to_call = Time::now() + snooze_time;

    System::getDelayedActions().setActionTime(
                DelayedActions::WakeUp,
                to_call);

    System::getDelayedActions().setActionTime(
                DelayedActions::DisplayLabel,
                to_call);

    int snooze_times = _server.getSnoozeTimes();
    _server.setSnoozeTimes(snooze_times + 1);
}
unsigned int Alarm::getSnoozeTimes() const
{
    return _server.getSnoozeTimes();
}
void Alarm::commitWakeUpTime()
{
    Time launch_time = getLaunchTime(_wake_up);
    //Time prefetch_time = getPrefetchTime(launch_time);

//    System::getDelayedActions().setActionTime(
//                DelayedActions::InternetFetch,
//                prefetch_time);

    System::getDelayedActions().setActionTime(
                DelayedActions::WakeUp,
                launch_time);
    System::getDelayedActions().setActionTime(
                DelayedActions::DisplayLabel,
                _wake_up);

    _server.setSnoozeTimes(0);
}

Time substractWithRespectToNow(const Time a, const Time b)
{
    Time now = Time::now();
    Second till = (a-now).getInSeconds();

    Second substract = b.getInSeconds();

    if(till < substract)
        substract = till / 2;

    return a - Time::fromSeconds(substract);
}

//Time Alarm::getPrefetchTime(const Time launch_time)
//{
//    Time interval(0, 15);
//    return substractWithRespectToNow(launch_time, interval);
//}

Time Alarm::getLaunchTime(const Time wake_up)
{
    //Feature is frozen wake up as user wants
//    Time interval(0, 5);
//    return substractWithRespectToNow(wake_up, interval);
    return wake_up;
}

void Alarm::setAlarm(const Time wake_up)
{
    _mode = WakeUp;
    _wake_up = wake_up;
    _go_out = Time(0,0);

    cancelAlarmTime();
    cancelAllDelayedActions();
    _server.setTime(AlarmServer::WakeUp, _wake_up);

    commitWakeUpTime();

    TimeGuess::getInstance().addValue(TimeGuess::WakeUp, wake_up);
    TimeGuess::getInstance().commit();
}

Alarm::Mode Alarm::getMode() const
{
    return _mode;
}
Time Alarm::getWakeUpTime() const
{
    return _wake_up;
}
Time Alarm::getGoOutTime() const
{
    return _go_out;
}

void Alarm::cancelAlarmTime()
{
    _server.cancelTime(AlarmServer::WakeUp);
    _server.cancelTime(AlarmServer::GoOut);
}

void Alarm::cancelAlarm()
{
    _mode = NoSet;
    _server.setSnoozeTimes(0);
    cancelAlarmTime();
    cancelAllDelayedActions();
}


void Alarm::cancelAllDelayedActions()
{
    //System::getDelayedActions().cancelAction(DelayedActions::InternetFetch);
    System::getDelayedActions().cancelAction(DelayedActions::WakeUp);
    cancellAllGoOutAlarms();
}
#include "cocos2d.h"
void Alarm::onGoOut(unsigned int alarm_id)
{
    if(alarm_id == 3)
    {
        cocos2d::CCLog("Time canceled");
        //Last alarm
        if(this)
            cancelAlarm();
        else
        {
            cocos2d::CCLog("Plain cancel");
            AlarmServer server;
            server.cancelTime(AlarmServer::GoOut);
        }
    }
}

void Alarm::onWakeUpAction()
{
    _server.cancelTime(AlarmServer::WakeUp);
    //System::getDelayedActions().cancelAction(DelayedActions::InternetFetch);
    System::getDelayedActions().cancelAction(DelayedActions::WakeUp);

    cancellAllGoOutAlarms();
    if(_mode == WakeUpAndGoOut)
    {
        _mode = GoOut;
        Time to_out = _go_out - Time::now();
        Second seconds_to_out = to_out.getInSeconds();

        if(seconds_to_out >= 60 * 1)
        {
            //If go out in less then 1 minute
            //Set last alarm
            System::getDelayedActions().setActionTime(DelayedActions::GoOutThird,
                                                      _go_out - Time(0,1));
        }
        if(seconds_to_out >= 60 * 5)
        {
            System::getDelayedActions().setActionTime(DelayedActions::GoOutSecond,
                                                      _go_out - Time(0,5));
        }
        if(seconds_to_out >= 60 * 10)
        {
            System::getDelayedActions().setActionTime(DelayedActions::GoOutFirst,
                                                      _go_out - Time(0,10));
        }
    }
    else
    {
        _mode = NoSet;
    }

    _server.setULong(AlarmServer::LastAlarmTimeStamp, time(0));
    _server.setSnoozeTimes(0);
}

bool Alarm::isMorning() const
{
    uint64_t last_alarm = _server.getULong(AlarmServer::LastAlarmTimeStamp);
    uint64_t cur_time = time(0);

    uint64_t max_diff = 3600 * 6;

    if(cur_time - last_alarm <= max_diff)
        return true;
    return false;
}

void Alarm::cancellAllGoOutAlarms()
{
    System::getDelayedActions().cancelAction(DelayedActions::GoOutFirst);
    System::getDelayedActions().cancelAction(DelayedActions::GoOutSecond);
    System::getDelayedActions().cancelAction(DelayedActions::GoOutThird);
}
