#ifndef MONTECARLOTREENODECOLLECTION_H
#define MONTECARLOTREENODECOLLECTION_H

#include <QReadWriteLock>

#include "montecarlotreenode.h"
class MonteCarloTreeNode;

class MonteCarloTreeNodeCollection {
 public:
  MonteCarloTreeNodeCollection();

  const std::shared_ptr<MonteCarloTreeNode> CreateNode(const std::shared_ptr<const Board> &board);
  const std::shared_ptr<MonteCarloTreeNode> GetNode(const std::shared_ptr<const Board> &board);
  void RemoveExpiredNodes();

 protected:
  QReadWriteLock nodes_lock_;
  QHash<ZobristValue, std::weak_ptr<MonteCarloTreeNode>> nodes_;
};

#endif  // MONTECARLOTREENODECOLLECTION_H
