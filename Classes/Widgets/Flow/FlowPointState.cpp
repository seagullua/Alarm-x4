#include "FlowPointState.h"

FlowPointState::FlowPointState()
    : _line_color(0), _state(Free), _highlighted(false), _has_next(false),
      _has_previous(false)
{}

void FlowPointState::setNodeType(const NodeType s)
{
    _state = s;
}

void FlowPointState::setLineColor(const FlowColor c)
{
    _line_color = c;
}
void FlowPointState::setHighlightColor(const FlowColor c)
{
    _highlight_color = c;
}

void FlowPointState::setHighlighted(const bool h)
{
    _highlighted = h;
}
void FlowPointState::setNextCordinates(const FlowPoint p)
{
    _next_cordinates = p;
    _has_next = true;
}


void FlowPointState::setCordinates(const FlowPoint p)
{
    _my_cordinates = p;
}

const FlowPointState::NodeType FlowPointState::getNodeType() const
{
    return _state;
}
const FlowColor FlowPointState::getLineColor() const
{
    return _line_color;
}
const FlowColor FlowPointState::getHighlightColor() const
{
    return _highlight_color;
}
const bool FlowPointState::isHighlighted() const
{
    return _highlighted;
}
const FlowPoint FlowPointState::getNextCordinates() const
{
    return _next_cordinates;
}
bool FlowPointState::hasNext() const
{
    return _has_next;
}

void FlowPointState::removeNext()
{
    _has_next = false;
}


void FlowPointState::setPreviousCordinates(const FlowPoint p)
{
    _previous_cordinates = p;
    _has_previous = true;
}

void FlowPointState::removePrevious()
{
    _has_previous = false;
}
bool FlowPointState::hasPrevious() const
{
    return _has_previous;
}

const FlowPoint FlowPointState::getCordinates() const
{
    return _my_cordinates;
}

const FlowPoint FlowPointState::getPreviousCordinates() const
{
    return _previous_cordinates;
}

bool FlowPointState::isFree() const
{
    return !hasPrevious() && !hasNext();
}
