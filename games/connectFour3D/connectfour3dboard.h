#ifndef CONNECTFOUR3DBOARD_H
#define CONNECTFOUR3DBOARD_H

#include "games/connectedlinesboard.h"

class ConnectFour3DBoard : public ConnectedLinesBoard<4, 4, 4> {
 public:
  ConnectFour3DBoard();

  PositionIndex PerformMove(Move move);

  std::shared_ptr<Board> Copy() const;

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

#endif  // CONNECTFOUR3DBOARD_H
