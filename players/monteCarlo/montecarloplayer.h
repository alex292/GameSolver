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
  MonteCarloPlayer(int time_per_move,
                   int num_threads = QThread::idealThreadCount(),
                   bool use_pondering = false);
  virtual ~MonteCarloPlayer() {}

  Move GetNextMove(Board* board);

 protected:
  const int num_threads_;
  const int time_per_move_;
  const bool use_pondering_;

  MonteCarloTree tree_;
  std::vector<std::unique_ptr<MonteCarloTreeExplorer>> tree_explorers_;
  QDeadlineTimer deadline_timer_;
  QThreadPool thread_pool_;
};

#endif  // MONTECARLOPLAYER_H
