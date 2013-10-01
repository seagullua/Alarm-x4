#include "JokesHolder.h"
#include "Core/Binary.h"
#include "cocos2d.h"
#include <algorithm>
#include "Core/file_exists.h"
using namespace cocos2d;

std::string JokesHolder::_jokes_file_name;
std::string JokesHolder::_providers_file_name;
JokesHolder* JokesHolder::_holder = 0;



InputBinaryStream& operator>>(InputBinaryStream& is, Joke& joke)
{
    std::string text;
    uint64_t time=0;
    uint32_t times_showed = 0;
    is >> text >> time >> times_showed;
    joke.setJokeText(text);
    joke.setRecieveTime(time);
    joke.setTimesShowed(times_showed);
    return is;
}

OutputBinaryStream& operator<<(OutputBinaryStream& os, const Joke& joke)
{
    os << joke.getJokeText();
    os << uint64_t(joke.getRecieveTime());
    os << joke.getTimesShowed();
    return os;
}

void JokesHolder::saveKnownProviders()
{
    if(_providers_dirty)
    {
        _providers_dirty = false;

        std::ofstream oss(_providers_file_name.c_str(), std::ios::out | std::ios::binary);
        OutputBinaryStream os(oss,BinaryStream::MaxProtocolVersion);

        os << uint32_t(_known_providers.size());
        for(KnownProviderMap::iterator it = _known_providers.begin();
            it != _known_providers.end(); ++it)
        {
            os << (it->first);
            os << uint64_t(it->second._last_update);
        }
    }
}

void JokesHolder::loadKnownProviders()
{
    if(fileExists(_providers_file_name.c_str()))
    {
        std::ifstream iss(_providers_file_name.c_str(), std::ios::in | std::ios::binary);
        InputBinaryStream is(iss);

        uint32_t size = 0;
        is >> size;
        if(is.isOK() && size < BinaryStream::MaxArraySize)
        {
            for(unsigned int i=0; i<size; ++i)
            {
                JokeProviderID id;
                uint64_t last_update = 0;
                is >> id >> last_update;
                _known_providers[id] = KnownProvider(last_update);
            }
        }
    }
}

void JokesHolder::saveJokes()
{
    if(_jokes_dirty)
    {
        _jokes_dirty = false;

        std::ofstream oss(_jokes_file_name.c_str(), std::ios::out | std::ios::binary);
        OutputBinaryStream os(oss,BinaryStream::MaxProtocolVersion);

        os << _jokes;
    }
}

void JokesHolder::loadJokes()
{
    if(fileExists(_jokes_file_name.c_str()))
    {
        std::ifstream iss(_jokes_file_name.c_str(), std::ios::in | std::ios::binary);
        InputBinaryStream is(iss);

        is >> _jokes;
        std::sort(_jokes.begin(), _jokes.end());
    }
}


JokesHolder::JokesHolder()
    : _jokes_dirty(false), _providers_dirty(false), _last_update(0), _updates_running(0)
{
    std::string wpath = CCFileUtils::sharedFileUtils()->getWriteablePath();
    _jokes_file_name = wpath + "jokes.ad";
    _providers_file_name = wpath + "jokes_providers.ad";
    loadJokes();
    loadKnownProviders();
}

JokesHolder& JokesHolder::sharedHolder()
{
    if(!_holder)
    {
        _holder = new JokesHolder;
    }
    return *_holder;
}

bool JokesHolder::hasJokes() const
{
    return _jokes.size() > 0;
}
bool JokesHolder::hasJoke(const Joke& joke)
{
    bool res = false;
    for(unsigned int i=0; i<_jokes.size(); ++i)
    {
        if(_jokes[i].getJokeText() == joke.getJokeText())
            return true;
    }
    return res;
}

Joke JokesHolder::getJoke()
{
    if(hasJokes())
    {
        static unsigned int last_showed = 0;
        unsigned int idx = 0;
        if(_jokes[idx].getTimesShowed() == 1)
            idx = (++last_showed) % _jokes.size();

        Joke res = _jokes[idx];
        _jokes[idx].incrementTimesShowed();
        _jokes_dirty = true;
        updateJokes();
        return res;
    }
    return Joke();
}

void JokesHolder::addJokeProvider(JokeProviderPtr provider)
{
    std::string provider_id = provider->getProviderID();
    _providers[provider_id] = provider.release();
    if(_known_providers.find(provider_id) == _known_providers.end())
    {
        _known_providers[provider_id] = KnownProvider(0);
        _providers_dirty = true;
    }
}

class JokesHolder::ProviderUpdateTask : public LTask
{
public:
    ProviderUpdateTask(const JokeProviderID& provider_id)
        : _provider_id(provider_id)
    {}

    void do_performTask(LTaskDelegatePtr delegate)
    {
        JokesHolder& holder = JokesHolder::sharedHolder();
        if(holder._providers.find(_provider_id) == holder._providers.end())
        {
            //No provider no tasl
            delegate->onTaskFinished();
            holder.updateFinished();
            return;
        }
        JokeProvider* provider = holder._providers[_provider_id];
        provider->getJokes(JokesDelegatePtr(new Reciever(delegate)));
    }

private:
    class Reciever : public JokesDelegate
    {
    public:
        Reciever(LTaskDelegatePtr delegate)
            : _task_delegate(delegate)
        {}
        virtual void onJokesRecieved(const Jokes& jokes)
        {
            time_t arrival_scan = time(0);
            JokesHolder& holder = JokesHolder::sharedHolder();

            holder._jokes.reserve(holder._jokes.size() + jokes.size());
            for(unsigned int i=0; i<jokes.size(); ++i)
            {
                Joke joke(jokes[i]);
                joke.setRecieveTime(arrival_scan+i);
                joke.setTimesShowed(0);
                if(!holder.hasJoke(joke))
                    holder._jokes.push_back(joke);
            }
            holder.updateJokes(true);
            holder._jokes_dirty = true;

            while(!holder._waiting.empty())
            {
                JokesGetDelegate* waiter = holder._waiting.front();
                holder._waiting.pop();
                waiter->jokeArrived(holder.getJoke());
                delete waiter;
            }
            JokesHolder::sharedHolder().updateFinished();
            _task_delegate->onTaskFinished();
        }
        virtual void onJokesError(const Error,
                                  const std::string&)
        {
            JokesHolder::sharedHolder().updateFinished();
            _task_delegate->onTaskFinished();

        }
    private:
        LTaskDelegatePtr _task_delegate;
    };

    JokeProviderID _provider_id;
};


bool operator<(const Joke& a, const Joke& b)
{
    return (a.getTimesShowed() < b.getTimesShowed()
            || (a.getTimesShowed() == b.getTimesShowed() && a.getRecieveTime() > b.getRecieveTime()));
}
#include "LTaskManager.h"
void JokesHolder::updateJokes(bool lazy)
{
    if(_jokes_dirty)
    {
        std::sort(_jokes.begin(), _jokes.end());
        const unsigned int JOKES_TO_STORE = 25;

        if(_jokes.size() > JOKES_TO_STORE)
        {
            CCLog("Jokes size resize %d", _jokes.size());
            _jokes.erase(_jokes.begin() + JOKES_TO_STORE, _jokes.end());
        }
    }

    const time_t UPDATE_EVERY = 3600;

    time_t current_time = time(0);
    LTaskManager& manager = LTaskManager::sharedManager();
    bool somebody_updated = false;
    if(current_time - _last_update > UPDATE_EVERY)
    {
        _last_update = current_time;
        for(ProviderMap::iterator prov=_providers.begin();
            prov != _providers.end(); ++prov)
        {
            JokeProviderID id = prov->second->getProviderID();
            KnownProvider& info = _known_providers[id];

            if(current_time - info._last_update > UPDATE_EVERY / 2)
            {
                info._last_update = current_time;
                _providers_dirty = true;
                manager.addTask(LTaskPtr(new ProviderUpdateTask(id)));
                _updates_running++;
                somebody_updated = true;
            }
        }
    }
    if(!somebody_updated && !lazy)
    {
        updateFinished();
    }
    manager.addTask(JokesHolder::getFlushTask());
}

void JokesHolder::getJoke(JokesGetDelegatePtr delegate)
{
    if(hasJokes())
    {
        delegate->jokeArrived(getJoke());
    }
    else
    {
        JokesGetDelegate* ptr = delegate.release();
        _waiting.push(ptr);
        updateJokes();
    }
}

void JokesHolder::setUpdateDelegate(JokesUpdateDelegatePtr delegate)
{
    _update_delegate = delegate;
}

void JokesHolder::updateFinished()
{
    if(_updates_running > 0)
        _updates_running--;

    if(_updates_running == 0 && _update_delegate.get())
    {
        _update_delegate->onUpdateFinished();
    }
}
