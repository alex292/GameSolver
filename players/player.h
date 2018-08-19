#ifndef PLAYER_H
#define PLAYER_H

#include "games/board.h"

#include <memory>

class Player {
 public:
  Player();
  virtual ~Player() {}

  virtual Move GetNextMove(const std::shared_ptr<const Board>& board) = 0;
};

#endif  // PLAYER_H
