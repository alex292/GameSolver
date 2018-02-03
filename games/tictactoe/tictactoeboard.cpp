#include "tictactoeboard.h"

#include <QDebug>

#include "games/zobristgenerator.h"

TicTacToeBoard::TicTacToeBoard()
    : Board(9, 9)
    , Board2D(3, 3)
    , ConnectedRowsBoard() {
  // zobrist values
  ZobristGenerator &zobrist_generator = ZobristGenerator::GetInstance();
  zobrist_values_positions_white_ = zobrist_generator.GenerateUniqueZobristValues(num_positions_);
  zobrist_values_positions_black_ = zobrist_generator.GenerateUniqueZobristValues(num_positions_);

  // connected rows
  InitConnectedRows();
}

PositionIndex TicTacToeBoard::PerformMove(Move move) {
  PositionIndex position = move;

  // set position
  if (is_turn_white_) {
    positions_white_[move] = true;
    zobrist_value_ ^= zobrist_values_positions_white_->at(position);
  } else {
    positions_black_[move] = true;
    zobrist_value_ ^= zobrist_values_positions_black_->at(position);
  }

  return position;
}

std::shared_ptr<Board> TicTacToeBoard::Copy() const {
  return std::make_shared<TicTacToeBoard>(*this);
}

void TicTacToeBoard::GenerateRowsToPositions() {
  rows_to_positions_ = std::make_shared<std::vector<std::vector<PositionIndex>>>();
  rows_to_positions_->push_back({0, 1, 2});  // 0
  rows_to_positions_->push_back({3, 4, 5});  // 1
  rows_to_positions_->push_back({6, 7, 8});  // 2
  rows_to_positions_->push_back({0, 3, 6});  // 3
  rows_to_positions_->push_back({1, 4, 7});  // 4
  rows_to_positions_->push_back({2, 5, 8});  // 5
  rows_to_positions_->push_back({0, 4, 8});  // 6
  rows_to_positions_->push_back({2, 4, 6});  // 7
}
