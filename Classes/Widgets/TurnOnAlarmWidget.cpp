#include "TurnOnAlarmWidget.h"
#include "cocos2d.h"
#include "Logic/System.h"
#include "Tiles/ClockTile.h"

using namespace cocos2d;
TurnOnAlarmWidget::TurnOnAlarmWidget(TurnOnAlarmDelegatePtr delegate, const Mode mode)
    : _rs(), _delegate(delegate), _mode(mode)
{
}

void TurnOnAlarmWidget::onInit()
{
    Alarm& a = System::getAlarm();
    Alarm::Mode mode = a.getMode();


    if(mode == Alarm::WakeUp ||
            mode == Alarm::WakeUpAndGoOut)
        showTurnOffButton();
    else
        showTurnOnButton();

    if(mode == Alarm::WakeUpAndGoOut || mode == Alarm::WakeUp)
    {
        showTime(R::tr(R::TurnOnAlarmWidget_AlarmIsSetFor), a.getWakeUpTime());



    }
    if(mode == Alarm::WakeUpAndGoOut || mode == Alarm::GoOut)
    {
        showTime(R::tr(R::TurnOnAlarmWidget_GoOutTime), a.getGoOutTime());

    }

}

void TurnOnAlarmWidget::showTime(const std::string& message,
                                 const Time time)
{
    MTileBlock space = getApi()->reserveSpace(
                Anchor::Right, Anchor::Top,
                MTileSize(6,2));

    getApi()->addTile(ClockTile::create(
                          message,
                          time, space.getPosition(), space.getSize(), ClockTile::Small,
                          "green.png"));
}

void TurnOnAlarmWidget::showTurnOnButton()
{
    MTileBlock space;
    if(_mode == NormalMode)
    {
        space = getApi()->reserveSpace(
                Anchor::Right, Anchor::Bottom,
                MTileSize(6,4),
                MTileSize(6,2));
    }
    else
    {
        space = getApi()->reserveSpace(
                Anchor::Right, Anchor::Bottom,
                MTileSize(6,2));
    }

    displayButton(space, R::tr(R::TurnOnAlarmWidget_TurnOnAlarm), "clock_icon.png");

}

void TurnOnAlarmWidget::showTurnOffButton()
{
    MTileBlock space = getApi()->reserveSpace(
                Anchor::Right, Anchor::Bottom,
                MTileSize(6,2));

    displayButton(space, R::tr(R::TurnOnAlarmWidget_TurnOffAlarm), "clock_icon.png");
}

void TurnOnAlarmWidget::displayButton(const MTileBlock& rs,
                                      const std::string& label_t,
                                      const std::string& icon)
{
    if(rs.isDefined())
    {

        //Create tile
        MTile* tile = MTile::create(
                    "violet.png",
                    rs.getPosition(),
                    rs.getSize(),
                    this,
                    tile_selector(TurnOnAlarmWidget::onTileClick));

        CCSprite* clock = CCSprite::create(icon.c_str());
        CCSize tile_size = tile->getInnerContentSize();

        float label_font = LF::BIG_FONT;
        float clock_scale_factor = 0.7f;
        if(rs.getSize().height() <= 2)
        {
            label_font = LF::MEDIUM_FONT;
            clock_scale_factor = 1;
        }

        float clock_scale = (tile_size.height /
                             clock->getContentSize().height) * clock_scale_factor;
        clock->setScale(clock_scale);
        tile->addChild(clock, Aligment::Left, Aligment::VCenter);



        CCLabelTTF* label = LF::label(label_t.c_str(), label_font);
        tile->addChild(label, Aligment::HCenter , Aligment::VCenter,
                       clock->getContentSize().width*clock_scale,
                       LF::vfix(label_font));

        getApi()->addTile(tile);
    }
}

void TurnOnAlarmWidget::onTileClick(MTile *)
{
    if(_delegate.get())
    {
        Alarm::Mode mode = System::getAlarm().getMode();
        if(mode == Alarm::NoSet || mode == Alarm::GoOut)
            _delegate->onAddAlarmClick();
        else
            _delegate->onRemoveAlarmClick();
    }
}
