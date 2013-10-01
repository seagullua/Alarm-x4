#ifndef ALARMTIMECHOOSER_H
#define ALARMTIMECHOOSER_H
#include "Core/Widget.h"
#include "Widgets/TimeSelector.h"
#include "Widgets/DialogWidget.h"
class AlarmTimeChooser : public Widget
{
public:
    enum Mode
    {
        WakeUp,
        WakeUpAndGoOut
    };

    class Delegate
    {
    public:
        Delegate(){}
        virtual ~Delegate(){}
        virtual void onWakeUpTimeChoosen(const Time)
        {}

        virtual void onWakeUpAndGoOutChoosen(const Time wake_up,
                                             const Time go_out)
        {wake_up;go_out;}

        virtual void onCancel()
        {}
    };
    typedef std::auto_ptr<Delegate> DelegatePtr;


    AlarmTimeChooser(const Mode, DelegatePtr delegate,
                     const Time wake_up_default=Time(8,30),
                     const Time go_out_default=Time(0,0));

private:
    void onInit();
    void onTimeChanged();
    class TimeSelectorDelegate;
    class DialogDelegate;

    Time _wake_up;
    Time _go_out;

    ClockTile* _sleep_time;
    ClockTile* _prepare_time;


    DialogWidget* _dialog_widget;

    DelegatePtr _delegate;

    Mode _mode;
};

#endif // ALARMTIMECHOOSER_H
