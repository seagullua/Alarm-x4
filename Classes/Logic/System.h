#ifndef SYSTEM_H
#define SYSTEM_H
#include "ValueStorage.h"
#include "DelayedActions.h"
#include "Alarm.h"
#include "MusicBox.h"
class System
{
public:
    static ValueStorage& getValueStorage()
    {
        return *_storage;
    }

    static DelayedActions& getDelayedActions()
    {
        return *_actions;
    }

    static Alarm& getAlarm()
    {
        return *_alarm;
    }

    static MusicBox& getMusicBox()
    {
        return *_music_box;
    }
private:
    friend class AppDelegate;
    class Loader
    {
    public:
        Loader()
        {

        }
        void init()
        {
            //Create in strict order to controll arrival order
            _storage = new ValueStorage;
            _actions = new DelayedActions;
            _alarm = new Alarm;
            _music_box = new MusicBox;
        }

        ~Loader()
        {
            //Delete in reverse order
            delete _music_box;
            delete _alarm;
            delete _actions;
            delete _storage;
        }
    };

    static Loader _ld;

    static ValueStorage* _storage;
    static DelayedActions* _actions;
    static Alarm* _alarm;
    static MusicBox* _music_box;


};

#endif // SYSTEM_H
