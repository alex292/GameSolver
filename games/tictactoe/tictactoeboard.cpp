#include "tictactoeboard.h"

#include <QDebug>

#include "games/zobristgenerator.h"

bool TicTacToeBoard::initialized_static_data_ = false;
std::vector<std::vector<PositionIndex>> TicTacToeBoard::rows_to_positions_;
std::vector<std::vector<unsigned char>> TicTacToeBoard::positions_to_rows_;
std::vector<ZobristValue> TicTacToeBoard::zobrist_values_positions_white_;
std::vector<ZobristValue> TicTacToeBoard::zobrist_values_positions_black_;

TicTacToeBoard::TicTacToeBoard() {
  if (!initialized_static_data_)
    InitializeStaticData();
}

PositionIndex TicTacToeBoard::PerformMove(Move move) {
  PositionIndex position = GetResultingPositionIndex(move);

  // set position
  if (is_turn_white_) {
    positions_white_[position] = true;
    zobrist_value_ ^= zobrist_values_positions_white_[position];
  } else {
    positions_black_[position] = true;
    zobrist_value_ ^= zobrist_values_positions_black_[position];
  }

  return position;
}

std::shared_ptr<Board> TicTacToeBoard::Copy() const {
  return std::make_shared<TicTacToeBoard>(*this);
}

void TicTacToeBoard::InitializeStaticData() {
  // generate rows to positions (for ConnectedRowsBoard)
  rows_to_positions_.push_back({0, 1, 2});  // 0
  rows_to_positions_.push_back({3, 4, 5});  // 1
  rows_to_positions_.push_back({6, 7, 8});  // 2
  rows_to_positions_.push_back({0, 3, 6});  // 3
  rows_to_positions_.push_back({1, 4, 7});  // 4
  rows_to_positions_.push_back({2, 5, 8});  // 5
  rows_to_positions_.push_back({0, 4, 8});  // 6
  rows_to_positions_.push_back({2, 4, 6});  // 7

  // generate positions to rows (for ConnectedRowsBoard)
  GeneratePositionsToRowsFromRowsToPositions(rows_to_positions_, positions_to_rows_);

  // zobrist values
  ZobristGenerator &zobrist_generator = ZobristGenerator::GetInstance();
  zobrist_generator.GenerateUniqueZobristValues(num_positions_, zobrist_values_positions_white_);
  zobrist_generator.GenerateUniqueZobristValues(num_positions_, zobrist_values_positions_black_);

  // mark as initialized
  initialized_static_data_ = true;
}

const std::vector<PositionIndex> &TicTacToeBoard::GetPositionsFromRow(unsigned char row) {
  return rows_to_positions_[row];
}

const std::vector<unsigned char> &TicTacToeBoard::GetRowsFromPosition(PositionIndex position) {
  return positions_to_rows_[position];
}
