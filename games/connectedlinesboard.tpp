#include "connectedlinesboard.h"

#include <QDebug>

template <unsigned int X, unsigned int Y, unsigned int Z>
void ConnectedLinesBoard<X, Y, Z>::CheckForGameOver(
    PositionIndex last_position) {
  // check for win
  for (LineIndex line : GetLinesForPosition(last_position)) {
    const std::vector<PositionIndex>& line_positions =
        GetPositionsForLine(line);

    bool all_desired = true;
    PositionValue desired_value = active_color_ == Color::WHITE
                                      ? PositionValue::WHITE
                                      : PositionValue::BLACK;
    for (int i = 0; i < line_positions.size(); ++i) {
      if (GetPositionValue(line_positions[i]) != desired_value) {
        all_desired = false;
        break;
      }
    }

    if (!all_desired)
      continue;

    if (active_color_ == Color::WHITE)
      state_ = GameState::WIN_WHITE;
    else
      state_ = GameState::WIN_BLACK;

    return;
  }

  // check for tie
  if (!HasRemainingMoves())
    state_ = GameState::TIE;
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
