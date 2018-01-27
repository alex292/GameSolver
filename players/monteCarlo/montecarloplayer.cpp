#include "montecarloplayer.h"

#include <QDebug>
#include <QThread>
#include <QTimer>

MonteCarloPlayer::MonteCarloPlayer(const std::shared_ptr<const Board> &board, int time_per_move, int num_threads)
    : Player(board)
    , time_per_move_(time_per_move)
    , num_threads_(num_threads) {
  tree_ = std::make_shared<MonteCarloTree>(board);

  deadline_timer_ = std::make_shared<QDeadlineTimer>();

  for (int i = 0; i < num_threads_; ++i) {
    std::shared_ptr<MonteCarloTreeExplorer> tree_explorer = std::make_shared<MonteCarloTreeExplorer>(tree_, deadline_timer_);
    tree_explorer->setAutoDelete(false);
    tree_explorers_.push_back(tree_explorer);
  }
}

Move MonteCarloPlayer::GetNextMove() {
  deadline_timer_->setRemainingTime(time_per_move_);

  for (const std::shared_ptr<MonteCarloTreeExplorer> &treeExplorer : tree_explorers_)
    thread_pool_.start(treeExplorer.get());

  thread_pool_.waitForDone();

  Move best_move = tree_->GetBestMove();

  return best_move;
}

void MonteCarloPlayer::Update(Move move) {
  tree_->Update(move);
}
