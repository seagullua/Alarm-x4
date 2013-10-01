#ifndef TURNONALARMMENUWIDGET_H
#define TURNONALARMMENUWIDGET_H

#include "Core/Widget.h"

class TurnOnAlarmMenuDelegate
{
public:
    TurnOnAlarmMenuDelegate(cocos2d::CCObject* wake_up_obj=0,
                            cocos2d::SEL_CallFunc wake_up_method=0,
                            cocos2d::CCObject* go_out_obj=0,
                            cocos2d::SEL_CallFunc go_out_method=0)
        : _wake_up_obj(wake_up_obj), _wake_up_method(wake_up_method),
          _go_out_obj(go_out_obj), _go_out_method(go_out_method)
    {}

    virtual ~TurnOnAlarmMenuDelegate()
    {}

    virtual void onWakeUpClick()
    {
        if(_wake_up_method && _wake_up_obj)
        {
            (_wake_up_obj->*_wake_up_method)();
        }
    }

    virtual void onGoOutInTimeClick()
    {
        if(_go_out_method && _go_out_obj)
        {
            (_go_out_obj->*_go_out_method)();
        }
    }
private:
    cocos2d::CCObject* _wake_up_obj;
    cocos2d::SEL_CallFunc _wake_up_method;
    cocos2d::CCObject* _go_out_obj;
    cocos2d::SEL_CallFunc _go_out_method;
};
typedef std::auto_ptr<TurnOnAlarmMenuDelegate> TurnOnAlarmMenuDelegatePtr;

class TurnOnAlarmMenuWidget : public Widget
{
public:
    TurnOnAlarmMenuWidget(TurnOnAlarmMenuDelegatePtr delegate);

private:
    TurnOnAlarmMenuWidget(const TurnOnAlarmMenuWidget&);
    TurnOnAlarmMenuWidget& operator=(const TurnOnAlarmMenuWidget&);

    void onInit();
    void displayButtons();

    void onWakeUpClick(MTile *);
    void onGoOutInTimeClick(MTile *);


    MTileBlock _rs;
    TurnOnAlarmMenuDelegatePtr _delegate;
};

#endif // TURNONALARMMENUWIDGET_H
