#ifndef CONNECTEDROWSBOARD_H
#define CONNECTEDROWSBOARD_H

#include "board.h"

class ConnectedRowsBoard : public virtual Board {
 public:
  ConnectedRowsBoard();

  void CheckForGameOver(PositionIndex last_position);

 protected:
  void InitConnectedRows();

  virtual void GenerateRowsToPositions() = 0;
  void GeneratePositionsToRows();

  std::shared_ptr<std::vector<std::vector<PositionIndex>>> rows_to_positions_;
  std::shared_ptr<std::vector<std::vector<unsigned char>>> positions_to_rows_;
};

#endif  // CONNECTEDROWSBOARD_H
