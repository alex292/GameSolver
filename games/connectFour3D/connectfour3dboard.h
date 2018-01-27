#ifndef CONNECTFOUR3DBOARD_H
#define CONNECTFOUR3DBOARD_H

#include "games/board2dstacked.h"
#include "games/connectedrowsboard.h"

class ConnectFour3DBoard : public Board2DStacked, public ConnectedRowsBoard {
 public:
  ConnectFour3DBoard();

  PositionIndex PerformMove(Move move);

  std::shared_ptr<Board> Copy() const;

 protected:
  void GenerateRowsToPositions();

  std::shared_ptr<std::vector<ZobristValue>> zobrist_values_positions_white_;
  std::shared_ptr<std::vector<ZobristValue>> zobrist_values_positions_black_;
};

#endif  // CONNECTFOUR3DBOARD_H
