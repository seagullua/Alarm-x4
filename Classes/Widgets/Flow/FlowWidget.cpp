#include "FlowWidget.h"
#include "FlowGame.h"
#include "Generator/FGenerator.h"
#include "SimpleAudioEngine.h"
#include "Logic/IntentParams.h"
#include "Tiles/FlowTile.h"
#include <fstream>
#include <algorithm>
using namespace cocos2d;
FlowWidget::FlowWidget(FlowGame::DelegatePtr delegate)
    : _delegate(delegate)
{
}


void FlowWidget::onInit()
{
    MTileSize s = MTileManager::sharedManager().getGrid().getSize();
    unsigned int size = MIN(s.height(), s.width() - 6);

    MTileBlock _rs = getApi()->reserveSpace(
                Anchor::Left, Anchor::Top,
                MTileSize(size, size));

    getApi()->addTile(FlowTile::create(_rs.getPosition(), _rs.getSize(), _delegate));

}


