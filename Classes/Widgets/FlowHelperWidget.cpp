#include "FlowHelperWidget.h"
#include "Tiles/FlowTile.h"
FlowHelperWidget::FlowHelperWidget(DelegatePtr delegate, const Mode mode)
    : _delegate(delegate), _flow_tile(0), _mode(mode)
{
}

class FlowHelperWidget::FlowDelegate : public FlowGame::Delegate
{
public:
    FlowDelegate(FlowHelperWidget* parent)
        : _parent(parent)
    {}

    void onWin()
    {
        _parent->onSolve();
    }

private:
    FlowHelperWidget* _parent;
};

void FlowHelperWidget::onInit()
{
    //Flow widget initialization
    MTileSize s = MTileManager::sharedManager().getGrid().getSize();

    unsigned int size = MIN(s.height(), s.width() - 6);
    _flow_space = getApi()->reserveSpace(
                Anchor::Left, Anchor::Top,
                MTileSize(size, size));

    launchFlow();



    //Help initialization
    unsigned int help_space_width = MIN(8, s.width() - _flow_space.getSize().width());
    MTileBlock help_space = getApi()->reserveSpace(
                Anchor::Right, Anchor::Top,
                MTileSize(help_space_width, 3));
    MTile* help = MTile::create("violet.png",help_space.getPosition(),
                                help_space.getSize());
    std::string help_text;

    if(_mode == OneTime)
    {
        help_text = R::tr(R::FlowHelperWidget_TurnOffRules);
    }
    else
    {
        help_text = R::tr(R::FlowHelperWidget_PlayRules);
    }

    addImageAndLabelToTile(help, help_text.c_str(), "help.png");


    getApi()->addTile(help);

    //Restart button
    MTileBlock replay_space = getApi()->reserveSpace(
                Anchor::Right, Anchor::Top,
                MTileSize(5,2));
    MTile* replay = MTile::create("blue.png",replay_space.getPosition(),
                                  replay_space.getSize(),
                                  this,tile_selector(FlowHelperWidget::launchFlow));
    addImageAndLabelToTile(replay,
                           R::tr(R::FlowHelperWidget_Restart)
                           , "restart.png" );
    getApi()->addTile(replay);

    //Exit button
    if(_mode == FreePlay)
    {

        MTileBlock exit_space = getApi()->reserveSpace(
                    Anchor::Right, Anchor::Top,
                    MTileSize(5,2));
        MTile* exit_tile = MTile::create("blue.png",exit_space.getPosition(),
                                      exit_space.getSize(),
                                      this,tile_selector(FlowHelperWidget::onExit));

        addImageAndLabelToTile(exit_tile,
                               R::tr(R::FlowHelperWidget_Exit)
                               , "back.png" );
        getApi()->addTile(exit_tile);
    }

}

void FlowHelperWidget::onExit()
{
    if(_delegate.get() && _mode == FreePlay)
    {
        _delegate->onWin();
    }
}

void FlowHelperWidget::onSolve()
{
    if(_delegate.get() && _mode == OneTime)
    {
        _delegate->onWin();
    }
    else if(_mode == FreePlay)
    {
        launchFlow();
    }
}

void FlowHelperWidget::launchFlow(MTile *)
{
    if(_flow_tile != 0)
    {
        getApi()->removeTile(_flow_tile);
    }

    _flow_tile = FlowTile::create(_flow_space.getPosition(),
                                  _flow_space.getSize(),
                                  FlowGame::DelegatePtr(new FlowDelegate(this)));

    getApi()->addTile(_flow_tile);
}
