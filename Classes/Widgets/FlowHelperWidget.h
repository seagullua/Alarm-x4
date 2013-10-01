#ifndef FLOWHELPERWIDGET_H
#define FLOWHELPERWIDGET_H
#include "Tiles/FlowTile.h"
#include "Core/Widget.h"
class FlowHelperWidget : public Widget
{
public:
    enum Mode
    {
        OneTime,
        FreePlay
    };

    typedef FlowGame::Delegate Delegate;
    typedef std::auto_ptr<Delegate> DelegatePtr;

    FlowHelperWidget(DelegatePtr delegate, const Mode mode = OneTime);
private:
    void onInit();
    void launchFlow(MTile* a=0);

    class FlowDelegate;
    void onSolve();
    void onExit();

    FlowTile* _flow_tile;
    DelegatePtr _delegate;
    MTileBlock _flow_space;
    Mode _mode;
};

#endif // FLOWHELPERWIDGET_H
