#include "tictactoeboard.h"

#include <QDebug>

#include <memory>

#include "games/zobristgenerator.h"

bool TicTacToeBoard::initialized_static_data_ = false;
std::vector<std::vector<PositionIndex>> TicTacToeBoard::lines_to_positions_;
std::vector<std::vector<LineIndex>> TicTacToeBoard::positions_to_lines_;
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

std::unique_ptr<Board> TicTacToeBoard::Copy() const {
  return std::make_unique<TicTacToeBoard>(*this);
}

void TicTacToeBoard::InitializeStaticData() {
  // generate lines to positions (for ConnectedLinesBoard)
  lines_to_positions_.push_back({0, 1, 2});  // 0
  lines_to_positions_.push_back({3, 4, 5});  // 1
  lines_to_positions_.push_back({6, 7, 8});  // 2
  lines_to_positions_.push_back({0, 3, 6});  // 3
  lines_to_positions_.push_back({1, 4, 7});  // 4
  lines_to_positions_.push_back({2, 5, 8});  // 5
  lines_to_positions_.push_back({0, 4, 8});  // 6
  lines_to_positions_.push_back({2, 4, 6});  // 7

  // generate positions to line (for ConnectedLinesBoard)
  GeneratePositionsToLinesFromLinesToPositions(lines_to_positions_,
                                               positions_to_lines_);

  // zobrist values
  ZobristGenerator& zobrist_generator = ZobristGenerator::GetInstance();
  zobrist_generator.GenerateUniqueZobristValues(
      num_positions_, zobrist_values_positions_white_);
  zobrist_generator.GenerateUniqueZobristValues(
      num_positions_, zobrist_values_positions_black_);

  // mark as initialized
  initialized_static_data_ = true;
}

const std::vector<PositionIndex>& TicTacToeBoard::GetPositionsForLine(
    LineIndex line) {
  return lines_to_positions_[line];
}

const std::vector<LineIndex>& TicTacToeBoard::GetLinesForPosition(
    PositionIndex position) {
  return positions_to_lines_[position];
}
