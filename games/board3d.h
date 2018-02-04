#ifndef BOARD3D_H
#define BOARD3D_H

#include "board.h"

#include <bitset>

template <unsigned int X, unsigned int Y, unsigned int Z>
class Board3D : public Board {
 public:
  Board3D();

  QString MoveToReadableMove(Move move) const;
  Move ReadableMoveToMove(const QString &readable_move, bool &valid) const;

  void GetPossibleMoves(std::vector<Move> &moves) const;
  virtual bool IsMovePossible(Move move) const;
  bool HasRemainingMoves();

 protected:
  const static unsigned int num_positions_ = X * Y * Z;
  const static unsigned int num_moves_ = X * Y;

  std::bitset<X * Y * Z> positions_white_;
  std::bitset<X * Y * Z> positions_black_;

  PositionIndex GetResultingPositionIndex(Move move) const;
  PositionIndex MapXYZtoPositionIndex(int x, int y, int z) const;
};

#include "board3d.tpp"

#endif  // BOARD3D_H
