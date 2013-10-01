#ifndef TIME_H
#define TIME_H

typedef unsigned int Hour;
typedef unsigned int Minute;
typedef long Second;

class Time
{
public:
    Time(const Hour h=0, const Minute m=0)
        : _hour(h), _minute(m)
    {}

    const Hour getHour() const
    {
        return _hour;
    }

    const Minute getMinute() const
    {
        return _minute;
    }

    const Second getInSeconds() const
    {
        return (_hour * 60 + _minute) * 60;
    }

    static Time fromSeconds(const Second seconds)
    {
        Hour h = seconds / 3600;
        Minute m = (seconds % 3600) / 60;
        return Time(h,m);
    }

    static Time now();
private:
    Hour _hour;
    Minute _minute;
};
inline bool operator==(const Time& a, const Time& b)
{
    return a.getHour() == b.getHour() && a.getMinute() == b.getMinute();
}

inline Time operator-(const Time& a, const Time& b)
{
    Second a_s = a.getInSeconds();
    Second b_s = b.getInSeconds();

    if(a_s < b_s)
        a_s += 3600 * 24;
    return Time::fromSeconds(a_s - b_s);
}

inline Time operator+(const Time& a, const Time& b)
{
    Second a_s = a.getInSeconds();
    Second b_s = b.getInSeconds();

    Second sum = (a_s + b_s) % (3600 * 24);
    return Time::fromSeconds(sum);
}

#endif // TIME_H
