#include "montecarlotreenodecollection.h"

#include <QDebug>

using namespace std;

MonteCarloTreeNodeCollection::MonteCarloTreeNodeCollection()
{
    nodes_.reserve(5000);

}

const std::shared_ptr<MonteCarloTreeNode> MonteCarloTreeNodeCollection::createNode(const std::shared_ptr<const Board> &board)
{
    ZobristValue zobristValue = board->getZobristValue();

    QWriteLocker locker(&nodesLock_);

    QHash<ZobristValue, weak_ptr<MonteCarloTreeNode>>::const_iterator i = nodes_.find(zobristValue);
    const weak_ptr<MonteCarloTreeNode> &weakTreeNode = i.value();

    if(i != nodes_.end()) {
        shared_ptr<MonteCarloTreeNode> node = weakTreeNode.lock();
        if(node)
            return node;
    }

    const shared_ptr<MonteCarloTreeNode> node = make_shared<MonteCarloTreeNode>(board);
    nodes_.insert(zobristValue, node);


    if(nodes_.size() % 10000 == 0)
        qDebug() << nodes_.size();

    return node;

}

const std::shared_ptr<MonteCarloTreeNode> MonteCarloTreeNodeCollection::getNode(const shared_ptr<const Board> &board)
{
    ZobristValue zobristValue = board->getZobristValue();

    QReadLocker locker(&nodesLock_);

    QHash<ZobristValue, weak_ptr<MonteCarloTreeNode>>::const_iterator i = nodes_.find(zobristValue);

    const weak_ptr<MonteCarloTreeNode> &weakTreeNode = i.value();

    if(i == nodes_.end() || weakTreeNode.expired()) {
        locker.unlock();
        return createNode(board);
    }

    if(board->getNumMovesMade() != weakTreeNode.lock()->getBoard()->getNumMovesMade())
        qDebug() << "mismatch";

    return weakTreeNode.lock();
}

void MonteCarloTreeNodeCollection::removeExpiredNodes()
{
    QWriteLocker locker(&nodesLock_);

    qDebug() << "numNodes:" << nodes_.size();
    QMutableHashIterator<ZobristValue, weak_ptr<MonteCarloTreeNode>> iter(nodes_);
    while(iter.hasNext()) {
        iter.next();
        weak_ptr<MonteCarloTreeNode> &node = iter.value();
        if(node.expired()) {
            nodes_.remove(iter.key());
        } else {
            node.lock()->removeExpiredParents();
        }
    }
    //qDebug() << "numNodes:" << nodes_.size();
}
