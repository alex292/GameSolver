#include "board3d.h"

template <unsigned int X, unsigned int Y, unsigned int Z>
const unsigned int Board3D<X, Y, Z>::num_positions_;

template <unsigned int X, unsigned int Y, unsigned int Z>
const unsigned int Board3D<X, Y, Z>::num_moves_;

template <unsigned int X, unsigned int Y, unsigned int Z>
Board3D<X, Y, Z>::Board3D() : positions_white_(), positions_black_() {}

template <unsigned int X, unsigned int Y, unsigned int Z>
QString Board3D<X, Y, Z>::MoveToReadableMove(Move move) const {
  int y = move / X;
  int x = move % Y;

  return ('A' + static_cast<char>(x)) + QString::number(y + 1);
}

template <unsigned int X, unsigned int Y, unsigned int Z>
Move Board3D<X, Y, Z>::ReadableMoveToMove(const QString& readable_move,
                                          bool& valid) const {
  int x = (readable_move.at(0).toUpper().toLatin1() - 'A');
  int y = readable_move.mid(1).toInt() - 1;

  valid = (x < 0 || x >= X || y < 0 || y >= Y);

  return y * X + x;
}

template <unsigned int X, unsigned int Y, unsigned int Z>
void Board3D<X, Y, Z>::GetPossibleMoves(std::vector<Move>& moves) const {
  if (IsGameOver())
    return;

  for (Move i = 0; i < num_moves_; ++i) {
    if (IsMovePossible(i))
      moves.push_back(i);
  }
}

template <unsigned int X, unsigned int Y, unsigned int Z>
bool Board3D<X, Y, Z>::IsMovePossible(Move move) const {
  if (Z == 1)
    return (!positions_white_[move] && !positions_black_[move]);

  for (int z = 0; z < Z; ++z) {
    PositionIndex p = move + z * X * Y;
    if (!positions_white_[p] && !positions_black_[p])
      return true;
  }
  return false;
}

template <unsigned int X, unsigned int Y, unsigned int Z>
bool Board3D<X, Y, Z>::HasRemainingMoves() {
  if (IsGameOver())
    return false;

  for (int i = 0; i < num_moves_; ++i) {
    if (IsMovePossible(i))
      return true;
  }

  return false;
}

template <unsigned int X, unsigned int Y, unsigned int Z>
PositionValue Board3D<X, Y, Z>::GetPositionValue(PositionIndex position) const {
  if (positions_white_[position])
    return PositionValue::WHITE;
  if (positions_black_[position])
    return PositionValue::BLACK;
  return PositionValue::FREE;
}

template <unsigned int X, unsigned int Y, unsigned int Z>
void Board3D<X, Y, Z>::SetPosition(PositionIndex position) {
  if (active_color_ == Color::WHITE)
    positions_white_[position] = true;
  else
    positions_black_[position] = true;
}

template <unsigned int X, unsigned int Y, unsigned int Z>
PositionIndex Board3D<X, Y, Z>::GetResultingPositionIndex(Move move) const {
  if (Z == 1)
    return move;

  for (int z = 0; z < Z; ++z) {
    PositionIndex p = move + z * X * Y;
    if (!positions_white_[p] && !positions_black_[p])
      return p;
  }

  return std::numeric_limits<PositionIndex>::max();
}

template <unsigned int X, unsigned int Y, unsigned int Z>
PositionIndex Board3D<X, Y, Z>::MapXYZtoPositionIndex(int x,
                                                      int y,
                                                      int z) const {
  return (x + y * X + z * X * Y);
}
