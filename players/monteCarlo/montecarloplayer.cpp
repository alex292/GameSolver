#include "montecarloplayer.h"

#include <QDebug>
#include <QThread>
#include <QTimer>

MonteCarloPlayer::MonteCarloPlayer(int time_per_move,
                                   int num_threads,
                                   bool use_pondering)
    : Player(),
      num_threads_(num_threads),
      time_per_move_(time_per_move),
      use_pondering_(use_pondering) {
  tree_ = std::make_shared<MonteCarloTree>();

  deadline_timer_ = std::make_shared<QDeadlineTimer>();

  for (int i = 0; i < num_threads_; ++i) {
    std::shared_ptr<MonteCarloTreeExplorer> tree_explorer =
        std::make_shared<MonteCarloTreeExplorer>(tree_, deadline_timer_);
    tree_explorer->setAutoDelete(false);
    tree_explorers_.push_back(tree_explorer);
  }
}

Move MonteCarloPlayer::GetNextMove(const std::shared_ptr<const Board>& board) {
  // stop pondering
  if (use_pondering_) {
    deadline_timer_->setRemainingTime(0);
    thread_pool_.waitForDone();
  }

  // explore
  tree_->SetRoot(board);
  deadline_timer_->setRemainingTime(time_per_move_);
  for (const std::shared_ptr<MonteCarloTreeExplorer>& treeExplorer :
       tree_explorers_)
    thread_pool_.start(treeExplorer.get());
  thread_pool_.waitForDone();

  // find best move
  Move best_move = tree_->GetBestMove();

  // make move (update tree root)
  std::shared_ptr<Board> board_copy = board->Copy();
  board_copy->MakeMove(best_move);
  tree_->SetRoot(board_copy);

  // start pondering
  if (use_pondering_) {
    deadline_timer_->setRemainingTime(-1);
    for (const std::shared_ptr<MonteCarloTreeExplorer>& treeExplorer :
         tree_explorers_)
      thread_pool_.start(treeExplorer.get());
  }

  return best_move;
}
