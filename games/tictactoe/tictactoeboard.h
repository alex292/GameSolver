#ifndef TICTACTOEBOARD_H
#define TICTACTOEBOARD_H

#include "games/connectedrowsboard.h"

class TicTacToeBoard : public ConnectedRowsBoard<24, 24, 1> {
 public:
  TicTacToeBoard();

  PositionIndex PerformMove(Move move);

  std::shared_ptr<Board> Copy() const;

 protected:
  static bool initialized_static_data_;

  static std::vector<std::vector<PositionIndex>> rows_to_positions_;
  static std::vector<std::vector<unsigned char>> positions_to_rows_;

  static std::vector<ZobristValue> zobrist_values_positions_white_;
  static std::vector<ZobristValue> zobrist_values_positions_black_;

  void InitializeStaticData();

  const std::vector<PositionIndex> &GetPositionsFromRow(unsigned char row);
  const std::vector<unsigned char> &GetRowsFromPosition(PositionIndex position);
};

#endif  // TICTACTOEBOARD_H
