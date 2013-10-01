#ifndef TIMEWIDGET_H
#define TIMEWIDGET_H
#include "Core/Widget.h"
#include <ctime>
class TimeWidget : public Widget
{
public:
    TimeWidget(Anchor::Horizontal x_anchor = Anchor::Left,
               Anchor::Vertical y_anchor = Anchor::Top);
private:
    void onInit();


    Anchor::Horizontal _x_anchor;
    Anchor::Vertical _y_anchor;

};

#endif // TIMEWIDGET_H
