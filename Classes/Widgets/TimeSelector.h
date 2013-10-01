#ifndef TIMESELECTOR_H
#define TIMESELECTOR_H
#include "Core/Widget.h"
#include "Tiles/ClockTile.h"
#include "Logic/Time.h"

class TimeSelector : public Widget
{
public:


    void setTime(const Time time);
    const Time getTime() const;

    class Delegate
    {
    public:
        Delegate(){}
        virtual void onTimeChanged(const Time){}
        virtual ~Delegate(){}
    };

    typedef std::auto_ptr<Delegate> DelegatePtr;
    void setDelegate(DelegatePtr delegate);

    TimeSelector(const std::string& label,
                 const Time time=Time(8,30),
                 DelegatePtr delegate = DelegatePtr(0),
                 Anchor::Horizontal a_h = Anchor::Right,
                 Anchor::Vertical a_v = Anchor::Top);

private:
    void onInit();
    void initUI();

    void performTimeCallback();

    MTileBlock _rs;

    void onTimeUpdated();

    ClockTile* _clock_tile;
    MTile* _main_tile;

    std::string _label;

    DelegatePtr _delegate;

    Hour _hours;
    Minute _minutes;

    Anchor::Horizontal _anchor_h;
    Anchor::Vertical _anchor_v;

    class SliderDelegate;

};

#endif // TIMESELECTOR_H
