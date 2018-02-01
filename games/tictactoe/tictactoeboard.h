#ifndef TICTACTOEBOARD_H
#define TICTACTOEBOARD_H

#include "games/board2d.h"
#include "games/connectedrowsboard.h"

class TicTacToeBoard : public Board2D, public ConnectedRowsBoard {
 public:
  TicTacToeBoard();

  PositionIndex PerformMove(Move move);

  std::shared_ptr<Board> Copy() const;

 protected:
  void GenerateRowsToPositions();

  std::shared_ptr<std::vector<ZobristValue>> zobrist_values_positions_white_;
  std::shared_ptr<std::vector<ZobristValue>> zobrist_values_positions_black_;
};

#endif  // TICTACTOEBOARD_H
