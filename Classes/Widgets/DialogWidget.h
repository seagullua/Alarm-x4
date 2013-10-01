#ifndef DIALOGWIDGET_H
#define DIALOGWIDGET_H
#include "Core/Widget.h"
class DialogWidget : public Widget
{
public:

    class Delegate
    {
    public:
        Delegate(cocos2d::CCObject* accept_obj=0,
                                cocos2d::SEL_CallFunc accept_method=0,
                                cocos2d::CCObject* decline_obj=0,
                                cocos2d::SEL_CallFunc decline_method=0)
            : _accept_obj(accept_obj), _accept_method(accept_method),
              _decline_obj(decline_obj), _decline_method(decline_method)
        {}

        virtual ~Delegate()
        {}

        virtual void onAcceptClick()
        {
            if(_accept_method && _accept_obj)
            {
                (_accept_obj->*_accept_method)();
            }
        }

        virtual void onDeclineClick()
        {
            if(_decline_method && _decline_obj)
            {
                (_decline_obj->*_decline_method)();
            }
        }
    private:
        cocos2d::CCObject* _accept_obj;
        cocos2d::SEL_CallFunc _accept_method;
        cocos2d::CCObject* _decline_obj;
        cocos2d::SEL_CallFunc _decline_method;
    };
    typedef std::auto_ptr<Delegate> DelegatePtr;

    DialogWidget(MTile* main_tile, const std::string& accept_text,
                 const std::string& decline_text, DelegatePtr delegate=DelegatePtr(0));
    void setAnchor(const Anchor::Horizontal x_a,
                   const Anchor::Vertical y_a);

    ~DialogWidget();

private:
    void onInit();
    void initUI();

    void onAcceptClick(MTile*);
    void onDeclineClick(MTile*);

    MTile* _ask_tile;
    DelegatePtr _delegate;
    std::string _accept_text;
    std::string _decline_text;

    Anchor::Horizontal _x_anchor;
    Anchor::Vertical _y_anchor;

    MTileBlock _rs;
};

#endif // DIALOGWIDGET_H
