#ifndef HUMANPLAYER_H
#define HUMANPLAYER_H

#include "players/player.h"

class HumanPlayer : public Player {
 public:
  HumanPlayer();

  Move GetNextMove(const std::shared_ptr<const Board> &board);
};

#endif  // HUMANPLAYER_H
