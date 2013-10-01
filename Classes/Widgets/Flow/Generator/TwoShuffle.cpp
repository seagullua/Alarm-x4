#include "FGenerator.h"
#ifdef USE_TWO_SHUFFLE

void shuffle(Table&);
void oddCorner(Table&);

Table tile(const Cordinate width, const Cordinate height)
{
    Table table(height, TRow(width));

    // Start with simple vertical tiling
    TraceID alpha =0;
    for(unsigned int y=0; y<height-1; y+=2)
    {
        for(unsigned int x = 0; x<width; ++x)
        {
            table[y][x] = alpha;
            table[y+1][x] = alpha;
            alpha++;
        }
    }

    // Add padding in case of odd height
    if (height%2 == 1)
    {
        for(unsigned int x = 0; x<width-1; x+=2)
        {

            table[height-1][x] = alpha;
            table[height-1][x+1] = alpha;
            alpha++;
        }

        // In case of odd width, add a single in the corner.
        // We will merge it into a real flow after shuffeling
        if (width%2 == 1)
        {
            table[height-1][width-1] = alpha;
        }
    }

    shuffle(table);
    return table;
}

void shuffle(Table& table)
{
    unsigned int width = table[0].size();
    unsigned int height = table.size();

    if (width == 1 || height == 1)
    {
        return;
    }

    unsigned int times = width*width*height*height;
    for (unsigned int i = 0; i < times; ++i)
    {
        unsigned int x = rand() % (width-1);
        unsigned int y = rand() % (height-1);

        if (table[y][x] == table[y][x+1] && table[y+1][x] == table[y+1][x+1])
        {
            // Horizontal case
            // aa \ ab
            // bb / ab
            table[y+1][x] = table[y][x];
            table[y][x+1] = table[y+1][x+1];

        }
        else if (table[y][x] == table[y+1][x] && table[y][x+1] == table[y+1][x+1])
        {
            // Vertical case
            // ab \ aa
            // ab / bb
            table[y][x+1] = table[y][x];
            table[y+1][x] = table[y+1][x+1];
        }
    }
    oddCorner(table);
}

void oddCorner(Table& table)
{
    unsigned int width = table[0].size();
    unsigned int height = table.size();
    if (width%2 == 1 && height%2 == 1)
    {
        // Horizontal case:
        // aax
        if (width > 2 && table[height-1][width-3] == table[height-1][width-2])
        {
            table[height-1][width-1] = table[height-1][width-2];
        }
        // Vertical case:
        // ab
        // ax
        if (height > 2 && table[height-3][width-1] == table[height-2][width-1])
        {
            table[height-1][width-1] = table[height-2][width-1];
        }
    }
}

#endif
