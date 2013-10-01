#ifndef LTASK_H
#define LTASK_H
#include <memory>
class LTaskDelegate
{
public:
    LTaskDelegate()
    {}
    virtual ~LTaskDelegate()
    {}

    virtual void onTaskFinished()
    {}
private:
    LTaskDelegate(LTaskDelegate&);
    LTaskDelegate& operator=(LTaskDelegate&);
};


typedef std::auto_ptr<LTaskDelegate> LTaskDelegatePtr;
class LTask
{
public:
    LTask()
    {}
    virtual ~LTask()
    {}
    void performTask(LTaskDelegatePtr delegate)
    {
        do_performTask(delegate);
    }

private:
    LTask(const LTask&);
    LTask& operator=(const LTask&);

    virtual void do_performTask(LTaskDelegatePtr)=0;
};

typedef std::auto_ptr<LTask> LTaskPtr;
#endif // LTASK_H
