#ifndef FLOWRENDERERNODE_H
#define FLOWRENDERERNODE_H
#include "cocos2d.h"
#include "FlowPointState.h"
class FlowRenderer;

class FlowRendererNode
{
public:
    FlowRendererNode();

    void setParent(FlowRenderer* parent);
    void setPointState(FlowPointState* state);
    void setHighlightSprite(cocos2d::CCSprite* sp);
    void update(const FlowPointState& new_state);

    void init();
private:
    FlowRenderer* _parent;
    FlowPointState* _point;
    cocos2d::CCSprite* _highlight_sprite;
    cocos2d::CCSprite* _circle_sprite;
    cocos2d::CCSprite* _connection_sprite;

    void showHighlight(const FlowPointState& new_state);
    void hideHighlight();
    void createCircle();
    void showConnection(const FlowPointState& new_state);
    void hideConnection();
};

#endif // FLOWRENDERERNODE_H
