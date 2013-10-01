#ifndef MTILEGRID_H
#define MTILEGRID_H

#include <vector>
#include <map>
#include "MTileSize.h"
#include "MTilePosition.h"

class MTileBlock
{
public:
    MTileBlock(const MTilePosition& position=MTilePosition(0,0),
                  const MTileSize& size=MTileSize(0,0))
        : _size(size), _position(position)
    {
    }

    const MTileSize& getSize() const
    {
        return _size;
    }

    const MTilePosition& getPosition() const
    {
        return _position;
    }
    bool isDefined() const
    {
        return _size.width() !=0 && _size.height() != 0;
    }
private:
    MTileSize _size;
    MTilePosition _position;
};

class Widget;
class MTileGridNode
{
public:
    MTileGridNode(const Widget* widget=0)
        : _widget(widget)
    {
    }

    bool isFree() const
    {
        return _widget == 0;
    }

    const Widget* getWidget() const
    {
        return _widget;
    }

private:
    const Widget* _widget;
};


class MTileGrid
{
    typedef std::vector<MTileGridNode> Nodes;
    typedef std::vector<MTileBlock> ReservationList;
    typedef std::map<const Widget*, ReservationList> ReservationMap;
public:
    static float getGridSize()
    {
        return GRID_SIZE * _grid_scale;
    }

    static float getGridSpacing()
    {
        return GRID_SPACING * _grid_scale;
    }

    static float getPadding()
    {
        return PADDING * _grid_scale;
    }

    static float getGridScale()
    {
        return _grid_scale;
    }

    MTileGrid();
    ~MTileGrid();

    const MTileSize& getSize() const;
    bool canFit(const MTileBlock& block_to_check) const;
    const MTileGridNode& getNode(const unsigned int x, const unsigned int y) const;

    bool reserve(const MTileBlock& block_to_check, const Widget* widget);
    const ReservationList& getReservationList(const Widget* widget) const;
    void cancelReservation(const Widget* widget);
    bool isSpaceReservedByWidget(const MTileBlock& block_to_check,
                                 const Widget* widget) const;
private:
    MTileGridNode& getNode(const unsigned int x, const unsigned int y);
    void cancelReservationInZone(const MTileBlock& block);
    MTileGrid(const MTileGrid&);
    MTileGrid& operator=(const MTileGrid&);

    static const float PADDING;
    static const float GRID_SIZE;
    static const float GRID_SPACING;
    static float _grid_scale;



    Nodes _vec;
    MTileSize _size;
    mutable ReservationMap _map;
};

#endif // MTILEGRID_H
