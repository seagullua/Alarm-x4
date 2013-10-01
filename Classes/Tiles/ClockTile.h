#ifndef CLOCKTILE_H
#define CLOCKTILE_H
#include "Core/MTile.h"
#include "Logic/Time.h"



class ClockTile : public MTile
{
public:
    enum Style
    {
        Small,
        Big
    };

    ClockTile();
    static ClockTile* create(const std::string& text, const Time time,
                             const MTilePosition& position,
                             const MTileSize& size,
                             const Style style=Big,
                             const std::string& color="blue.png");

    void setTime(const Time time);

private:
    void updateTime();
    void init(const std::string& text, const Time time,
              const MTilePosition& position, const MTileSize& size,
              const Style style, const std::string& color);

    cocos2d::CCLabelTTF* _clock;

    std::string _text;
    Hour _hours;
    Minute _minutes;
};

#endif // CLOCKTILE_H
