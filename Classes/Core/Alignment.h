#ifndef ALIGNMENT_H
#define ALIGNMENT_H
class Aligment
{
public:
    enum Vertical
    {
        Top = 8,
        Bottom = 16,
        VCenter = 32,
        VNone = 0
    };

    enum Horizontal
    {
        Left = 1,
        Right = 4,
        HCenter = 2,
        HNone = 0
    };
};

#endif // ALIGNMENT_H
