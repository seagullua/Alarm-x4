#ifndef TILESIZE_H
#define TILESIZE_H



class MTileSize
{
public:
    MTileSize(const unsigned int width=0, const unsigned int height=0)
        : _width(width), _height(height)
    {}
    MTileSize(const MTileSize& obj)
        : _width(obj._width), _height(obj._height)
    {}
    MTileSize& operator=(const MTileSize& obj)
    {
        _width = obj._width;
        _height = obj._height;
        return *this;
    }

    ~MTileSize()
    {}

    const unsigned int width() const
    {
        return _width;
    }
    const unsigned int height() const
    {
        return _height;
    }
    unsigned int& width()
    {
        return _width;
    }

    unsigned int& height()
    {
        return _height;
    }

private:
    unsigned int _width;
    unsigned int _height;
};




#endif // TILESIZE_H
