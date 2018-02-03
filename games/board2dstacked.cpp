#include "board2dstacked.h"

Board2DStacked::Board2DStacked(int size_x, int size_y, int size_z)
    : Board(size_x * size_y * size_z, size_x * size_y)
    , Board2D(size_x, size_y)
    , size_z_(size_z) {
  stack_heights_ = std::vector<unsigned char>(num_moves_, 0);
}

bool Board2DStacked::IsMovePossible(Move move) const {
  return (stack_heights_[move] < size_z_);
}

PositionIndex Board2DStacked::MapXYZtoPositionIndex(int x, int y, int z) const {
  return (x + y * size_x_ + z * size_x_ * size_y_);
}
