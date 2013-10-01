#include "FlowRenderer.h"
using namespace cocos2d;

FlowRenderer::FlowRenderer(const FlowTable& table)
    : _table(table), _nodes_renderers(table.getWidth(), Row(table.getHeight())),
      _active_circle(0)
{
    //Get the sprites loader
    _spl = GraphicsManager::getLoaderFor(this,
                                                      "flow.plist",
                                                      "flow.png");
    _spl->inject();

    CCSprite* test_texture = _spl->loadSprite("background.png");
    _textures_size = test_texture->getContentSize().width;
    test_texture->removeFromParent();

    this->setContentSize(CCSize(_textures_size * table.getWidth(),
                                _textures_size * table.getHeight()));


    createNodes();
    createBackground();

    initNodes();

}

void FlowRenderer::updateCell(const FlowPoint cordinates,
                const FlowPointState& new_state)
{
    _nodes_renderers[cordinates.x()][cordinates.y()].update(new_state);
    _table.get(cordinates) = new_state;
}

const cocos2d::ccColor3B& FlowRenderer::getRenderColor(const FlowColor c)
{
    return _colors[c % _colors.size()];
}

const cocos2d::CCSize FlowRenderer::getNodeSize()
{
    return CCSize(_textures_size, _textures_size);
}

const cocos2d::CCPoint FlowRenderer::getNodePosition(const FlowPoint& pos)
{
    float half = _textures_size / 2;
    return CCPoint(half + _textures_size * pos.x(),
                   half + _textures_size * pos.y());
}


void FlowRenderer::createBackground()
{
    unsigned int width = _table.getWidth();
    unsigned int height = _table.getHeight();
    for(unsigned int x=0; x<width; ++x)
    {
        for(unsigned int y=0; y<height; ++y)
        {
            CCSprite* border = _spl->loadSprite("border.png");
            border->setPosition(this->getNodePosition(FlowPoint(x, y)));
        }
    }
}

void FlowRenderer::createNodes()
{
    unsigned int width = _table.getWidth();
    unsigned int height = _table.getHeight();
    for(unsigned int x=0; x<width; ++x)
    {
        for(unsigned int y=0; y<height; ++y)
        {
            FlowRendererNode& node = _nodes_renderers[x][y];
            node.setParent(this);

            FlowPointState* state = &_table(x, y);
            node.setPointState(state);

            CCSprite* highlight = _spl->loadSprite("background.png");
            highlight->setOpacity(80);
            highlight->setPosition(this->getNodePosition(FlowPoint(x, y)));

            node.setHighlightSprite(highlight);
        }
    }
}

void FlowRenderer::initNodes()
{
    unsigned int width = _table.getWidth();
    unsigned int height = _table.getHeight();
    for(unsigned int x=0; x<width; ++x)
    {
        for(unsigned int y=0; y<height; ++y)
        {
            FlowRendererNode& node = _nodes_renderers[x][y];
            node.init();
        }
    }
}

cocos2d::CCSprite* FlowRenderer::createSprite(const char* name)
{
    return _spl->loadSprite(name);
}

FlowRenderer* FlowRenderer::create(const FlowTable& t)
{
    FlowRenderer* r = new FlowRenderer(t);
    if(r)
    {
        r->autorelease();
    }
    return r;
}

FlowRenderer::ColorVec FlowRenderer::_colors=getDefaultColors();
FlowRenderer::ColorVec FlowRenderer::getDefaultColors()
{
    ColorVec v(12);
    v[0] = ccc3(255, 0, 0); //Red
    v[1] = ccc3(0, 0, 255); //Blue
    v[2] = ccc3(29,205,0); //Green
    v[3] = ccc3(246,255,0); //Yellow
    v[4] = ccc3(255,0,216); //Pink
    v[5] = ccc3(255,150,0); //Orange
    v[6] = ccc3(144,0,210); //Violet
    v[7] = ccc3(158,0,0); //Dark red
    v[8] = ccc3(255,255,255); //White
    v[9] = ccc3(0,255,240); //Light blue
    v[10] = ccc3(195,255,91); //Light green
    v[11] = ccc3(1,10,131); //Very dark blue
    return v;
}

const FlowTable& FlowRenderer::getTable() const
{
    return _table;
}

void FlowRenderer::showActiveCircle(const cocos2d::CCPoint& p, const FlowColor color)
{
    if(_active_circle == 0)
    {
        _active_circle = createSprite("circle.png");
        _active_circle->setZOrder(1000);
        _active_circle->setScale(2);
        _active_circle->setOpacity(100);
    }

    _active_circle->setVisible(true);
    _active_circle->setColor(getRenderColor(color));
    _active_circle->setPosition(p);
}

void FlowRenderer::hideActiveCircle()
{
    if(_active_circle != 0)
    {
        _active_circle->setVisible(false);
    }
}
