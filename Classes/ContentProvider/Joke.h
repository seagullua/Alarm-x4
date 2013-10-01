#ifndef JOKE_H
#define JOKE_H
#include <string>
#include <vector>
#include <ctime>
class Joke
{
public:
    Joke(const std::string& joke_text="")
        : _joke_text(joke_text), _times_showed(0), _recieve_time(0)
    {
    }
    ~Joke()
    {}
    Joke(const Joke& joke)
        : _joke_text(joke._joke_text), _times_showed(joke._times_showed),
          _recieve_time(joke._recieve_time)
    {}
    Joke& operator=(const Joke& joke)
    {
        _joke_text = joke._joke_text;
        _times_showed = joke._times_showed;
        _recieve_time = joke._recieve_time;
        return *this;
    }

    const std::string& getJokeText() const
    {
        return _joke_text;
    }
    void setJokeText(const std::string& str)
    {
        _joke_text = str;
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
            _times_showed = 1;

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
    std::string _joke_text;
    time_t _recieve_time;
    unsigned int _times_showed;
};

typedef std::vector<Joke> Jokes;
#endif // JOKE_H
