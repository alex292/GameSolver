#ifndef PLAYER_H
#define PLAYER_H

#include "games/board.h"

#include <memory>

class Player {
 public:
  Player(const std::shared_ptr<const Board> &board);

  virtual Move GetNextMove() = 0;
  virtual void Update(Move move){};

 protected:
  std::shared_ptr<const Board> board_;
};

#endif  // PLAYER_H
