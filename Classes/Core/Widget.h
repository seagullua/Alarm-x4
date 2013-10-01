#ifndef WIDGET_H
#define WIDGET_H
#include "cocos2d.h"
#include "MTileManager.h"
#include "LabelFactory.h"



class Widget : public cocos2d::CCObject
{
public:
    Widget()
        : _is_hidden(true), _is_initialized(false)
    {}

    virtual ~Widget()
    {
        delete _api;
    }

protected:
    typedef MTileManager::Api Api;
    MTileManager::ApiPtr getApi() const
    {
        return _api;
    }
    bool isHidden() const
    {
        return _is_hidden;
    }
    bool isInitialized() const
    {
        return _is_initialized;
    }
private:
    Widget(const Widget&);
    Widget& operator=(const Widget&);
    friend class MTileManager;

    void initialize(MTileManager::ApiPtr api)
    {
        _is_hidden = false;
        _api = api;
        _is_initialized = true;
        onInit();

    }

    void suspend()
    {
        onSuspend();
        _is_hidden = true;
    }

    void awake()
    {
        _is_hidden = false;
        onInit();
    }

    virtual void onInit()
    {}

    virtual void onSuspend()
    {}


    MTileManager::ApiPtr _api;
    bool _is_hidden;
    bool _is_initialized;
};

//template <class W>
//WidgetPtr widget()
//{
//    return WidgetPtr(new W);
//}

//template <class W, class A1>
//WidgetPtr widget(A1 a1)
//{
//    return WidgetPtr(new W(a1));
//}

//template <class W, class A1, class A2>
//WidgetPtr widget(A1 a1, A2 a2)
//{
//    return WidgetPtr(new W(a1, a2));
//}

//template <class W, class A1, class A2, class A3>
//WidgetPtr widget(A1 a1, A2 a2, A3 a3)
//{
//    return WidgetPtr(new W(a1, a2, a3));
//}

//template <class W, class A1, class A2, class A3, class A4>
//WidgetPtr widget(A1 a1, A2 a2, A3 a3, A4 a4)
//{
//    return WidgetPtr(new W(a1, a2, a3, a4));
//}

//template <class W, class A1, class A2, class A3, class A4, class A5>
//WidgetPtr widget(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5)
//{
//    return WidgetPtr(new W(a1, a2, a3, a4, a5));
//}
template <class W, class Api>
W* addWidget(Api api)
{
    W* widget = new W;
    api->addWidget(WidgetPtr(widget));
    return widget;
}

template <class W, class Api, class A1>
W* addWidget(Api api, A1 a1)
{
    W* widget = new W(a1);
    api->addWidget(WidgetPtr(widget));
    return widget;
}

template <class W, class Api, class A1, class A2>
W* addWidget(Api api, A1 a1, A2 a2)
{
    W* widget = new W(a1,a2);
    api->addWidget(WidgetPtr(widget));
    return widget;
}

template <class W, class Api, class A1, class A2, class A3>
W* addWidget(Api api, A1 a1, A2 a2, A3 a3)
{
    W* widget = new W(a1,a2,a3);
    api->addWidget(WidgetPtr(widget));
    return widget;
}

template <class W, class Api, class A1, class A2, class A3, class A4>
W* addWidget(Api api, A1 a1, A2 a2, A3 a3, A4 a4)
{
    W* widget = new W(a1,a2,a3,a4);
    api->addWidget(WidgetPtr(widget));
    return widget;
}

template <class W, class Api, class A1, class A2, class A3, class A4, class A5>
W* addWidget(Api api, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5)
{
    W* widget = new W(a1,a2,a3,a4,a5);
    api->addWidget(WidgetPtr(widget));
    return widget;
}



template <class D>
std::auto_ptr<D> delegate()
{
    typedef std::auto_ptr<D> DPtr;
    return DPtr(new D);
}

template <class D>
std::auto_ptr<D> delegate(cocos2d::CCObject* obj,
                          cocos2d::SEL_CallFunc method)
{
    typedef std::auto_ptr<D> DPtr;
    return DPtr(new D(obj, method));
}

template <class D>
std::auto_ptr<D> delegate(cocos2d::CCObject* obj,
                          cocos2d::SEL_CallFunc method,
                          cocos2d::CCObject* obj2,
                          cocos2d::SEL_CallFunc method2)
{
    typedef std::auto_ptr<D> DPtr;
    return DPtr(new D(obj, method, obj2, method2));
}

#endif // WIDGET_H
