#ifndef SCRIPT_H
#define SCRIPT_H
#include "MTileManager.h"
#include "cocos2d.h"

class Script : public cocos2d::CCObject
{
public:
    Script()
        : _is_active(false)
    {}

    virtual ~Script()
    {
        delete _api;
    }
protected:
    MTileManager::WApiPtr getApi() const
    {
        return _api;
    }
private:
    Script(const Script&);
    Script& operator=(const Script&);
    friend class MTileManager;

    virtual void onInit()
    {}

    MTileManager::ExWApiPtr getExApi() const
    {
        return _api;
    }
    void initialize(MTileManager::ExWApiPtr api)
    {
        _is_active = true;
        _api = api;
        onInit();
    }

    bool isActive() const
    {
        return _is_active;
    }

    bool _is_active;
    MTileManager::ExWApiPtr _api;
};

#endif // SCRIPT_H
