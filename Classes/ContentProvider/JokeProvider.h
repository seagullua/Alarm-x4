#ifndef JOKEPROVIDER_H
#define JOKEPROVIDER_H

#include "Joke.h"
#include <memory>
#include <ctime>
class JokesDelegate
{
public:
    enum Error
    {
        NetworkError,
        ParseError
    };

    virtual void onJokesRecieved(const Jokes&)
    {}
    virtual void onJokesError(const Error error,
                              const std::string& error_description)
    {
        error;
        error_description;
    }
    virtual ~JokesDelegate()
    {}
    JokesDelegate()
    {}
private:
    JokesDelegate(const JokesDelegate&);
    JokesDelegate& operator=(const JokesDelegate&);
};

typedef std::auto_ptr<JokesDelegate> JokesDelegatePtr;
typedef std::string JokeProviderID;
class JokeProvider
{
public:
    JokeProvider()
    {}
    void getJokes(JokesDelegatePtr delegate)
    {
        _last_request = time(NULL);
        do_getJokes(delegate);
    }
    const time_t getLastRequestTime() const
    {
        return _last_request;
    }
    const JokeProviderID getProviderID() const
    {
        return do_getProviderID();
    }
    virtual ~JokeProvider()
    {}
private:
    virtual void do_getJokes(JokesDelegatePtr) = 0;
    virtual JokeProviderID do_getProviderID() const = 0;
    time_t _last_request;
    JokeProvider(const JokeProvider&);
    JokeProvider& operator=(const JokeProvider&);
};

typedef std::auto_ptr<JokeProvider> JokeProviderPtr;
#endif // JOKEPROVIDER_H
