#ifndef ALARMWIDGET_H
#define ALARMWIDGET_H
#include "Core/Widget.h"
class AlarmWidget : public Widget
{
public:
    class Delegate
    {
    public:
        Delegate(cocos2d::CCObject* snooze_obj=0,
                                cocos2d::SEL_CallFunc snooze_method=0,
                                cocos2d::CCObject* turn_off_obj=0,
                                cocos2d::SEL_CallFunc turn_off_method=0)
            : _snooze_obj(snooze_obj), _snooze_method(snooze_method),
              _turn_off_obj(turn_off_obj), _turn_off_method(turn_off_method)
        {}

        virtual ~Delegate()
        {}

        virtual void onSnoozeClick()
        {
            if(_snooze_method && _snooze_obj)
            {
                (_snooze_obj->*_snooze_method)();
            }
        }

        virtual void onTurnOffClick()
        {
            if(_turn_off_method && _turn_off_obj)
            {
                (_turn_off_obj->*_turn_off_method)();
            }
        }
    private:
        cocos2d::CCObject* _snooze_obj;
        cocos2d::SEL_CallFunc _snooze_method;
        cocos2d::CCObject* _turn_off_obj;
        cocos2d::SEL_CallFunc _turn_off_method;
    };

    typedef std::auto_ptr<Delegate> DelegatePtr;

    AlarmWidget(DelegatePtr delegate);
private:
    void onInit();
    void onSnoozeClick();
    void onTurnOffClick();
    DelegatePtr _delegate;
};

#endif // ALARMWIDGET_H
