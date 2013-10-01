#ifndef SLIDERSELECTORTILE_H
#define SLIDERSELECTORTILE_H
#include "Core/MTile.h"
#include "Core/GraphicsManager.h"

class SliderSelectorTile : public MTile, public cocos2d::CCTouchDelegate
{
public:
    class Delegate
    {
    public:
        Delegate(){}
        virtual void onValueChanged(const unsigned int){}
        virtual ~Delegate() {}
    };
    typedef std::auto_ptr<Delegate> DelegatePtr;

    typedef unsigned int ValueT;



    static SliderSelectorTile* create(const std::string& title,
                                      const MTilePosition& position,
                                      const MTileSize& size,
                                      const ValueT value,
                                      DelegatePtr delegate=DelegatePtr(0),
                                      const ValueT min_value=0,
                                      const ValueT max_value=100,
                                      const ValueT clamp=1);



    virtual ~SliderSelectorTile();
private:
    SliderSelectorTile();

    void init(const std::string& title, const MTilePosition&,
              const MTileSize&, DelegatePtr, const ValueT value,
              const ValueT min_value, const ValueT max_value, const ValueT clamp);

    void initUI();

    void performCallback();
    void do_performCallback(float);
    void updateSlider();
    void addTicks();
    void addTick(float position, float tick_scale);

    bool updateTouch(cocos2d::CCTouch *touch, bool change_state);
    bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void ccTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);

    ValueT sliderPositionToValue(float x) const;

    unsigned int _value;
    unsigned int _max_value;
    unsigned int _min_value;
    unsigned int _clamp;
    bool _callback_scheduled;
    std::string _title;

    DelegatePtr _delegate;


    cocos2d::CCSize _progress_bar_size;
    cocos2d::CCSize _progress_bar_texture_size;
    cocos2d::CCSprite* _slider;

    static SliderSelectorTile* _touch_lock;
    SpritesLoader _spl;
};



#endif // SLIDERSELECTORTILE_H
