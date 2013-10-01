#include "FlowRendererNode.h"
#include "FlowRenderer.h"
using namespace cocos2d;
FlowRendererNode::FlowRendererNode()
    : _parent(0), _point(0), _highlight_sprite(0), _circle_sprite(0), _connection_sprite(0)

{
}

void FlowRendererNode::setParent(FlowRenderer* parent)
{
    _parent = parent;
}

void FlowRendererNode::setPointState(FlowPointState* state)
{
    _point = state;
}

void FlowRendererNode::setHighlightSprite(cocos2d::CCSprite* sp)
{
    _highlight_sprite = sp;
    _highlight_sprite->setVisible(false);
}

void FlowRendererNode::update(const FlowPointState& new_state)
{
    //Connection check
    if(new_state.hasNext() != _point->hasNext() ||
            new_state.getNextCordinates() != _point->getNextCordinates() ||
            new_state.getLineColor() != _point->getLineColor())
    {
        if(!new_state.hasNext())
            hideConnection();
        else
            showConnection(new_state);
    }

    //Highlight check
    if(new_state.isHighlighted() != _point->isHighlighted() ||
            new_state.getHighlightColor() != _point->getHighlightColor())
    {
        if(new_state.isHighlighted())
            showHighlight(new_state);
        else
            hideHighlight();
    }
}

void FlowRendererNode::init()
{
    if(_point->getNodeType() == FlowPointState::Circle)
        createCircle();
    if(_point->isHighlighted())
        showHighlight(*_point);
    if(_point->hasNext())
        showConnection(*_point);
}

void FlowRendererNode::showHighlight(const FlowPointState& new_state)
{
    _highlight_sprite->setVisible(true);
    _highlight_sprite->setColor(_parent->getRenderColor(new_state.getHighlightColor()));

}

void FlowRendererNode::hideHighlight()
{
    _highlight_sprite->setVisible(false);
}

void FlowRendererNode::createCircle()
{
    if(_circle_sprite == 0)
    {
        _circle_sprite = _parent->createSprite("circle.png");
        _circle_sprite->setPosition(_parent->getNodePosition(_point->getCordinates()));
        _circle_sprite->setColor(_parent->getRenderColor(_point->getLineColor()));
    }
}

void FlowRendererNode::showConnection(const FlowPointState& new_state)
{
    if(new_state.hasNext())
    {
        if(_connection_sprite == 0)
        {
            _connection_sprite = _parent->createSprite("connection.png");
        }
        _connection_sprite->setVisible(true);
        _connection_sprite->setColor(_parent->getRenderColor(new_state.getLineColor()));
        FlowPoint my = new_state.getCordinates();
        FlowPoint target = new_state.getNextCordinates();

        int h_diff = int(my.x()) - int(target.x());
        int v_diff = int(my.y()) - int(target.y());

        CCSize node_size = _parent->getNodeSize();
        CCPoint pos = _highlight_sprite->getPosition();

        if(h_diff == 0)
        {
            _connection_sprite->setRotation(90);
            _connection_sprite->setPositionX(pos.x);
            _connection_sprite->setPositionY(pos.y - node_size.height /2 * v_diff);
        }
        else if(v_diff == 0)
        {
            _connection_sprite->setRotation(0);
            _connection_sprite->setPositionY(pos.y);
            _connection_sprite->setPositionX(pos.x - node_size.width /2 * h_diff);
        }
    }
}

void FlowRendererNode::hideConnection()
{
    if(_connection_sprite != 0)
    {
        _connection_sprite->setVisible(false);
    }
}
