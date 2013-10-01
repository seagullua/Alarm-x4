#ifndef FLOWWIDGET_H
#define FLOWWIDGET_H
#include "Core/Widget.h"
#include "FlowGame.h"
class FlowWidget : public Widget
{
public:
    FlowWidget(FlowGame::DelegatePtr delegate);
private:
    void onInit();
    void onSolve();

    FlowGame::DelegatePtr _delegate;
};

#endif // FLOWWIDGET_H
