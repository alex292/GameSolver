#ifndef MONTECARLOTREENODECOLLECTION_H
#define MONTECARLOTREENODECOLLECTION_H

#include "montecarlotreenode.h"
class MonteCarloTreeNode;

#include <QReadWriteLock>

class MonteCarloTreeNodeCollection
{
public:
    MonteCarloTreeNodeCollection();

    const std::shared_ptr<MonteCarloTreeNode> createNode(const std::shared_ptr<const Board> &board);
    const std::shared_ptr<MonteCarloTreeNode> getNode(const std::shared_ptr<const Board> &board);
    void removeExpiredNodes();

protected:
    QHash<ZobristValue, std::weak_ptr<MonteCarloTreeNode>> nodes_;
    QReadWriteLock nodesLock_;
};

#endif // MONTECARLOTREENODECOLLECTION_H
