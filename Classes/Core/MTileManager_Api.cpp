#define MTILE_IMPL
#include "MTileManager.h"
#include "ContentProvider/LTaskManager.h"
#include "impl/MTileManager_CreateTileWithDelay.h"
#include "impl/MTileManager_AnimationFinalizer.h"
#include "impl/MTileManager_TileDecorationChanger.h"
#include "impl/MTileManager_MTileLayer.h"
#include "Script.h"
#include "Widget.h"
#include <cstdlib>

using namespace cocos2d;

static const float TILE_SCALE_TIME = 0.15f;
static const float MAX_DELAY_TIME = 0.3f;

float getRandomDelay(float max)
{
    return float(rand()) / RAND_MAX * max;
}



MTileManager::Api::~Api()
{}


MTileManager::Api::Api(Widget* widget)
    : _parent(widget)
{

}

void MTileManager::Api::addTile(MTile* tile)
{
    const MTileGrid& grid = MTileManager::sharedManager().getGrid();
    //Check reservation
    if(!grid.isSpaceReservedByWidget(
                MTileBlock(tile->getTilePosition(), tile->getTileSize()),
                _parent))
        return;

    tile->retain();

    MTileManager::sharedManager()._widgets_tiles[_parent].insert(tile);

    MTileLayer* main_layer = MTileManager::sharedManager()._main_layer;
    if(!main_layer->inTilesSet(tile)
            && !main_layer->inAnimationSet(tile))
    {
        LTaskManager::sharedManager().pauseFor(2);
        tile->setScaleY(0);
        main_layer->addChild(tile);

        tile->runAction(CCSequence::create(
                            CCDelayTime::create(getRandomDelay(MAX_DELAY_TIME)),
                            CCScaleTo::create(TILE_SCALE_TIME, 1, 1),
                            CCCallFunc::create(AF::create(tile),
                                               callfunc_selector(AF::finalize)),
                            NULL
                            ));

    }
}

void MTileManager::Api::moveTile(MTile* tile, const MTilePosition& new_pos)
{
    const MTileGrid& grid = MTileManager::sharedManager().getGrid();
    //Check reservation
    if(!grid.isSpaceReservedByWidget(
                MTileBlock(tile->getTilePosition(), tile->getTileSize()),
                _parent) ||
            !grid.isSpaceReservedByWidget(
                MTileBlock(new_pos, tile->getTileSize()),
                _parent)
            )
        return;

    MTileLayer* main_layer = MTileManager::sharedManager()._main_layer;
    if(main_layer->inTilesSet(tile))
    {
        LTaskManager::sharedManager().pauseFor(2);
        tile->runAction(
                    CCSequence::create(
                        CCDelayTime::create(getRandomDelay(MAX_DELAY_TIME/2)),
                        CCScaleTo::create(TILE_SCALE_TIME, 1, 0),
                        CCCallFunc::create(TDC::create(tile, new_pos, tile->getTileSize()),
                                           callfunc_selector(TDC::finalize)),
                        CCDelayTime::create(getRandomDelay(MAX_DELAY_TIME)),
                        CCScaleTo::create(TILE_SCALE_TIME, 1, 1),
                        CCCallFunc::create(AF::create(tile),
                                           callfunc_selector(AF::finalize)),
                        NULL
                        ));
    }
}
void MTileManager::Api::resizeTile(MTile*, const MTileSize& )
{}
void MTileManager::Api::removeTile(MTile* tile)
{
    const MTileGrid& grid = MTileManager::sharedManager().getGrid();
    //Check reservation
    if(!grid.isSpaceReservedByWidget(
                MTileBlock(tile->getTilePosition(), tile->getTileSize()),
                _parent))
        return;

    MTileManager::sharedManager()._widgets_tiles[_parent].erase(tile);

    MTileLayer* main_layer = MTileManager::sharedManager()._main_layer;
    if(main_layer->inDestroySet(tile))
    {
        return;
    }

    bool found = false;
    bool isAnimated = false;
    if(main_layer->inTilesSet(tile))
    {
        found = true;
        //_main_layer->_tiles.erase(tile);
    }
    if(main_layer->inAnimationSet(tile))
    {
        found = true;
        isAnimated= true;
        //_main_layer->_animated_tiles.erase(tile);
    }

    if(found)
    {
        LTaskManager::sharedManager().pauseFor(2);
        main_layer->_tiles_being_destroyed.insert(tile);
        main_layer->_tiles.erase(tile);
        main_layer->_animated_tiles.insert(tile);
        tile->runAction(CCSequence::create(
                            CCScaleTo::create(TILE_SCALE_TIME, 1, 0),
                            CCCallFunc::create(tile, callfunc_selector(MTile::selfDestroy)),
                            NULL
                            ));
    }

}

MTileBlock MTileManager::Api::reserveSpace(const SpaceRequest& sr)
{
    return reserveSpace(sr.getHorizontalAnchor(),
                        sr.getVerticalAnchor(),
                        sr.getMaxSize(), sr.getMinSize());
}

MTileBlock MTileManager::Api::reserveSpace(
        const Anchor::Horizontal horizontal,
        const Anchor::Vertical vertical,
        MTileSize max_size,
        MTileSize min_size)
{
    assert(horizontal == Anchor::Left || horizontal == Anchor::Right || horizontal == Anchor::HMiddle);
    assert(vertical == Anchor::Top || vertical == Anchor::Bottom);

    float max_width = max_size.width();
    float max_height = max_size.height();
    float min_width = max_width;
    float min_height = max_height;

    if(min_size.width() && min_size.width() < max_width)
        min_width = min_size.width();
    if(min_size.height() && min_size.height() < max_height)
        min_height = min_size.height();

    const MTileGrid& grid = MTileManager::sharedManager().getGrid();


    unsigned int grid_height = grid.getSize().height();
    unsigned int grid_width = grid.getSize().width();

    unsigned int first_found_y = grid_height;

    unsigned int x = 0;
    if(horizontal == Anchor::Right)
        x = grid_width-1;
    else if(horizontal == Anchor::HMiddle)
        x = (grid_width-max_size.width()) / 2;

    for(unsigned int i=0; i<grid_height; ++i)
    {
        unsigned int y = i;
        if(vertical == Anchor::Bottom)
            y = grid_height - i -1;

        if(grid.getNode(x, y).isFree())
        {
            first_found_y = y;
            break;
        }
    }

    if(first_found_y < grid_height)
    {
        //If we go from bottom
        unsigned int free_space = first_found_y;
        //If we go from top
        if(vertical == Anchor::Top)
        {
            free_space = grid_height - first_found_y - 1;
        }
        MTileSize target_size(max_width,
                              MAX(MIN(free_space, max_height),
                                  min_height));

        unsigned int target_x = 0;
        if(horizontal == Anchor::Right)
            target_x = grid_width-target_size.width();
        else if(horizontal == Anchor::HMiddle)
            target_x = (grid_width-target_size.width())/2;

        unsigned int target_y = first_found_y;
        if(vertical == Anchor::Bottom)
            target_y = first_found_y + 1 - target_size.height();

        MTilePosition target_position(target_x,target_y);
        MTileBlock request(target_position, target_size);

        //Reserve the space
        if(reserveSpace(request))
        {
            return request;
        }
    }
    return MTileBlock(MTilePosition(0,0), MTileSize(0,0));
}

void MTileManager::Api::replaceTile(MTile* old_tile, MTile* new_tile)
{
    //Check reservation
    const MTileGrid& grid = MTileManager::sharedManager().getGrid();
    bool old_tile_reserved = false;
    bool new_tile_reserved = false;
    if(old_tile &&
            grid.isSpaceReservedByWidget(
                MTileBlock(old_tile->getTilePosition(),
                           old_tile->getTileSize()),
                _parent))
    {
        old_tile_reserved = true;
    }
    else if(!old_tile)
        old_tile_reserved = true;

    if(grid.isSpaceReservedByWidget(
                MTileBlock(new_tile->getTilePosition(),
                           new_tile->getTileSize()),
                _parent))
    {
        new_tile_reserved = true;
    }

    if(!new_tile_reserved || !old_tile_reserved)
        return;

    MTileLayer* main_layer = MTileManager::sharedManager()._main_layer;
    float wait_before_creation = 0;

    if(main_layer->inDestroySet(old_tile))
    {
        wait_before_creation = TILE_SCALE_TIME;
    }
    else if(main_layer->inAnimationSet(old_tile)
            || main_layer->inTilesSet(old_tile))
    {
        removeTile(old_tile);
        wait_before_creation = TILE_SCALE_TIME;
    }
    else
    {
        wait_before_creation = 0;
    }
    LTaskManager::sharedManager().pauseFor(2);
    if(wait_before_creation == 0)
    {
        addTile(new_tile);
    }
    else
    {
        CreateTileWithDelay* delay = new CreateTileWithDelay(new_tile, wait_before_creation, this);
        delay->run();
    }
}

void MTileManager::Api::addWidget(WidgetPtr widget)
{
    Widget* ptr = widget.get();

    MTileManager& m = MTileManager::sharedManager();
    m._active_script->getApi()->addWidget(widget,MTileManager::WApi::NO_DELAY);
    m._widgets[_parent].insert(ptr);


}

bool MTileManager::Api::reserveSpace(const MTileBlock& block)
{
    return MTileManager::sharedManager()._grid.reserve(block, _parent);
}
void MTileManager::Api::cancelReservation()
{
    MTileManager::sharedManager()._grid.cancelReservation(_parent);
}


