#include "AlarmTimeChooser.h"
#include "Widgets/TimeSelector.h"
#include "DialogWidget.h"
AlarmTimeChooser::AlarmTimeChooser(const Mode m, DelegatePtr delegate, const Time wake_up_default, const Time go_out_default)
    : _mode(m), _wake_up(wake_up_default), _go_out(wake_up_default+go_out_default),
      _delegate(delegate)
{
}

class AlarmTimeChooser::TimeSelectorDelegate : public TimeSelector::Delegate
{
public:
    TimeSelectorDelegate(AlarmTimeChooser& parent,
                         Time& time)
        : _parent(parent), _time(time)
    {}
    void onTimeChanged(const Time t)
    {
        _time = t;
        _parent.onTimeChanged();
    }

    static TimeSelector::DelegatePtr create(AlarmTimeChooser& parent,
                                            Time& time)
    {
        return TimeSelector::DelegatePtr(new TimeSelectorDelegate(
                                             parent, time));
    }

private:
    AlarmTimeChooser& _parent;
    Time& _time;
};

class AlarmTimeChooser::DialogDelegate : public DialogWidget::Delegate
{
public:
    DialogDelegate(AlarmTimeChooser* parent)
        : _parent(parent)
    {

    }

    void onAcceptClick()
    {
        if(_parent->_mode == WakeUpAndGoOut)
        {
            _parent->_delegate->onWakeUpAndGoOutChoosen(_parent->_wake_up,
                                                        _parent->_go_out);
        }
        else
        {
            _parent->_delegate->onWakeUpTimeChoosen(_parent->_wake_up);
        }
    }

    void onDeclineClick()
    {
        _parent->_delegate->onCancel();
    }

private:
    AlarmTimeChooser* _parent;
};

void AlarmTimeChooser::onInit()
{
    addWidget<TimeSelector>(getApi(), R::tr(R::AlarmTimeChooser_AlarmIsSet), _wake_up,
                            TimeSelectorDelegate::create(*this, _wake_up));





    _sleep_time = ClockTile::create(
                R::tr(R::AlarmTimeChooser_TimeToSleep),
                _wake_up, MTilePosition(0,0), MTileSize(6,2),ClockTile::Small,
                "orange.png");

    MTile* tile_for_dialog = _sleep_time;

    if(_mode == WakeUpAndGoOut)
    {
        MTileBlock rs = getApi()->reserveSpace(Anchor::Left, Anchor::Top, MTileSize(6,2));

        _prepare_time = ClockTile::create(
                    R::tr(R::AlarmTimeChooser_TimeToGetReady),
                    _wake_up, MTilePosition(rs.getPosition()), rs.getSize(),
                    ClockTile::Small,
                    "orange.png");

        tile_for_dialog= _prepare_time;
        getApi()->addTile(_sleep_time);

        addWidget<TimeSelector>(getApi(), R::tr(R::AlarmTimeChooser_ExitTime),
                                _go_out,
                                TimeSelectorDelegate::create(*this, _go_out),
                                Anchor::Right, Anchor::Bottom);
    }


    DialogWidget* dialog = addWidget<DialogWidget>(
                getApi(), tile_for_dialog,
                R::tr(R::AlarmTimeChooser_SetAlarm),
                R::tr(R::AlarmTimeChooser_Cancel),
                DialogWidget::DelegatePtr(new DialogDelegate(this)));

    dialog->setAnchor(Anchor::Left, Anchor::Top);

    onTimeChanged();
}

void AlarmTimeChooser::onTimeChanged()
{
    //_wake_up_time->setTime(_wake_up);
    Time to_sleep = _wake_up - Time::now();
    Second to_sleep_s = to_sleep.getInSeconds();
    _sleep_time->setTime(Time::fromSeconds(to_sleep_s - (to_sleep_s % 300)));

    if(_mode == WakeUpAndGoOut)
    {
        _prepare_time->setTime(_go_out - _wake_up);
    }

}
