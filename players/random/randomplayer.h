#ifndef RANDOMPLAYER_H
#define RANDOMPLAYER_H

#include "players/player.h"

#include <random>
#include <vector>

class RandomPlayer : public Player
{
public:
    RandomPlayer(const std::shared_ptr<const Board> &board);

    Move getNextMove();

protected:
    std::random_device randomDevice_;
    std::mt19937 randomEngine_;

    std::vector<Move> possibleMoves_;
};

#endif // RANDOMPLAYER_H
