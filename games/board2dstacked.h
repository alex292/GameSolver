#ifndef BOARD2DSTACKED_H
#define BOARD2DSTACKED_H

#include "board2d.h"

class Board2DStacked : public Board2D
{
public:
    Board2DStacked(int sizeX, int sizeY, int sizeZ);

    bool isMovePossible(Move move) const;

protected:
    const int sizeZ_;

    std::vector<unsigned char> stackHeights_;

    PositionIndex mapXYZtoPositionIndex(int x, int y, int z) const;
};

#endif // BOARD2DSTACKED_H
