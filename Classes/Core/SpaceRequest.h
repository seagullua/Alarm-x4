#ifndef SPACEREQUEST_H
#define SPACEREQUEST_H
#include "MTileSize.h"
#include "Anchor.h"
class SpaceRequest
{
public:
    SpaceRequest(const Anchor::Horizontal horizontal,
                 const Anchor::Vertical vertical,
                 const MTileSize& max_size,
                 const MTileSize& min_size=MTileSize(0,0))
        :
          _max_size(max_size),
          _min_size(min_size),
          _horizontal(horizontal),
          _vertical(vertical)
    {}



    const MTileSize& getMaxSize() const
    {
        return _max_size;
    }

    const MTileSize& getMinSize() const
    {
        return _min_size;
    }
    const Anchor::Horizontal getHorizontalAnchor() const
    {
        return _horizontal;
    }

    const Anchor::Vertical getVerticalAnchor() const
    {
        return _vertical;
    }
private:
    MTileSize _max_size;
    MTileSize _min_size;
    Anchor::Horizontal _horizontal;
    Anchor::Vertical _vertical;
};

#endif // SPACEREQUEST_H
