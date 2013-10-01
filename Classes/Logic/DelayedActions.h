#ifndef DELAYEDACTIONS_H
#define DELAYEDACTIONS_H

#include "Time.h"

class DelayedActions
{
public:
    enum Action
    {
        LaunchApp=1,
        WakeUp=2,

        //InternetFetch=4,
        DisplayLabel=5,

        GoOutFirst=1001,
        GoOutSecond=1002,
        GoOutThird=1003
    };



    void setActionTime(const Action, const Time);
    void cancelAction(const Action);
private:
    friend class System;
    DelayedActions();

};

#endif // DELAYEDACTIONS_H
