#include "connectedlinesboard.h"

#include <QDebug>

template <unsigned int X, unsigned int Y, unsigned int Z>
void ConnectedLinesBoard<X, Y, Z>::CheckForGameOver(
    PositionIndex last_position) {
  // check for win
  std::bitset<X* Y* Z>& positions =
      (is_turn_white_) ? positions_white_ : positions_black_;
  for (LineIndex line : GetLinesForPosition(last_position)) {
    const std::vector<PositionIndex>& line_positions =
        GetPositionsForLine(line);

    bool all_desired = true;
    for (int i = 0; i < line_positions.size(); ++i) {
      if (!positions[line_positions[i]]) {
        all_desired = false;
        break;
      }
    }

    if (!all_desired)
      continue;

    is_game_over_ = true;
    is_win_white_ = is_turn_white_;
    is_win_black_ = !is_turn_white_;
    return;
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
void ConnectedLinesBoard<X, Y, Z>::GeneratePositionsToLinesFromLinesToPositions(
    const std::vector<std::vector<PositionIndex>>& lines_to_positions,
    std::vector<std::vector<LineIndex>>& positions_to_lines) {
  positions_to_lines.clear();
  positions_to_lines.resize(num_positions_);
  for (LineIndex line = 0; line < lines_to_positions.size(); ++line) {
    for (PositionIndex position_index : lines_to_positions[line]) {
      positions_to_lines[position_index].push_back(line);
    }
  }
}
