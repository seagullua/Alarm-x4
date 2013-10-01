#include "SliderSelectorTile.h"
#include "Core/LabelFactory.h"
using namespace cocos2d;
SliderSelectorTile::SliderSelectorTile()
    : _callback_scheduled(false)
{
}

SliderSelectorTile* SliderSelectorTile::create(const std::string& title, const MTilePosition& position,
                                               const MTileSize& size, const ValueT value, DelegatePtr delegate,
                                               const ValueT min_value, const ValueT max_value, const ValueT clamp)
{
    SliderSelectorTile* obj = new SliderSelectorTile();
    if(obj)
    {
        obj->autorelease();
        obj->init(title, position, size, delegate, value, min_value, max_value, clamp);
    }
    return obj;
}

void SliderSelectorTile::init(const std::string& title, const MTilePosition& position,
                              const MTileSize& size, DelegatePtr delegate, const ValueT value,
                              const ValueT min_value, const ValueT max_value, const ValueT clamp)
{
    _title = title;
    _delegate = delegate;
    _min_value = min_value;
    _max_value =max_value;
    _value=value;
    _clamp=clamp;
    _callback_scheduled = false;



    //Reserve scpace
    MTile::init("violet.png", position, size,0,0);



    //Draw UI elements
    initUI();

    //Start listening for touch
    CCDirector* pDirector = CCDirector::sharedDirector();
    pDirector->getTouchDispatcher()->addTargetedDelegate(this, kCCMenuHandlerPriority, false);


}

SliderSelectorTile::~SliderSelectorTile()
{
    //Unregister touch delegate
    CCDirector* pDirector = CCDirector::sharedDirector();
    pDirector->getTouchDispatcher()->removeDelegate(this);
}

void SliderSelectorTile::initUI()
{
    //Add slider texture
    _slider = CCSprite::create("red.png");
    _slider->setAnchorPoint(ccp(0, 1));

    _slider->setScaleX(0);
    this->addChild(_slider, Aligment::HNone, Aligment::VNone, 0, 0);

    //Scale texture to fit the tile
    _progress_bar_size = this->getContentSize();
    _progress_bar_texture_size = _slider->getContentSize();
    _slider->setScaleY(_progress_bar_size.height /
                       _progress_bar_texture_size.height);


    //Add title label
    this->addChild(LF::label(_title.c_str(), LF::SMALL_FONT),
                   Aligment::Left ,Aligment::VCenter,
                   0, LF::vfix(LF::SMALL_FONT));

    _spl = GraphicsManager::getLoaderFor(this, "tick.plist", "tick.png");
    _spl->inject();

    addTicks();

    updateSlider();
}
void SliderSelectorTile::do_performCallback(float)
{
    _callback_scheduled = false;
    if(_delegate.get() != 0)
    {
        _delegate->onValueChanged(_value);
    }
}

void SliderSelectorTile::performCallback()
{
    if(!_callback_scheduled)
    {
        _callback_scheduled = true;
        const float RESPONCE_TIME = 0.3f;
        this->scheduleOnce(schedule_selector(SliderSelectorTile::do_performCallback),
                           RESPONCE_TIME);
    }
}
void SliderSelectorTile::updateSlider()
{
    if(this->isVisible())
    {
        //Calculate the percents of the slider
        float percents = float(_value) / _max_value;

        float full_scale = _progress_bar_size.width /
                _progress_bar_texture_size.width;
        float scale = percents * full_scale;

        //If scales differs
        if(scale != _slider->getScaleX())
        {
            //Animate toward new scale
            const float ANIMATION_TIME = 0.2f;
            _slider->stopAllActions();
            _slider->runAction(
                        CCScaleTo::create(ANIMATION_TIME,
                                          scale,
                                          _slider->getScaleY()));
        }
    }
}

SliderSelectorTile::ValueT SliderSelectorTile::sliderPositionToValue(float x) const
{
    float width = _progress_bar_size.width;

    float percents = _max_value * x / width;
    if(percents < 0)
        percents = 0;
    if(percents > _max_value)
        percents = _max_value;

    ValueT new_value = static_cast<unsigned int>(
                percents/float(_clamp)  + 0.5f);
    new_value *= _clamp;
    return new_value;
}

bool SliderSelectorTile::updateTouch(cocos2d::CCTouch *touch, bool change_state)
{
    bool res = false;
    //Check if widget is visible and we the lock is free
    if(this->isVisible() && (change_state || _touch_lock == this || _touch_lock == 0))
    {
        //Get touch point in local cordinates
        CCPoint touchLocation = touch->getLocation();
        CCPoint local_cords = this->convertToNodeSpace(touchLocation);

        CCRect r = this->rect();
        r.origin = CCPointZero;

        //If no other touch is performed and the touch is inside my block
        if (change_state && r.containsPoint(local_cords))
        {
            //Lock the touch
            _touch_lock = this;
            res = true;
        }

        //If the touch is locked by me update values
        if(_touch_lock == this)
        {
            //Parse new value
            ValueT new_value = sliderPositionToValue(local_cords.x);

            //Update it if needed
            if(new_value != _value)
            {
                _value = new_value;
                updateSlider();
                performCallback();
            }
        }
    }
    return res;
}

bool SliderSelectorTile::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *)
{
    return updateTouch(pTouch, true);
}

void SliderSelectorTile::ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *)
{
    updateTouch(pTouch, false);
}
void SliderSelectorTile::ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *)
{
    updateTouch(pTouch, false);
    _touch_lock = 0;
}

void SliderSelectorTile::ccTouchCancelled(cocos2d::CCTouch *, cocos2d::CCEvent *)
{
    _touch_lock = 0;
}

void SliderSelectorTile::addTick(float position, float tick_scale)
{
    //CCSprite* tick = CCSprite::create("tick.png");

    //this->addChild(tick, Aligment::HNone, Aligment::VNone, 0, 0);

    CCSprite* tick = _spl->loadSprite("tick.png");
    tick->setPosition(ccp(position, 0));
    tick->setAnchorPoint(ccp(0.5f, 0));
    tick->setScale(tick_scale);
}

void SliderSelectorTile::addTicks()
{
    unsigned int units = (_max_value - _min_value) / _clamp;
    float unit_size = _progress_bar_size.width / units;


    float normal_tick = 1;
    float smalled_tick = 0.6f;

    for(unsigned int i=1; i<units; ++i)
    {
        float sickness = 0.3f;

        if(i * 2 == units)
            sickness = normal_tick;
        if(i * 4 == units || i * 4 / 3 == units)
            sickness = smalled_tick;


        addTick(i*unit_size, sickness);
    }

}

SliderSelectorTile* SliderSelectorTile::_touch_lock = 0;
