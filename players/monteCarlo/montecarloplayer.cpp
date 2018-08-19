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
  for (int i = 0; i < num_threads_; ++i) {
    std::unique_ptr<MonteCarloTreeExplorer> tree_explorer =
        std::make_unique<MonteCarloTreeExplorer>(&tree_, &deadline_timer_);
    tree_explorer->setAutoDelete(false);
    tree_explorers_.push_back(std::move(tree_explorer));
  }
}

Move MonteCarloPlayer::GetNextMove(Board* board) {
  // stop pondering
  if (use_pondering_) {
    deadline_timer_.setRemainingTime(0);
    thread_pool_.waitForDone();
  }

  // explore
  tree_.SetRoot(board->Copy());
  deadline_timer_.setRemainingTime(time_per_move_);
  for (const std::unique_ptr<MonteCarloTreeExplorer>& tree_explorer :
       tree_explorers_)
    thread_pool_.start(tree_explorer.get());
  thread_pool_.waitForDone();

  // find best move
  Move best_move = tree_.GetTreeRoot()->GetBestMove();

  // make move (update tree root)
  std::unique_ptr<Board> board_copy = board->Copy();
  board_copy->MakeMove(best_move);
  tree_.SetRoot(std::move(board_copy));

  // start pondering
  if (use_pondering_) {
    deadline_timer_.setRemainingTime(-1);
    for (const std::unique_ptr<MonteCarloTreeExplorer>& tree_explorer :
         tree_explorers_)
      thread_pool_.start(tree_explorer.get());
  }

  return best_move;
}
