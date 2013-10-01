#include "FGenerator.h"
#ifdef THREE_BLOCK_SHUFFLE
typedef std::vector<Table> TableVec;

TableVec getRandTableVecs()
{
    TableVec res(4);

    {
        Table t(3, TRow(3,0));
        t[1][0] = 1;
        t[1][1] = 2;
        t[1][2] = 2;
        t[2][0] = 1;
        t[2][1] = 1;
        t[2][2] = 2;

        res[0] = t;
    }

    {
        Table t(3, TRow(3,0));
        t[0][2] = 1;
        t[1][1] = 1;
        t[1][2] = 1;

        res[1] = t;
    }

    {
        Table t(3, TRow(3,0));
        t[0][0] = 1;
        t[1][1] = 1;
        t[1][0] = 1;

        res[2] = t;
    }

    {
        Table t(3, TRow(3,0));
        t[1][0] = 1;
        t[1][1] = 1;
        t[1][2] = 2;
        t[2][0] = 1;
        t[2][1] = 2;
        t[2][2] = 2;

        res[3] = t;
    }

    return res;

}

Table rotate(const Table& t, const unsigned int r)
{
    if(r == 0)
        return t;

    unsigned int width = t[0].size();
    unsigned int height = t.size();

    Table res(height, TRow(width));
    for(unsigned int y=0; y<3; ++y)
    {
        for(unsigned int x=0; x<3; ++x)
        {
            unsigned int x_n = x;
            unsigned int y_n = y;

            if(r == 1 || r == 2)
            {
                x_n = width - x - 1;
            }
            if(r == 2 || r == 3)
            {
                y_n = height - y - 1;
            }

            res[y_n][x_n] = t[y][x];
        }
    }
    return res;
}

Table getThreeTable(const TraceID id)
{
    static const TableVec tables = getRandTableVecs();

    Table res = tables[rand() % tables.size()];
    for(unsigned int y=0; y<3; ++y)
    {
        for(unsigned int x=0; x<3; ++x)
        {
            res[y][x] += id;
        }
    }
    return rotate(res, rand() % 4);
}

void combine(Table& table, const Table& add, unsigned int x_s, unsigned int y_s)
{
    unsigned int width = table[0].size();
    unsigned int height = table.size();

    unsigned int p_width = add[0].size();
    unsigned int p_height = add.size();

    for(unsigned int y=0; y<p_height && y+y_s < height; ++y)
    {
        for(unsigned int x=0; x<p_width && x+x_s < width; ++x)
        {
            table[y+y_s][x+x_s] = add[y][x];
        }
    }
}

//Three rows
Table tile(const Cordinate width, const Cordinate height)
{
    Table table(height, TRow(width));

    // Start with simple vertical tiling
    TraceID alpha =0;
    for(unsigned int y=2; y<height; y+=3)
    {
        for(unsigned int x = 2; x<width; x+=3)
        {
            combine(table, getThreeTable(alpha), x-2, y-2);
            alpha+=5;
        }
    }
    return table;
}
#endif
