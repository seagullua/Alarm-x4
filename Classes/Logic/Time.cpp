#include "Time.h"
#include <ctime>

Time Time::now()
{
    time_t cur_time = time(0);
    tm* time = localtime(&cur_time);

    return Time(time->tm_hour, time->tm_min);
}
