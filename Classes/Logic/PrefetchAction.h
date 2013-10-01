#ifndef PREFETCHACTION_H
#define PREFETCHACTION_H

class PrefetchAction
{
public:
    static void performPrefetch();
private:
    void do_performPrefetch();

    void delegateFinish();

    unsigned int _waiting_delegates;
    class WeatherDelegate;
    class JokesUpdate;
    PrefetchAction();
};

#endif // PREFETCHACTION_H
