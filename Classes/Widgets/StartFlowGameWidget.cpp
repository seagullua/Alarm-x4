#include "StartFlowGameWidget.h"
using namespace cocos2d;
StartFlowGameWidget::StartFlowGameWidget(DelegatePtr delegate)
    : _delegate(delegate)
{
}

void StartFlowGameWidget::onInit()
{
    MTileBlock rs = getApi()->reserveSpace(Anchor::Left, Anchor::Bottom,
                                           MTileSize(6,2));

    MTile* tile = MTile::create("blue.png", rs.getPosition(), rs.getSize(),
                                this, tile_selector(StartFlowGameWidget::onClick));
    addImageAndLabelToTile(tile, R::tr(R::StartFlowGameWidget_MorningPuzzle), "flow_icon.png");
    getApi()->addTile(tile);
}

void StartFlowGameWidget::onClick()
{
    if(_delegate.get())
        _delegate->onClick();
}
