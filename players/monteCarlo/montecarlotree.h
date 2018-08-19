#ifndef MONTECARLOTREE_H
#define MONTECARLOTREE_H

#include <QReadWriteLock>

#include "montecarlotreenode.h"
#include "montecarlotreenodecollection.h"

class MonteCarloTree {
 public:
  MonteCarloTree() {}

  const std::shared_ptr<MonteCarloTreeNode> root() const { return root_; }
  void SetRoot(std::unique_ptr<const Board> board);

  const std::shared_ptr<MonteCarloTreeNode> GetNode(
      std::unique_ptr<const Board> board);

 protected:
  std::shared_ptr<MonteCarloTreeNode> root_;
  MonteCarloTreeNodeCollection node_collection_;
};

#endif  // MONTECARLOTREE_H
