#include "MTileGrid.h"
#include "MTile.h"
#include "cocos2d.h"
using namespace cocos2d;



MTileGrid::MTileGrid()
{
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    float scaled = CCDirector::sharedDirector()->getContentScaleFactor();
    float norm_grid_size = (GRID_SIZE + GRID_SPACING) / scaled;
    float spacing_scaled = GRID_SPACING / scaled;

    float width_normed = (visibleSize.width-spacing_scaled) / norm_grid_size;
    float height_normed = (visibleSize.height-spacing_scaled) / norm_grid_size;

    const unsigned int min_width = 15;
    const unsigned int min_height = 9;

    float scale = MIN(width_normed / min_width, height_normed / min_height);
    //If the calculate grid is enough to fit screen do not make it bigger
    _grid_scale = MIN(scale, 1);

    unsigned int tile_num_horizontaly = width_normed / _grid_scale + 0.1f;
    unsigned int tile_num_verticaly = height_normed / _grid_scale + 0.1f;

    _size = MTileSize(tile_num_horizontaly, tile_num_verticaly);
    _vec.resize(tile_num_horizontaly*tile_num_verticaly, MTileGridNode(0));

}

const MTileGridNode& MTileGrid::getNode(const unsigned int x, const unsigned int y) const
{
    return _vec[y * _size.width() + x];
}
MTileGridNode& MTileGrid::getNode(const unsigned int x, const unsigned int y)
{
    return _vec[y * _size.width() + x];
}


MTileGrid::~MTileGrid()
{}

const MTileSize& MTileGrid::getSize() const
{
    return _size;
}

bool MTileGrid::canFit(const MTileBlock& block_to_check) const
{
    bool can_fit = true;
    unsigned int left_x = block_to_check.getPosition().x();
    unsigned int right_x = left_x + block_to_check.getSize().width();

    unsigned int top_y = block_to_check.getPosition().y();
    unsigned int bottom_y = top_y + block_to_check.getSize().height();
    for(unsigned int y=top_y; y < bottom_y; ++y)
    {
        if(y >= _size.height())
        {
            can_fit = false;
            break;
        }
        for(unsigned int x=left_x; x < right_x; ++x)
        {
            if(x >= _size.width() || !getNode(x, y).isFree())
            {
                can_fit = false;
                break;
            }
        }
    }
    return can_fit;
}

bool MTileGrid::reserve(const MTileBlock& block_to_reserve, const Widget* widget)
{
    if(!canFit(block_to_reserve))
        return false;

    if(widget == 0)
        return false;

    _map[widget].push_back(block_to_reserve);

    unsigned int left_x = block_to_reserve.getPosition().x();
    unsigned int right_x = left_x + block_to_reserve.getSize().width();

    unsigned int top_y = block_to_reserve.getPosition().y();
    unsigned int bottom_y = top_y + block_to_reserve.getSize().height();

    MTileGridNode reservation(widget);
    for(unsigned int y=top_y; y < bottom_y; ++y)
    {
        for(unsigned int x=left_x; x < right_x; ++x)
        {
            getNode(x, y) = reservation;
        }
    }
    return true;
}

const MTileGrid::ReservationList& MTileGrid::getReservationList(const Widget* widget) const
{
    return _map[ widget ];
}

void MTileGrid::cancelReservationInZone(const MTileBlock& block)
{
    unsigned int left_x = block.getPosition().x();
    unsigned int right_x = left_x + block.getSize().width();

    unsigned int top_y = block.getPosition().y();
    unsigned int bottom_y = top_y + block.getSize().height();

    MTileGridNode reservation(0);
    for(unsigned int y=top_y; y < bottom_y; ++y)
    {
        for(unsigned int x=left_x; x < right_x; ++x)
        {
            getNode(x, y) = reservation;
        }
    }
}

void MTileGrid::cancelReservation(const Widget* widget)
{
    ReservationList& list = _map[widget];
    unsigned int reservations_size = list.size();
    for(unsigned int i=0; i<reservations_size; ++i)
    {
        cancelReservationInZone(list[i]);
    }
    list.clear();
}

bool MTileGrid::isSpaceReservedByWidget(const MTileBlock& block_to_check,
                                        const Widget* widget) const
{
    ReservationList& list = _map[widget];
    unsigned int left_x = block_to_check.getPosition().x();
    unsigned int right_x = left_x + block_to_check.getSize().width();

    unsigned int top_y = block_to_check.getPosition().y();
    unsigned int bottom_y = top_y + block_to_check.getSize().height();
    for(unsigned int i=0; i<list.size(); ++i)
    {
        MTileBlock block = list[i];
        MTileSize outer_size = block.getSize();
        MTilePosition outer_position = block.getPosition();

        if(
                left_x >= outer_position.x() &&
                top_y >= outer_position.y() &&
                right_x <= outer_position.x() + outer_size.width() &&
                bottom_y <= outer_position.y() + outer_size.height()
                )
        {
            return true;
        }
    }
    return false;
}


const float MTileGrid::PADDING = 15;
const float MTileGrid::GRID_SIZE = 60;
const float MTileGrid::GRID_SPACING = 20;
float MTileGrid::_grid_scale=1;
