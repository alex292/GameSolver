#include "connectedrowsboard.h"

ConnectedRowsBoard::ConnectedRowsBoard() {}

void ConnectedRowsBoard::CheckForGameOver(PositionIndex last_position) {
  // check for win
  PositionValue desired_position_value = (is_turn_white_) ? POSITION_VALUE_WHITE : POSITION_VALUE_BLACK;
  for (unsigned char row : positions_to_rows_->at(last_position)) {
    const std::vector<PositionIndex> &row_positions = rows_to_positions_->at(row);

    bool all_desired = true;
    for (int i = 0; i < row_positions.size() && all_desired; ++i)
      all_desired = (positions_[row_positions[i]] == desired_position_value);

    if (all_desired) {
      is_game_over_ = true;
      is_win_white_ = is_turn_white_;
      is_win_black_ = !is_turn_white_;
      return;
    }
  }

  // check for tie
  if (!HasRemainingMoves()) {
    is_game_over_ = true;
    is_win_white_ = false;
    is_win_black_ = false;
    return;
  }
}

void ConnectedRowsBoard::InitConnectedRows() {
  GenerateRowsToPositions();
  GeneratePositionsToRows();
}

void ConnectedRowsBoard::GeneratePositionsToRows() {
  positions_to_rows_ = std::make_shared<std::vector<std::vector<unsigned char>>>(positions_.size());

  for (int row = 0; row < rows_to_positions_->size(); ++row) {
    for (PositionIndex positionIndex : rows_to_positions_->at(row)) {
      positions_to_rows_->at(positionIndex).push_back(row);
    }
  }
}
