#include "TimeSelector.h"
#include "Tiles/SliderSelectorTile.h"
#include <sstream>
#include <cmath>
using namespace cocos2d;

TimeSelector::TimeSelector(const std::string &label, const Time time, TimeSelector::DelegatePtr delegate, Anchor::Horizontal a_h, Anchor::Vertical a_v)
    : _hours(time.getHour()), _minutes(time.getMinute()), _main_tile(0),
      _label(label), _delegate(delegate), _anchor_h(a_h), _anchor_v(a_v)
{
}

class TimeSelector::SliderDelegate : public SliderSelectorTile::Delegate
{
public:
    SliderDelegate(TimeSelector& parent, unsigned int& value)
        : _parent(parent), _value(value)
    {}

    void onValueChanged(const unsigned int nv)
    {
        _value = nv;
        _parent.onTimeUpdated();
    }

private:
    TimeSelector& _parent;
    unsigned int& _value;
};


/**
 * @brief Intialization of the widget
 */
void TimeSelector::onInit()
{
    //Reserve scpace
    _rs = getApi()->reserveSpace(_anchor_h, _anchor_v, MTileSize(8,4),
                                 MTileSize(6,4));

    //Draw UI elements
    initUI();
}

void TimeSelector::initUI()
{
    //If the space is reserved
    if(_rs.isDefined())
    {

        //Add clock tile
        _clock_tile = ClockTile::create(_label, Time(_hours, _minutes),
                                        _rs.getPosition(),
                                        MTileSize(_rs.getSize().width(),2),
                                        ClockTile::Big, "green.png");
        getApi()->addTile(_clock_tile);

        //Add sliders
        typedef SliderSelectorTile::DelegatePtr Delegate;

        Delegate hour_delegate(new SliderDelegate(*this, _hours));
        Delegate minute_delegate(new SliderDelegate(*this, _minutes));

        getApi()->addTile(SliderSelectorTile::create(
                              R::tr(R::TimeSelector_Hours),
                              MTilePosition(_rs.getPosition().x(),
                                            _rs.getPosition().y()+2),
                              MTileSize(_rs.getSize().width(), 1),
                              _hours,
                              hour_delegate,
                              0,
                              24));

        getApi()->addTile(SliderSelectorTile::create(
                              R::tr(R::TimeSelector_Minutes),
                              MTilePosition(_rs.getPosition().x(),
                                            _rs.getPosition().y()+3),
                              MTileSize(_rs.getSize().width(), 1),
                              _minutes,
                              minute_delegate,
                              0,
                              60,
                              5));

    }
}


void TimeSelector::setTime(const Time time)
{
    _hours = time.getHour();
    _minutes = time.getMinute();

    _clock_tile->setTime(time);
}

const Time TimeSelector::getTime() const
{
    return Time(_hours % 24, _minutes % 60);
}

void TimeSelector::setDelegate(DelegatePtr delegate)
{
    _delegate = delegate;
}

void TimeSelector::performTimeCallback()
{
    if(_delegate.get() != 0)
    {
        _delegate->onTimeChanged(getTime());
    }
}

void TimeSelector::onTimeUpdated()
{
    _clock_tile->setTime(getTime());
    performTimeCallback();
}
