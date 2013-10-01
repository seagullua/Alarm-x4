#include "AlarmWidget.h"
#include "TimeWidget.h"
#include "Tiles/CurrentTimeTile.h"
#include "Widgets/DialogWidget.h"
#include "cocos2d.h"
using namespace cocos2d;
AlarmWidget::AlarmWidget(DelegatePtr delegate)
    : _delegate(delegate)
{
}

void AlarmWidget::onInit()
{
    MTileBlock timeWidget = getApi()->reserveSpace(Anchor::Left, Anchor::Top, MTileSize(6,3));
    MTile* time =CurrentTimeTile::create(timeWidget.getPosition(), timeWidget.getSize());
    getApi()->addTile(time);


    //MTileBlock rs = getApi()->reserveSpace(Anchor::HMiddle, Anchor::Top, MTileSize(6,3));
    MTile* good_morning = MTile::create("orange.png", MTilePosition(0,0), MTileSize(8,3));
    good_morning->addChild(LF::label(R::tr(R::AlarmWidget_TimeToWakeUp), LF::BIG_FONT * 1.2f),
                           Aligment::HCenter, Aligment::VCenter);
    //getApi()->addTile(good_morning);


    DialogWidget* dialog = addWidget<DialogWidget>(getApi(), good_morning, R::tr(R::AlarmWidget_TurnOff), R::tr(R::AlarmWidget_Snooze),
                            delegate<DialogWidget::Delegate>(this, callfunc_selector(AlarmWidget::onTurnOffClick),
                                                             this, callfunc_selector(AlarmWidget::onSnoozeClick)));
    dialog->setAnchor(Anchor::Right, Anchor::Bottom);
}

void AlarmWidget::onSnoozeClick()
{
    if(_delegate.get())
    {
        _delegate->onSnoozeClick();
    }
}

void AlarmWidget::onTurnOffClick()
{
    if(_delegate.get())
    {
        _delegate->onTurnOffClick();
    }
}
