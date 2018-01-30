#ifndef MONTECARLOPLAYER_H
#define MONTECARLOPLAYER_H

#include <QHash>
#include <QReadWriteLock>
#include <QThreadPool>

#include "montecarlotree.h"
#include "montecarlotreeexplorer.h"
#include "players/player.h"

class MonteCarloPlayer : public Player {
 public:
  MonteCarloPlayer(int time_per_move, int num_threads = QThread::idealThreadCount());

  Move GetNextMove(const std::shared_ptr<const Board> &board);

 protected:
  const int num_threads_;
  const int time_per_move_;

  std::shared_ptr<MonteCarloTree> tree_;

  std::vector<std::shared_ptr<MonteCarloTreeExplorer>> tree_explorers_;

  std::shared_ptr<QDeadlineTimer> deadline_timer_;

  QThreadPool thread_pool_;
};

#endif  // MONTECARLOPLAYER_H
