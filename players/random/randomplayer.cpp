#include "randomplayer.h"

RandomPlayer::RandomPlayer() : Player(), random_engine_(random_device_()) {}

Move RandomPlayer::GetNextMove(Board* board) {
  possible_moves_.clear();

  board->GetPossibleMoves(possible_moves_);

  std::uniform_int_distribution<size_t> random_index_distribution(
      0, possible_moves_.size() - 1);
  return possible_moves_[random_index_distribution(random_engine_)];
}
