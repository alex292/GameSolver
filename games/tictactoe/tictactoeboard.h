#ifndef TICTACTOEBOARD_H
#define TICTACTOEBOARD_H

#include "games/connectedlinesboard.h"

class TicTacToeBoard : public ConnectedLinesBoard<3, 3, 1> {
 public:
  TicTacToeBoard();

  PositionIndex PerformMove(Move move);

  std::unique_ptr<Board> Copy() const;

 protected:
  static bool initialized_static_data_;

  static std::vector<std::vector<PositionIndex>> lines_to_positions_;
  static std::vector<std::vector<LineIndex>> positions_to_lines_;

  static std::vector<ZobristValue> zobrist_values_positions_white_;
  static std::vector<ZobristValue> zobrist_values_positions_black_;

  void InitializeStaticData();

  const std::vector<PositionIndex>& GetPositionsForLine(LineIndex line);
  const std::vector<LineIndex>& GetLinesForPosition(PositionIndex position);
};

#endif  // TICTACTOEBOARD_H
