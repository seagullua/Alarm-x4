#include "TimeGuess.h"
#include "Core/Binary.h"
#include "Core/file_exists.h"
#include "cocos2d.h"
using namespace cocos2d;
TimeGuess::TimeGuess()
    : _current_day(0)
{
    std::string wpath = CCFileUtils::sharedFileUtils()->getWriteablePath();
    _file_path = wpath + "tguess.ad";
    loadFromFile();

    time_t cur_time = time(0);
    tm* tm_val = localtime(&cur_time);

    _current_day = tm_val->tm_wday;
}


OutputBinaryStream& operator<<(OutputBinaryStream& os, const Time& t)
{
    os << uint32_t(t.getHour()) << uint32_t(t.getMinute());
    return os;
}

InputBinaryStream& operator>>(InputBinaryStream& is, Time& t)
{
    uint32_t hour = 0;
    uint32_t minute = 0;

    is >> hour >> minute;

    t = Time(hour, minute);
    return is;
}

OutputBinaryStream& operator<<(OutputBinaryStream& os, const TimeGuess::TD& td)
{
    os << uint16_t(td._day) << td._time;
    return os;
}

InputBinaryStream& operator>>(InputBinaryStream& is, TimeGuess::TD& td)
{
    uint16_t day=0;
    is >> day >> td._time;
    td._day = day;
    return is;
}



void TimeGuess::loadFromFile()
{
    if(fileExists(_file_path.c_str()))
    {
        std::ifstream iss(_file_path.c_str(), std::ios::in | std::ios::binary);
        InputBinaryStream is(iss);

        uint32_t size = 0;
        is >> size;

        for(uint32_t i=0; i<size; ++i)
        {
            uint32_t key = 0;
            TimeVec vec;
            is >> key >> vec;
            _map[static_cast<Key>(key)]=vec;
        }
    }
}

void TimeGuess::saveToFile()
{
    std::ofstream oss(_file_path.c_str(), std::ios::out | std::ios::binary);
    OutputBinaryStream os(oss,BinaryStream::MaxProtocolVersion);

    os << _map.size();
    for(Map::iterator i=_map.begin(); i!=_map.end(); ++i)
    {
        os << uint32_t(i->first) << i->second;
    }
}

void TimeGuess::commit()
{
    saveToFile();
}

TimeGuess& TimeGuess::getInstance()
{
    if(_obj == 0)
        _obj = new TimeGuess();
    return *_obj;
}

TimeGuess* TimeGuess::_obj=0;

void TimeGuess::addValue(const Key key, const Time& t)
{
    TD td;
    td._day = _current_day;
    td._time = t;
    _map[key].push_back(td);
}

const Time TimeGuess::getValue(const Key key, const Time &default_time) const
{
    Map::const_iterator it = _map.find(key);
    if(it == _map.end())
        return default_time;

    const TimeVec& current = it->second;
    if(current.size() == 0)
        return default_time;
    if(current.size() == 1)
        return current[0]._time;

    TimeVec today;
    unsigned int cur_size = current.size();
    for(unsigned int i=0; i<cur_size; ++i)
    {
        TD td = current[cur_size-i-1];
        if(td._day == _current_day)
            today.push_back(td);
        if(today.size() >= 2)
            break;
    }

    if(today.size() == 0)
    {
        //Today first time
        return timeFromTo(current[cur_size-1], current[cur_size-2]);
    }
    else if(today.size() == 1)
    {
        return today[0]._time;
    }
    return timeFromTo(today[0], today[1]);

}

Time TimeGuess::timeFromTo(const TD& new_td, const TD& old_td) const
{
    if(new_td._time == old_td._time)
        return new_td._time;
    return old_td._time;
}
