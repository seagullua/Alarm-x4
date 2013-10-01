#ifndef JOKESHOLDER_H
#define JOKESHOLDER_H
#include "JokeProvider.h"
#include <ctime>
#include <map>
#include <queue>
#include "LTask.h"
class JokesGetDelegate
{
public:
    JokesGetDelegate()
    {}
    virtual void jokeArrived(const Joke&)
    {}
    virtual ~JokesGetDelegate()
    {}
};
class JokesUpdateDelegate
{
public:
    JokesUpdateDelegate()
    {}
    virtual ~JokesUpdateDelegate()
    {}
    virtual void onUpdateFinished()
    {}
};

typedef std::auto_ptr<JokesGetDelegate> JokesGetDelegatePtr;
typedef std::auto_ptr<JokesUpdateDelegate> JokesUpdateDelegatePtr;
class JokesHolder
{
public:
    static JokesHolder& sharedHolder();
    static LTaskPtr getFlushTask()
    {
        return LTaskPtr(new FlushTask);
    }


    void getJoke(JokesGetDelegatePtr);
    void updateJokes(bool lazy=false);

    void addJokeProvider(JokeProviderPtr provider);
    void setUpdateDelegate(JokesUpdateDelegatePtr delegate);
private:

    Joke getJoke();
    bool hasJokes() const;
    class FlushTask : public LTask
    {
    private:
        void do_performTask(LTaskDelegatePtr delegate)
        {

            JokesHolder& holder = sharedHolder();

            holder.saveJokes();
            holder.saveKnownProviders();
            delegate->onTaskFinished();
        }
    };

    class ProviderUpdateTask;

    struct KnownProvider
    {
        KnownProvider(time_t last_update=0)
            : _last_update(last_update)
        {}
        time_t _last_update;
    };

    typedef std::map<JokeProviderID, JokeProvider*> ProviderMap;
    typedef std::map<JokeProviderID, KnownProvider> KnownProviderMap;
    typedef std::vector<Joke> JokesVector;
    typedef std::queue<JokesGetDelegate*> JokesWaitingQueue;
    ProviderMap _providers;
    KnownProviderMap _known_providers;
    JokesVector _jokes;
    JokesWaitingQueue _waiting;

    void saveKnownProviders();
    void loadKnownProviders();
    void saveJokes();
    void loadJokes();
    bool hasJoke(const Joke& joke);

    bool _jokes_dirty;
    bool _providers_dirty;
    time_t _last_update;

    void updateFinished();
    JokesUpdateDelegatePtr _update_delegate;
    unsigned int _updates_running;

    static std::string _jokes_file_name;
    static std::string _providers_file_name;
    static JokesHolder* _holder;
    JokesHolder();
};

#endif // JOKESHOLDER_H
