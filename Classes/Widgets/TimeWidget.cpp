#include "TimeWidget.h"
#include <sstream>
#include "Tiles/CurrentTimeTile.h"
using namespace cocos2d;

TimeWidget::TimeWidget(Anchor::Horizontal x_anchor, Anchor::Vertical y_anchor)
    :
      _x_anchor(x_anchor),
      _y_anchor(y_anchor)
{
}


void TimeWidget::onInit()
{

    MTileBlock _rs = getApi()->reserveSpace(_x_anchor, _y_anchor,
                                 MTileSize(6,3));
    CurrentTimeTile* tile = CurrentTimeTile::create(_rs.getPosition(), _rs.getSize());
    getApi()->addTile(tile);

}
