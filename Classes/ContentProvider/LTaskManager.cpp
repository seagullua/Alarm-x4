#include "LTaskManager.h"
LTaskManager* LTaskManager::_manager = 0;
LTaskManager& LTaskManager::sharedManager()
{
    if(!_manager)
    {
        _manager = new LTaskManager;
    }
    return *_manager;
}

void LTaskManager::addTask(LTaskPtr task)
{
    LTask* mytask = task.release();
    _task_queue.push(mytask);
}
void LTaskManager::pauseFor(float seconds)
{
    if(_pause < seconds)
        _pause = seconds;
}

void LTaskManager::updateEvent(float spent)
{
    _pause -= spent;
    if(_pause > 0)
        return;

    if(_is_running)
        return;

    if(_task_queue.empty())
        return;

    LTask* task = _task_queue.front();
    _task_queue.pop();

    task->performTask(LTaskDelegatePtr(new Finisher));
    _now_running = task;
}

LTaskManager::LTaskManager()
    : _is_running(false), _now_running(0), _pause(0)
{

}
