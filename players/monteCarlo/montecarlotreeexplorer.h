#ifndef MONTECARLOTREEEXPLORER_H
#define MONTECARLOTREEEXPLORER_H

#include <QDeadlineTimer>
#include <QRunnable>

#include "montecarlotree.h"
#include "players/random/randomplayer.h"

class MonteCarloTreeExplorer : public QRunnable {
 public:
  MonteCarloTreeExplorer(MonteCarloTree* tree,
                         const QDeadlineTimer* deadline_timer);

  void run();

 protected:
  MonteCarloTree* tree_;
  const QDeadlineTimer* deadline_timer_;

  RandomPlayer random_player_;

  std::unique_ptr<const Board> RandomPlayout(
      const std::shared_ptr<const MonteCarloTreeNode>& node);

  void BackpropagateResult(
      const std::vector<std::shared_ptr<MonteCarloTreeNode>>& node_path,
      const Board* playout_board);
};

#endif  // MONTECARLOTREEEXPLORER_H
