#ifndef WALLPAPER_H
#define WALLPAPER_H
#include <ctime>
#include <string>
#include <vector>

class Wallpaper
{
public:
    Wallpaper(const std::string& file_name = "")
        : _file_name(file_name), _times_showed(0), _recieve_time(0)
    {

    }
    const std::string& getFileName() const
    {
        return _file_name;
    }

    void setFileName(const std::string& file_name)
    {
        _file_name = file_name;
    }

    const time_t getRecieveTime() const
    {
        return _recieve_time;
    }
    const unsigned int getTimesShowed() const
    {
        return _times_showed;
    }
    void incrementTimesShowed()
    {
        if(_times_showed == 0)
            _times_showed++;
    }
    void setTimesShowed(const unsigned int times)
    {
        _times_showed = times;
        if(_times_showed > 1)
            _times_showed = 1;
    }

    void setRecieveTime(const time_t& time)
    {
        _recieve_time = time;
    }

private:
    unsigned int _times_showed;
    std::string _file_name;
    time_t _recieve_time;
};

typedef std::vector<Wallpaper> Wallpapers;
#endif // WALLPAPER_H
