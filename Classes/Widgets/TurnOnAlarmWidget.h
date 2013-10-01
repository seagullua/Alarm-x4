#ifndef TURNONALARMWIDGET_H
#define TURNONALARMWIDGET_H
#include "Core/Widget.h"
#include "Logic/Time.h"

class TurnOnAlarmDelegate
{
public:
    TurnOnAlarmDelegate(cocos2d::CCObject* obj=0,
                        cocos2d::SEL_CallFunc method=0,
                        cocos2d::CCObject* obj2=0,
                        cocos2d::SEL_CallFunc method2=0)
        : _obj(obj), _method(method),
          _obj2(obj2), _method2(method2)
    {}

    virtual ~TurnOnAlarmDelegate()
    {}

    virtual void onAddAlarmClick()
    {
        if(_obj && _method)
        {
            (_obj->*_method)();
        }
    }

    virtual void onRemoveAlarmClick()
    {
        if(_obj2 && _method2)
        {
            (_obj2->*_method2)();
        }
    }

private:
    cocos2d::CCObject* _obj;
    cocos2d::SEL_CallFunc _method;
    cocos2d::CCObject* _obj2;
    cocos2d::SEL_CallFunc _method2;
};
typedef std::auto_ptr<TurnOnAlarmDelegate> TurnOnAlarmDelegatePtr;

class TurnOnAlarmWidget : public Widget
{
public:
    enum Mode
    {
        MorningMode,
        NormalMode
    };

    TurnOnAlarmWidget(TurnOnAlarmDelegatePtr delegate, const Mode mode);
private:
    TurnOnAlarmWidget(const TurnOnAlarmWidget&);
    TurnOnAlarmWidget& operator=(const TurnOnAlarmWidget&);

    void onInit();

    //void reserveSpace();
    void showTurnOnButton();
    void showTurnOffButton();
    void showTime(const std::string& message,
                  const Time time);

    void displayButton(const MTileBlock& rs,
                       const std::string& label_t,
                       const std::string& icon);

    void onTileClick(MTile *sender);

    MTileBlock _rs;
    TurnOnAlarmDelegatePtr _delegate;
    Mode _mode;
};

#endif // TURNONALARMWIDGET_H
