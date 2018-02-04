#include "board.h"

#include <QDebug>

#include "zobristgenerator.h"

void Board::MakeMove(Move move) {
  PositionIndex last_position = PerformMove(move);

  CheckForGameOver(last_position);

  is_turn_white_ = !is_turn_white_;
}
