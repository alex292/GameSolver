#ifndef RANDOMPLAYER_H
#define RANDOMPLAYER_H

#include "players/player.h"

#include <random>
#include <vector>

class RandomPlayer : public Player {
 public:
  RandomPlayer();

  Move GetNextMove(const std::shared_ptr<const Board> &board);

 protected:
  std::random_device random_device_;
  std::mt19937 random_engine_;

  std::vector<Move> possible_moves_;
};

#endif  // RANDOMPLAYER_H
