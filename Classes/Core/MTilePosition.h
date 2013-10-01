#ifndef TILEPOSITION_H
#define TILEPOSITION_H

class MTilePosition
{
public:
    MTilePosition(const unsigned int x, const unsigned int y)
        : _x(x), _y(y)
    {}
    MTilePosition(const MTilePosition& obj)
        : _x(obj._x), _y(obj._y)
    {}
    MTilePosition& operator=(const MTilePosition& obj)
    {
        _x = obj._x;
        _y = obj._y;
        return *this;
    }

    ~MTilePosition()
    {}

    const unsigned int x() const
    {
        return _x;
    }
    const unsigned int y() const
    {
        return _y;
    }
    unsigned int& x()
    {
        return _x;
    }

    unsigned int& y()
    {
        return _y;
    }

private:
    unsigned int _x;
    unsigned int _y;
};

#endif // TILEPOSITION_H
