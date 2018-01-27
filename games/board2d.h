#ifndef BOARD2D_H
#define BOARD2D_H

#include "board.h"

class Board2D : public virtual Board {
 public:
  Board2D(int size_x, int size_y);

  QString MoveToReadableMove(Move move) const;
  Move ReadableMoveToMove(const QString &readable_move, bool &valid) const;

  void GetPossibleMoves(std::vector<Move> &moves) const;
  virtual bool IsMovePossible(Move move) const;

 protected:
  const int size_x_;
  const int size_y_;

  const int num_moves_;
  const int num_positions_;

  Board2D(int size_x, int size_y, int num_moves, int num_positions);
};

#endif  // BOARD2D_H
