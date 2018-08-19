#ifndef CONNECTEDLINESBOARD_H
#define CONNECTEDLINESBOARD_H

#include "board3d.h"

template <unsigned int X, unsigned int Y, unsigned int Z>
class ConnectedLinesBoard : public Board3D<X, Y, Z> {
 public:
  ConnectedLinesBoard() {}

  void CheckForGameOver(PositionIndex last_position);

  using Board3D<X, Y, Z>::GetPositionValue;

 protected:
  virtual const std::vector<PositionIndex>& GetPositionsForLine(
      LineIndex line) = 0;
  virtual const std::vector<LineIndex>& GetLinesForPosition(
      PositionIndex position) = 0;

  void GeneratePositionsToLinesFromLinesToPositions(
      const std::vector<std::vector<PositionIndex>>& lines_to_positions_,
      std::vector<std::vector<LineIndex>>& positions_to_lines);

  using Board3D<X, Y, Z>::num_positions_;
  using Board3D<X, Y, Z>::HasRemainingMoves;
  using Board::active_color_;
  using Board::state_;
};

#include "connectedlinesboard.tpp"

#endif  // CONNECTEDLINESBOARD_H
