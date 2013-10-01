#ifndef LABELFACTORY_H
#define LABELFACTORY_H
#include "cocos2d.h"

class LabelFactory
{
public:
    static const float BIG_FONT;
    static const float HUGE_FONT;
    static const float MEDIUM_FONT;
    static const float SMALL_FONT;
    static const char* const DEFAULT_FONT;

    static cocos2d::CCLabelTTF* label(const char* input, float font_size)
    {
        float scaled = cocos2d::CCDirector::sharedDirector()->getContentScaleFactor();
        return cocos2d::CCLabelTTF::create(input, DEFAULT_FONT, font_size / scaled);
    }

    static cocos2d::CCLabelTTF* label(const char* input, float font_size,
                            const cocos2d::CCSize& dimensions, cocos2d::CCTextAlignment hAlignment,
                                      cocos2d::CCVerticalTextAlignment vAlignment = cocos2d::kCCVerticalTextAlignmentTop)
    {
        float scaled = cocos2d::CCDirector::sharedDirector()->getContentScaleFactor();
        return cocos2d::CCLabelTTF::create(input, DEFAULT_FONT, font_size / scaled, dimensions, hAlignment, vAlignment);
    }

    static float vfix(float font_size)
    {
        float scaled = cocos2d::CCDirector::sharedDirector()->getContentScaleFactor();
        return - font_size * _v_fix / scaled;
    }

private:
    static const float _v_fix;
};
typedef LabelFactory LF;
#endif // LABELFACTORY_H
