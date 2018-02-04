#ifndef CONNECTEDROWSBOARD_H
#define CONNECTEDROWSBOARD_H

#include "board3d.h"

template <unsigned int X, unsigned int Y, unsigned int Z>
class ConnectedRowsBoard : public Board3D<X, Y, Z> {
 public:
  ConnectedRowsBoard() {}

  void CheckForGameOver(PositionIndex last_position);

 protected:
  virtual const std::vector<PositionIndex> &GetPositionsFromRow(unsigned char row) = 0;
  virtual const std::vector<unsigned char> &GetRowsFromPosition(PositionIndex position) = 0;

  void GeneratePositionsToRowsFromRowsToPositions(const std::vector<std::vector<PositionIndex>> &rows_to_positions_, std::vector<std::vector<unsigned char>> &positions_to_rows);

  /*virtual void GenerateRowsToPositions() = 0;
  void GeneratePositionsToRows();

  std::shared_ptr<std::vector<std::vector<PositionIndex>>> rows_to_positions_;
  std::shared_ptr<std::vector<std::vector<unsigned char>>> positions_to_rows_;*/

  using Board3D<X, Y, Z>::positions_;
  using Board3D<X, Y, Z>::num_positions_;
  using Board3D<X, Y, Z>::HasRemainingMoves;
  using Board::is_game_over_;
  using Board::is_turn_white_;
  using Board::is_win_black_;
  using Board::is_win_white_;
};

#include "connectedrowsboard.tpp"

#endif  // CONNECTEDROWSBOARD_H
