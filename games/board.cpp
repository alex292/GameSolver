#include "board.h"

#include <QDebug>

#include "zobristgenerator.h"

void Board::MakeMove(Move move) {
  PositionIndex last_position = PerformMove(move);

  CheckForGameOver(last_position);

  if (active_color_ == Color::WHITE)
    active_color_ = Color::BLACK;
  else
    active_color_ = Color::WHITE;
}

bool Board::IsWin(Color color) const {
  if (color == Color::WHITE)
    return state_ == GameState::WIN_WHITE;
  else
    return state_ == GameState::WIN_BLACK;
}
