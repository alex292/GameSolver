#ifndef MONTECARLOTREEEXPLORER_H
#define MONTECARLOTREEEXPLORER_H

#include <QDeadlineTimer>
#include <QRunnable>

#include "montecarlotree.h"
#include "players/random/randomplayer.h"

class MonteCarloTreeExplorer : public QRunnable {
 public:
  MonteCarloTreeExplorer(const std::shared_ptr<MonteCarloTree> &tree, const std::shared_ptr<const QDeadlineTimer> &deadline_timer);

  void run();

 protected:
  const std::shared_ptr<const QDeadlineTimer> deadline_timer_;

  std::shared_ptr<MonteCarloTree> tree_;

  RandomPlayer random_player_;

  const std::shared_ptr<const Board> RandomPlayout(const std::shared_ptr<const MonteCarloTreeNode> &node);

  void BackpropagateResult(const std::vector<const std::shared_ptr<MonteCarloTreeNode>> &node_path, const std::shared_ptr<const Board> &playout_board);
};

#endif  // MONTECARLOTREEEXPLORER_H
