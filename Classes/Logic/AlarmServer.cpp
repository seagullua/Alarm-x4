#include "AlarmServer.h"
#include "System.h"
AlarmServer::AlarmServer()
{
}
static const int TIME_SUFFIX = 1100;
static const int LONG_SUFFIX = 1200;
static const int SAVE_INT = 1300;

int keyToKey(const AlarmServer::Key key, const int suffix)
{
    return key*10000 + suffix;
}



bool AlarmServer::isTimeSet(const Key k) const
{
    return System::getValueStorage().get(keyToKey(k, TIME_SUFFIX)) == 1;
}



void AlarmServer::cancelTime(const Key k)
{
    System::getValueStorage().set(keyToKey(k, TIME_SUFFIX), 0);
}

void AlarmServer::setTime(const Key k, const Time t)
{
    ValueStorage& st = System::getValueStorage();
    int id = keyToKey(k, TIME_SUFFIX);

    st;
    st.set(id, 1);
    st.set(id+1, t.getHour());
    st.set(id+2, t.getMinute());
}


const Time AlarmServer::getTime(const Key k) const
{
    Time res(0,0);
    if(isTimeSet(k))
    {
        ValueStorage& s = System::getValueStorage();
        s;
        unsigned int hours = s.get(keyToKey(k,TIME_SUFFIX)+1);
        unsigned int minutes = s.get(keyToKey(k,TIME_SUFFIX)+2);

        res = Time(hours, minutes);
    }
    return res;
}

void AlarmServer::setULong(const Key key, const uint64_t ul)
{
    uint64_t max_int = 0xFFFFFFFFL;
    uint32_t a = (ul & max_int);
    uint32_t b = (ul >> 32);
    int k = keyToKey(key, LONG_SUFFIX);

    ValueStorage& st = System::getValueStorage();
    st;
    st.set(k, a);
    st.set(k+1, b);
}

uint64_t AlarmServer::getULong(const Key key) const
{
    ValueStorage& st = System::getValueStorage();
    st;

    int k = keyToKey(key, LONG_SUFFIX);
    uint32_t a = st.get(k);
    uint32_t b = st.get(k+1);

    return (uint64_t(b) << 32) + a;
}

void AlarmServer::setSnoozeTimes(const int times)
{
    ValueStorage& st = System::getValueStorage();
    st;

    int k = keyToKey(SnoozeTimes, SAVE_INT);
    st.set(k, times);
}

int AlarmServer::getSnoozeTimes() const
{
    ValueStorage& st = System::getValueStorage();
    st;

    int k = keyToKey(SnoozeTimes, SAVE_INT);
    return st.get(k);
}
