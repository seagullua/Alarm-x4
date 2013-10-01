#ifndef ALARMSERVER_H
#define ALARMSERVER_H
#include "Time.h"
#include <stdint.h>
class AlarmServer
{
public:
    enum Key
    {
        WakeUp,
        GoOut,
        LastAlarmTimeStamp,
        SnoozeTimes
    };

    bool isTimeSet(const Key) const;
    const Time getTime(const Key) const;
    void cancelTime(const Key);
    void setTime(const Key, const Time);

    void setULong(const Key, const uint64_t l);
    uint64_t getULong(const Key) const ;

    void setSnoozeTimes(const int times);
    int getSnoozeTimes() const;
private:
    friend class Alarm;
    AlarmServer();
};

#endif // ALARMSERVER_H
