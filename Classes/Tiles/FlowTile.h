#ifndef FLOWTILE_H
#define FLOWTILE_H
#include "Core/MTile.h"
#include "Widgets/Flow/FlowGame.h"
class FlowTile : public MTile
{
public:

    static FlowTile* create(const MTilePosition& position,
                             const MTileSize& size, FlowGame::DelegatePtr);

private:
    FlowTile();
    void init(const MTilePosition&pos, const MTileSize& size, FlowGame::DelegatePtr d);
};

#endif // FLOWTILE_H
