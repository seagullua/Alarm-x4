#include "TurnOnAlarmMenuWidget.h"
using namespace cocos2d;
TurnOnAlarmMenuWidget::TurnOnAlarmMenuWidget(
        TurnOnAlarmMenuDelegatePtr delegate)
    : _delegate(delegate)
{

}


void TurnOnAlarmMenuWidget::onInit()
{
    _rs = getApi()->reserveSpace(Anchor::Right, Anchor::Bottom,
                                 MTileSize(6,2*3));
    displayButtons();
}

void TurnOnAlarmMenuWidget::displayButtons()
{
    if(_rs.isDefined())
    {
        MTilePosition base_pos = _rs.getPosition();
        MTileSize base_size = MTileSize(6, 2);
        {
            MTile* tile = MTile::create(
                        "violet.png",
                        base_pos,
                        base_size);

            CCSize tile_size = tile->getInnerContentSize();
            CCLabelTTF* question = LF::label(R::tr(R::TurnOnAlarmMenuWidget_AlarmPurpose),
                                             LF::MEDIUM_FONT,
                                             CCSize(tile_size.width,0),
                                             kCCTextAlignmentCenter);
            tile->addChild(question,
                           Aligment::HCenter,
                           Aligment::VCenter,
                           0, LF::vfix(LF::MEDIUM_FONT));

            getApi()->addTile(tile);
        }
        {
            MTile* tile = MTile::create(
                        "blue.png",
                        MTilePosition(base_pos.x(),
                                      base_pos.y() + base_size.height()),
                        base_size,
                        this,
                        tile_selector(TurnOnAlarmMenuWidget::onWakeUpClick)
                        );
            addImageAndLabelToTile(tile, R::tr(R::TurnOnAlarmMenuWidget_WakeUp), "bed_icon.png");
            getApi()->addTile(tile);
        }
        {
            MTile* tile = MTile::create(
                        "blue.png",
                        MTilePosition(base_pos.x(),
                                      base_pos.y() + base_size.height()*2),
                        base_size,
                        this,
                        tile_selector(TurnOnAlarmMenuWidget::onGoOutInTimeClick)
                        );
            addImageAndLabelToTile(tile, R::tr(R::TurnOnAlarmMenuWidget_WakeUpAndGoOut), "go_out.png");
            getApi()->addTile(tile);
        }
    }
}


void TurnOnAlarmMenuWidget::onWakeUpClick(MTile *)
{
    if(_delegate.get())
    {
        _delegate->onWakeUpClick();
    }
}

void TurnOnAlarmMenuWidget::onGoOutInTimeClick(MTile *)
{
    if(_delegate.get())
    {
        _delegate->onGoOutInTimeClick();
    }
}


