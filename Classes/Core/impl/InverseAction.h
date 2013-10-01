#ifndef INVERSEACTION_H
#define INVERSEACTION_H
#ifndef MTILE_IMPL
#error "Include is forbidden"
#endif
#include "cocos2d.h"
using namespace cocos2d;

class InverseAction
{
public:

    /**
     * @brief Runs action
     * @return return true if the back action was performed
     * return false if the back action was not performed
     */
    virtual bool runAction(MTileManager::ExWApiPtr)
    {
        return false;
    }

    InverseAction()
    {}

    virtual ~InverseAction()
    {}
};

class HideWidget : public InverseAction
{
public:
    HideWidget(Widget* widget)
        : _widget(widget)
    {}

    virtual bool runAction(MTileManager::ExWApiPtr api)
    {
        api->noBackSectionStart();
        api->removeWidget(_widget);
        api->noBackSectionEnd();
        return false;
    }
private:
    Widget* _widget;
};

class ShowWidget : public InverseAction
{
public:
    ShowWidget(Widget* widget)
        : _widget(widget)
    {}

    virtual bool runAction(MTileManager::ExWApiPtr api)
    {
        api->noBackSectionStart();
        api->showWidget(_widget);
        api->noBackSectionEnd();
        return false;
    }
private:
    Widget* _widget;
};

class BackBarrier : public InverseAction
{
public:
    BackBarrier()
    {}
    virtual bool runAction(MTileManager::ExWApiPtr)
    {
        return true;
    }
};

class BackCallback : public InverseAction
{
public:
    BackCallback(CCObject* obj, SEL_CallFunc method)
        :_obj(obj), _method(method)
    {
    }
    virtual bool runAction(MTileManager::ExWApiPtr)
    {
        if(_obj && _method)
            (_obj->*_method)();
        return true;
    }
private:
    CCObject* _obj;
    SEL_CallFunc _method;
};
#endif // INVERSEACTION_H
