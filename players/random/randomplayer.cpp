#include "randomplayer.h"

RandomPlayer::RandomPlayer(const std::shared_ptr<const Board> &board)
    : random_engine_(random_device_())
    , Player(board) {}

Move RandomPlayer::GetNextMove() {
  possible_moves_.clear();

  board_->GetPossibleMoves(possible_moves_);

  std::uniform_int_distribution<int> random_index_distribution(0, possible_moves_.size() - 1);
  return possible_moves_[random_index_distribution(random_engine_)];
}
