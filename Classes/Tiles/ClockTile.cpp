#include "ClockTile.h"
#include "Core/LabelFactory.h"

using namespace cocos2d;
ClockTile::ClockTile()
    : _clock(0)
{
}

ClockTile* ClockTile::create(const std::string& text,
                             const Time time,
                             const MTilePosition& position,
                             const MTileSize& size, const Style style, const std::string &color)

{
    ClockTile* obj = new ClockTile();
    if(obj)
    {
        obj->autorelease();
        obj->init(text, time, position, size,style, color);
    }
    return obj;
}

void ClockTile::setTime(const Time time)
{
    _hours = time.getHour();
    _minutes = time.getMinute();
    updateTime();
}


void ClockTile::updateTime()
{
    std::stringstream ss;
    unsigned int hours = _hours % 24;
    unsigned int minutes = _minutes % 60;

    if(hours < 10)
        ss << '0';
    ss << hours << ':';
    if(minutes < 10)
        ss << '0';
    ss << minutes;

    _clock->setString(ss.str().c_str());
}

void ClockTile::init(const std::string& text,
                     const Time time,
                     const MTilePosition &position,
                     const MTileSize &size,
                     const Style style,
                     const std::string &color)
{
    _hours = time.getHour();
    _minutes = time.getMinute();
    _text = text;

    MTile::init(color.c_str(), position,size,0,0);

    CCSize tile_size = this->getInnerContentSize();

    //Calculate font sizes
    float font_size = LF::MEDIUM_FONT;
    float time_font = LF::BIG_FONT * 2;
    float text_width = tile_size.width/5*2;
    if(style==Small)
    {
        font_size = LF::MEDIUM_FONT*0.7;
        time_font = LF::BIG_FONT * 1.2;
        text_width = tile_size.width*0.6;
    }

    //Add title label
    CCLabelTTF* label = LF::label(_text.c_str(),
                                  font_size,
                                  CCSize(text_width, 0),
                                  kCCTextAlignmentCenter);
    this->addChild(label,
                   Aligment::Left,Aligment::VCenter,
                   0,LF::vfix(font_size));

    //Add clock label
    _clock = LF::label("",
                       time_font);
    this->addChild(_clock,
                   Aligment::Right, Aligment::VCenter,
                   0, LF::vfix(time_font) / 2);

    updateTime();

}
