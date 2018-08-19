#ifndef HUMANPLAYER_H
#define HUMANPLAYER_H

#include "players/player.h"

class HumanPlayer : public Player {
 public:
  HumanPlayer();
  virtual ~HumanPlayer() {}

  Move GetNextMove(Board* board);
};

#endif  // HUMANPLAYER_H
