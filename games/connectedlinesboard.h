#ifndef CONNECTEDLINESBOARD_H
#define CONNECTEDLINESBOARD_H

#include "board3d.h"

template <unsigned int X, unsigned int Y, unsigned int Z>
class ConnectedLinesBoard : public Board3D<X, Y, Z> {
 public:
  ConnectedLinesBoard() {}

  void CheckForGameOver(PositionIndex last_position);

 protected:
  virtual const std::vector<PositionIndex>& GetPositionsForLine(
      LineIndex line) = 0;
  virtual const std::vector<LineIndex>& GetLinesForPosition(
      PositionIndex position) = 0;

  void GeneratePositionsToLinesFromLinesToPositions(
      const std::vector<std::vector<PositionIndex>>& lines_to_positions_,
      std::vector<std::vector<LineIndex>>& positions_to_lines);

  using Board3D<X, Y, Z>::positions_white_;
  using Board3D<X, Y, Z>::positions_black_;
  using Board3D<X, Y, Z>::num_positions_;
  using Board3D<X, Y, Z>::HasRemainingMoves;
  using Board::is_game_over_;
  using Board::is_turn_white_;
  using Board::is_win_black_;
  using Board::is_win_white_;
};

#include "connectedlinesboard.tpp"

#endif  // CONNECTEDLINESBOARD_H
