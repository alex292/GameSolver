#ifndef BOARD2DSTACKED_H
#define BOARD2DSTACKED_H

#include "board2d.h"

class Board2DStacked : public Board2D {
 public:
  Board2DStacked(int size_x, int size_y, int size_z);

  bool IsMovePossible(Move move) const;

 protected:
  const int size_z_;

  std::vector<unsigned char> stack_heights_;

  PositionIndex MapXYZtoPositionIndex(int x, int y, int z) const;
};

#endif  // BOARD2DSTACKED_H
