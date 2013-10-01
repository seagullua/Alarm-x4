#ifndef VKRSSPARSER_H
#define VKRSSPARSER_H

#include "JokeProvider.h"

class VKRssJokeProvider : public JokeProvider
{
public:
    VKRssJokeProvider(const std::string& feed_url, bool rebuild_url=true);
private:
    void do_getJokes(JokesDelegatePtr delegate);
    JokeProviderID do_getProviderID() const;
    VKRssJokeProvider(const VKRssJokeProvider&);
    VKRssJokeProvider& operator=(const VKRssJokeProvider&);
    std::string _feed_url;
    bool _rebuild_url;
};

#endif // VKRSSPARSER_H
