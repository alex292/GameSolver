#ifndef MONTECARLOTREE_H
#define MONTECARLOTREE_H

#include <QReadWriteLock>

#include "montecarlotreenode.h"
#include "montecarlotreenodecollection.h"

class MonteCarloTree {
 public:
  MonteCarloTree();

  const std::shared_ptr<MonteCarloTreeNode> GetTreeRoot() const { return root_; }

  const std::shared_ptr<MonteCarloTreeNode> Selection();
  const std::shared_ptr<MonteCarloTreeNode> Expansion(const std::shared_ptr<MonteCarloTreeNode> &selected_node);

  Move GetBestMove();
  void SetRoot(const std::shared_ptr<const Board> &board);

 protected:
  std::shared_ptr<MonteCarloTreeNode> root_;
  std::shared_ptr<MonteCarloTreeNodeCollection> node_collection_;
};

#endif  // MONTECARLOTREE_H