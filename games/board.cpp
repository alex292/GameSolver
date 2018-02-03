#include "board.h"

#include <QDebug>

#include "zobristgenerator.h"

Board::Board(int num_positions, int num_moves)
    : num_positions_(num_positions)
    , num_moves_(num_moves)
    , positions_white_(num_positions, false)
    , positions_black_(num_positions, false) {}

void Board::MakeMove(Move move) {
  PositionIndex last_position = PerformMove(move);

  CheckForGameOver(last_position);

  is_turn_white_ = !is_turn_white_;
}
