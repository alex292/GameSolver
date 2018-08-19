#ifndef MONTECARLOTREE_H
#define MONTECARLOTREE_H

#include <QReadWriteLock>

#include "montecarlotreenode.h"
#include "montecarlotreenodecollection.h"

class MonteCarloTree {
 public:
  MonteCarloTree();

  const std::shared_ptr<MonteCarloTreeNode> GetTreeRoot() const {
    return root_;
  }

  void Selection(std::vector<std::shared_ptr<MonteCarloTreeNode>>& nodes);
  void Expansion(std::vector<std::shared_ptr<MonteCarloTreeNode>>& nodes);

  void SetRoot(std::unique_ptr<const Board> board);

 protected:
  std::shared_ptr<MonteCarloTreeNode> root_;
  std::unique_ptr<MonteCarloTreeNodeCollection> node_collection_;
};

#endif  // MONTECARLOTREE_H
