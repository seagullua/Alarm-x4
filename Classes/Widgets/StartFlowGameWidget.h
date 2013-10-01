#ifndef STARTFLOWGAMEWIDGET_H
#define STARTFLOWGAMEWIDGET_H
#include "Core/Widget.h"
class StartFlowGameWidget : public Widget
{
public:
    class Delegate
    {
    public:
        Delegate(cocos2d::CCObject* obj=0,
                            cocos2d::SEL_CallFunc method=0)
            : _obj(obj), _method(method)
        {}

        virtual ~Delegate()
        {}

        virtual void onClick()
        {
            if(_obj && _method)
            {
                (_obj->*_method)();
            }
        }


    private:
        cocos2d::CCObject* _obj;
        cocos2d::SEL_CallFunc _method;
    };
    typedef std::auto_ptr<Delegate> DelegatePtr;
    StartFlowGameWidget(DelegatePtr delegate);

private:
    void onInit();
    void onClick();

    DelegatePtr _delegate;
};

#endif // STARTFLOWGAMEWIDGET_H
