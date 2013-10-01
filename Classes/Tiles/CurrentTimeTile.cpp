#include "CurrentTimeTile.h"

#include "Core/LabelFactory.h"

#include "CurrentTimeTile.h"
#include <sstream>
using namespace cocos2d;

CurrentTimeTile::CurrentTimeTile()
    :

      _main_tile(0),
      _upper_label(0),
      _hour_label(0),
      _scheduled(false),
      _last_day(0),
      _last_minute(0)
{
}

CurrentTimeTile* CurrentTimeTile::create(
        const MTilePosition& position,
        const MTileSize& size)

{
    CurrentTimeTile* obj = new CurrentTimeTile();
    if(obj)
    {
        obj->autorelease();
        obj->init(position, size);
    }
    return obj;
}


void CurrentTimeTile::onInit()
{
    _main_tile = 0;
    _hour_label = 0;
    _upper_label = 0;

    time_t cur_time = time(0);
    tm* time = localtime(&cur_time);
    rebuildAll(time);
}

void CurrentTimeTile::scheduleUpdate()
{
    if(!_scheduled)
    {
        _scheduled = true;
        CCDirector::sharedDirector()->getScheduler()->scheduleSelector(
                    schedule_selector(CurrentTimeTile::tickUpdate),
                    this,
                    1.0f, kCCRepeatForever, 0, false);
    }
}

void CurrentTimeTile::unscheduleUpdate()
{
    if(_scheduled)
    {
        _scheduled = false;
        CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(
                    schedule_selector(CurrentTimeTile::tickUpdate),
                    this);
    }
}





void CurrentTimeTile::rebuildAll(tm* time)
{

    if(_main_tile == 0)
    {
        //Create tile
        _main_tile = this;
    }


    std::string date_str = R::f().formatDate(time->tm_year, time->tm_mon, time->tm_mday, time->tm_wday);

    _last_day = time->tm_mday;

    if(_upper_label)
    {
        _upper_label->removeFromParent();
        _upper_label = 0;
    }

    _upper_label = LF::label(date_str.c_str(), LF::MEDIUM_FONT);
    _main_tile->addChild(_upper_label,
                         Aligment::HCenter, Aligment::Top, 0,
                         LF::vfix(LF::MEDIUM_FONT));

    float max_content_width = _main_tile->getInnerContentSize().width;
    float title_size = _upper_label->getContentSize().width;

    if(title_size > max_content_width)
    {
        _upper_label->setScale(max_content_width / title_size);
    }

    if(_hour_label)
    {
        _hour_label->removeFromParent();
        _hour_label = 0;
    }
    _hour_label = LF::label("", LF::HUGE_FONT);
    _main_tile->addChild(_hour_label, Aligment::HNone, Aligment::VNone);
    _hour_label->setAnchorPoint(ccp(0.5f, 0.5f));
    CCSize tile_size = _main_tile->getContentSize();
    _hour_label->setPosition(ccp(tile_size.width/2,
                                 tile_size.height/2 - _upper_label->getContentSize().height/5));

    rebuildClock(time);
    scheduleUpdate();

}

void CurrentTimeTile::rebuildClock(tm* time)
{
    std::stringstream left_str;
    left_str << (time->tm_hour)<<':';
    if(time->tm_min < 10)
        left_str << '0' << time->tm_min;
    else
        left_str << time->tm_min;

    _hour_label->setString(left_str.str().c_str());
    _last_minute = time->tm_min;
}


void CurrentTimeTile::tickUpdate(float)
{
    time_t cur_time = time(0);
    tm* time = localtime(&cur_time);

    if(_last_day != time->tm_mday)
    {
        rebuildAll(time);
    }
    else if(_last_minute != time->tm_min)
    {
        rebuildClock(time);
    }

}

void CurrentTimeTile::init(const MTilePosition& pos, const MTileSize& size)
{
    MTile::init("green.png", pos, size, 0,0);
    this->onInit();
}

