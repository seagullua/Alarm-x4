#include "FlowTable.h"
#include <cassert>

FlowTable::FlowTable(const unsigned int width, const unsigned int height)
    : _width(width), _height(height), _colors(), _table(width, Row(height))
{
    for(unsigned int x = 0; x<width; ++x)
    {
        for(unsigned int y=0; y<height; ++y)
        {
            _table[x][y].setCordinates(FlowPoint(x, y));
        }
    }
}



void FlowTable::addColor(const FlowPoint first,
              const FlowPoint second)
{
    assert(get(first).getNodeType() == FlowPointState::Free &&
           get(second).getNodeType() == FlowPointState::Free);

    FlowPointPair pair(first, second);
    FlowPointState& first_s = get(first);
    FlowPointState& second_s = get(second);

    FlowColor color = _colors.size();
    _colors.push_back(pair);

    first_s.setNodeType(FlowPointState::Circle);
    first_s.setLineColor(color);

    second_s.setNodeType(FlowPointState::Circle);
    second_s.setLineColor(color);

}

const unsigned int FlowTable::getWidth() const
{
    return _width;
}

const unsigned int FlowTable::getHeight() const
{
    return _height;
}

const FlowPointState& FlowTable::operator()(const Cordinate x,
                                 const Cordinate y) const
{
    assert(x < _width && y < _height);
    return _table[x][y];
}

FlowPointState& FlowTable::operator()(const Cordinate x,
                           const Cordinate y)
{
    assert(x < _width && y < _height);
    return _table[x][y];
}

const FlowPointState& FlowTable::get(const FlowPoint& p) const
{
    return this->operator ()(p.x(), p.y());
}
FlowPointState& FlowTable::get(const FlowPoint& p)
{
    return this->operator ()(p.x(), p.y());
}

const unsigned int FlowTable::getColorsNumber() const
{
    return _colors.size();
}
