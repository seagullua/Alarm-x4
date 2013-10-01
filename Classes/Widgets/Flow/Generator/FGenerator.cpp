#include "FGenerator.h"
#include <algorithm>

//Numberlinc algorithm
//https://github.com/thomasahle/numberlink/blob/master/src/numberlink/gen.go


Table tile(const Cordinate width, const Cordinate height);

void findFlows(Table&);
FlowTask print(const Table&);

FlowTask generate(const Cordinate width, const Cordinate height)
{
    Table table = tile(width, height);
    findFlows(table);
    return print(table);
}





bool isFlowHead(const unsigned int x, const unsigned int y, const Table& table);
void layFlow(int x, int y, Table& table);

typedef std::vector<unsigned int> Perm;
Perm randPerm(const unsigned int n)
{
    Perm perm(n);
    for(unsigned int i=0; i<n; ++i)
    {
        perm[i]=i;
    }
    std::random_shuffle(perm.begin(), perm.end());
    return perm;
}

void findFlows(Table& table)
{
    unsigned int width = table[0].size();
    unsigned int height = table.size();

    unsigned int all = width*height;
    Perm perm=randPerm(all);

    unsigned int iteration = all;
    for(unsigned int i=0; i<iteration; ++i)
    {
        //unsigned int p = rand() % all;
        unsigned int p = perm[i];
        unsigned int x = p%width;
        unsigned int y = p/width;

        if (isFlowHead(x, y, table))
        {
            layFlow(x, y, table);
        }
    }
}

int DX[4] = {0, 1, 0, -1};
int DY[4] = {-1, 0, 1, 0};

inline bool inside(int x, int y, unsigned int width, unsigned int height)
{
    return 0 <= x && x < int(width) && 0 <= y && y < int(height);
}

bool isFlowHead(const unsigned int x_u, const unsigned int y_u, const Table& table)
{
    int x = int(x_u);
    int y = int(y_u);

    unsigned int width = table[0].size();
    unsigned int height = table.size();

    unsigned int degree = 0;

    for (unsigned int i = 0; i < 4; i++)
    {
        int x1 = x+DX[i];
        int y1 = y+DY[i];

        if (inside(x1, y1, width, height) && table[y1][x1] == table[y][x])
        {
            degree += 1;
        }
    }
    return degree < 2;
}
typedef std::pair<int, int> IntPair;
bool canConnect(int x1, int y1, int x2, int y2, const Table& table);
void fill(int x, int y, TraceID alpha, Table& table);
IntPair follow(int x, int y, int x0, int y0, const Table& table);

void layFlow(int x, int y, Table& table)
{
    unsigned int width = table[0].size();
    unsigned int height = table.size();

    Perm perm=randPerm(4);
    for(unsigned int it=0; it<4; ++it)
    {
        unsigned int i = perm[it];
        int x1 = x+DX[i];
        int y1 = y+DY[i];

        if (inside(x1, y1, width, height) && canConnect(x, y, x1, y1, table))
        {
            fill(x1, y1, table[y][x], table);
            IntPair p2 =  follow(x1, y1, x, y, table);
            layFlow(p2.first, p2.second, table);
            return;
        }
    }
}

bool canConnect(int x1, int y1, int x2, int y2, const Table& table)
{
    int width = table[0].size();
    int height = table.size();

    // Check (x1,y2) and (x2,y2) are flow heads
    if (table[y1][x1] == table[y2][x2])
    {
        return false;
    }
    if (!isFlowHead(x1, y1, table) || !isFlowHead(x2, y2, table))
    {
        return false;
    }

    for (int y3 = 0; y3 < height; y3++)
    {
        for (int x3 = 0; x3 < width; x3++)
        {
            for (unsigned int i = 0; i < 4; i++)
            {
                int x4 = x3+DX[i];
                int y4 = y3+DY[i];

                if (inside(x4, y4, width, height) &&
                        !(x3 == x1 && y3 == y1 && x4 == x2 && y4 == y2) &&
                        table[y3][x3] == table[y1][x1] && table[y4][x4] == table[y2][x2])
                {
                    return false;
                }
            }
        }
    }
    return true;
}

void fill(int x, int y, TraceID alpha, Table& table)
{
    unsigned int width = table[0].size();
    unsigned int height = table.size();
    TraceID orig = table[y][x];
    table[y][x] = alpha;
    for (unsigned int i = 0; i < 4; i++)
    {
        int x1 = x+DX[i];
        int y1 = y+DY[i];

        if (inside(x1, y1, width, height) && table[y1][x1] == orig) {
            fill(x1, y1, alpha, table);
        }
    }
}

IntPair follow(int x, int y, int x0, int y0, const Table& table)
{
    unsigned int width = table[0].size();
    unsigned int height = table.size();
    for (unsigned int i = 0; i < 4; i++)
    {
        int x1 = x+DX[i];
        int y1 = y+DY[i];

        if (inside(x1, y1, width, height) && !(x1 == x0 && y1 == y0) &&
                table[y][x] == table[y1][x1])
        {
            return follow(x1, y1, x, y, table);
        }
    }
    return IntPair(x, y);
}

FlowTask print(const Table& table)
{

    unsigned int width = table[0].size();
    unsigned int height = table.size();

    FlowTaskMap map;

    for (unsigned int y = 0; y < height; y++)
    {
        for (unsigned int x = 0; x < width; x++)
        {

            if (isFlowHead(x, y, table))
            {
                TraceID tid = table[y][x];
                map[tid].push_back(FlowPoint(x, y));
            }
        }
    }

    FlowTask res;
    res.reserve(map.size());

    for(FlowTaskMap::iterator it = map.begin(); it!=map.end(); ++it)
    {
        res.push_back(it->second);
    }

    std::random_shuffle(res.begin(), res.end());
    return res;
}
