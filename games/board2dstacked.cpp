#include "board2dstacked.h"

using namespace std;

Board2DStacked::Board2DStacked(int sizeX, int sizeY, int sizeZ) :
    Board2D(sizeX, sizeY, sizeX*sizeY, sizeX*sizeY*sizeZ),
    sizeZ_(sizeZ)
{
    stackHeights_ = vector<unsigned char>(numMoves_, 0);
}

bool Board2DStacked::isMovePossible(Move move) const
{
    return (stackHeights_[move] < sizeZ_);
}

PositionIndex Board2DStacked::mapXYZtoPositionIndex(int x, int y, int z) const
{
    return (x + y*sizeX_ + z*sizeX_*sizeY_);
}
