#include "connectedrowsboard.h"

#include <QDebug>

template <unsigned int X, unsigned int Y, unsigned int Z>
void ConnectedRowsBoard<X, Y, Z>::CheckForGameOver(PositionIndex last_position) {
  // check for win
  PositionValue desired_value = (is_turn_white_) ? POSITION_VALUE_WHITE : POSITION_VALUE_BLACK;
  for (unsigned char row : GetRowsFromPosition(last_position)) {
    const std::vector<PositionIndex> &row_positions = GetPositionsFromRow(row);

    bool all_desired = true;
    for (int i = 0; i < row_positions.size(); ++i) {
      if (positions_[row_positions[i]] != desired_value) {
        all_desired = false;
        break;
      }
    }

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

template <unsigned int X, unsigned int Y, unsigned int Z>
void ConnectedRowsBoard<X, Y, Z>::GeneratePositionsToRowsFromRowsToPositions(const std::vector<std::vector<PositionIndex>> &rows_to_positions, std::vector<std::vector<unsigned char>> &positions_to_rows) {
  positions_to_rows.clear();
  positions_to_rows.resize(num_positions_);
  for (int row = 0; row < rows_to_positions.size(); ++row) {
    for (PositionIndex position_index : rows_to_positions[row]) {
      positions_to_rows[position_index].push_back(row);
    }
  }
}
