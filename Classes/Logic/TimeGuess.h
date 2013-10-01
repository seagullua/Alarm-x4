#ifndef TIMEGUESS_H
#define TIMEGUESS_H

#include "Time.h"
#include <map>
#include <vector>
#include <string>

class TimeGuess
{
public:
    enum Key
    {
        WakeUp,
        TimeToPrepare
    };

    void addValue(const Key key, const Time& t);
    const Time getValue(const Key key, const Time& default_time) const;

    void commit();

    static TimeGuess& getInstance();

    struct TD
    {
        Time _time;
        unsigned int _day;
    };
private:


    TimeGuess();
    typedef std::vector<TD> TimeVec;
    typedef std::map<Key, TimeVec> Map;

    Map _map;
    void loadFromFile();
    void saveToFile();

    Time timeFromTo(const TD& new_td, const TD& old_td) const;

    std::string _file_path;
    unsigned int _current_day;

    static TimeGuess* _obj;

};

#endif // TIMEGUESS_H
