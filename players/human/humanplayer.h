#ifndef HUMANPLAYER_H
#define HUMANPLAYER_H

#include "players/player.h"

class HumanPlayer : public Player {
 public:
  HumanPlayer(const std::shared_ptr<const Board> &board);

  Move GetNextMove();
};

#endif  // HUMANPLAYER_H
