#ifndef FLOWPOINT_H
#define FLOWPOINT_H
#include "Core/Binary.h"

typedef unsigned int Cordinate;
class FlowPoint
{
public:
    FlowPoint(const Cordinate x=0, const Cordinate y=0)
        : _x(x), _y(y)
    {}

    const Cordinate x() const
    {
        return _x;
    }
    const Cordinate y() const
    {
        return _y;
    }
private:
    Cordinate _x;
    Cordinate _y;
};

inline bool operator==(const FlowPoint& a, const FlowPoint& b)
{
    return a.x() == b.x() && a.y() == b.y();
}
inline bool operator!=(const FlowPoint& a, const FlowPoint& b)
{
    return !(a == b);
}

inline bool isPointNextTo(const FlowPoint& a, const FlowPoint& b)
{
    if(a.x() == b.x() && (a.y()+1 == b.y() || b.y()+1 == a.y()))
        return true;
    if(a.y() == b.y() && (a.x()+1 == b.x() || b.x()+1 == a.x()))
        return true;
    return false;
}

inline OutputBinaryStream& operator<<(OutputBinaryStream& os, const FlowPoint& a)
{
    //char buffer[2] = {a.x(), a.y()};
    //os.writeBytes(buffer, 2);
    os << uint16_t(a.x()) << uint16_t(a.y());
    return os;
}

inline InputBinaryStream& operator>>(InputBinaryStream& is, FlowPoint& b)
{
    uint16_t x = 0, y = 0;
    is >> x >> y;
    b = FlowPoint(x, y);
    return is;
}

#endif // FLOWPOINT_H
