#ifndef LTASKMANAGER_H
#define LTASKMANAGER_H
#include <queue>
#include "LTask.h"
#include "cocos2d.h"
class LTaskManager : public cocos2d::CCNode
{
public:
    static LTaskManager& sharedManager();

    void addTask(LTaskPtr task);

    void updateEvent(float pause);
    void pauseFor(float seconds);
private:
    class Finisher : public LTaskDelegate
    {
    public:
        Finisher()
        {}
        virtual void onTaskFinished()
        {
            LTaskManager& manager = sharedManager();
            manager._is_running = false;
            delete manager._now_running;
            manager._now_running = NULL;
        }
    };

    LTaskManager();

    static LTaskManager* _manager;
    bool _is_running;
    float _pause;
    LTask* _now_running;
    typedef std::queue<LTask*> TaskQueue;
    TaskQueue _task_queue;

};

#endif // LTASKMANAGER_H
