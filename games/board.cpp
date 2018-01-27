#include "board.h"

#include <QDebug>

Board::Board() {}

void Board::MakeMove(Move move) {
  move_sequence_.push_back(move);

  PositionIndex last_position = PerformMove(move);
  CheckForGameOver(last_position);

  is_turn_white_ = !is_turn_white_;
}
