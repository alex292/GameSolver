#include "board2d.h"

Board2D::Board2D(int size_x, int size_y)
    : Board2D(size_x, size_y, size_x * size_y, size_x * size_y) {}

Board2D::Board2D(int size_x, int size_y, int num_moves, int num_positions)
    : size_x_(size_x)
    , size_y_(size_y)
    , num_moves_(num_moves)
    , num_positions_(num_positions) {
  positions_ = std::vector<PositionValue>(num_positions, POSITION_VALUE_FREE);
}

QString Board2D::MoveToReadableMove(Move move) const {
  int y = move / size_x_;
  int x = move % size_x_;

  return ((char)'A' + x) + QString::number(y + 1);
}

Move Board2D::ReadableMoveToMove(const QString &readable_move, bool &valid) const {
  int x = (readable_move.at(0).toUpper().toLatin1() - 'A');
  int y = readable_move.mid(1).toInt() - 1;

  valid = (x < 0 || x >= size_x_ || y < 0 || y >= size_y_);

  return y * size_x_ + x;
}

void Board2D::GetPossibleMoves(std::vector<Move> &moves) const {
  if (is_game_over_)
    return;

  for (int i = 0; i < num_moves_; ++i) {
    if (IsMovePossible(i))
      moves.push_back(i);
  }
}

bool Board2D::IsMovePossible(Move move) const {
  return (positions_[move] == POSITION_VALUE_FREE);
}
