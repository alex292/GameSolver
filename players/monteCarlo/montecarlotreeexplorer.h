#ifndef MONTECARLOTREEEXPLORER_H
#define MONTECARLOTREEEXPLORER_H

#include <QDeadlineTimer>
#include <QRunnable>

#include "montecarlotree.h"

class MonteCarloTreeExplorer : public QRunnable {
 public:
  MonteCarloTreeExplorer(const std::shared_ptr<MonteCarloTree> &tree, const std::shared_ptr<const QDeadlineTimer> &deadline_timer);

  void run();

 protected:
  const std::shared_ptr<const QDeadlineTimer> deadline_timer_;

  std::shared_ptr<MonteCarloTree> tree_;
};

#endif  // MONTECARLOTREEEXPLORER_H
