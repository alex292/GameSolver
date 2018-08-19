#ifndef MONTECARLOTREENODECOLLECTION_H
#define MONTECARLOTREENODECOLLECTION_H

#include <QReadWriteLock>

#include "montecarlotreenode.h"

class MonteCarloTreeNodeCollection {
 public:
  MonteCarloTreeNodeCollection() {}

  const std::shared_ptr<MonteCarloTreeNode> CreateNode(
      std::unique_ptr<const Board> board);
  const std::shared_ptr<MonteCarloTreeNode> GetNode(
      std::unique_ptr<const Board> board);
  void RemoveExpiredNodes();

 protected:
  QReadWriteLock nodes_lock_;
  QHash<ZobristValue, std::weak_ptr<MonteCarloTreeNode>> nodes_;
};

#endif  // MONTECARLOTREENODECOLLECTION_H
