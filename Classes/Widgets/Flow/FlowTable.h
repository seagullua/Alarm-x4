#ifndef FLOWTABLE_H
#define FLOWTABLE_H
#include "FlowPointPair.h"
#include "FlowPointState.h"
#include <vector>

class FlowTable
{
public:
    FlowTable(const unsigned int width,
              const unsigned int height);

    void addColor(const FlowPoint first,
                  const FlowPoint second);

    const unsigned int getWidth() const;
    const unsigned int getHeight() const;
    const unsigned int getColorsNumber() const;

    const FlowPointState& operator()(const Cordinate x,
                                     const Cordinate y) const;
    FlowPointState& operator()(const Cordinate x,
                               const Cordinate y);

    const FlowPointState& get(const FlowPoint& p) const;
    FlowPointState& get(const FlowPoint& p);
private:
    unsigned int _width;
    unsigned int _height;
    std::vector<FlowPointPair> _colors;

    typedef std::vector<FlowPointState> Row;
    typedef std::vector<Row> Table;

    Table _table;
};

#endif // FLOWTABLE_H
