#include "montecarlotreenodecollection.h"

#include <QDebug>

MonteCarloTreeNodeCollection::MonteCarloTreeNodeCollection() {
  nodes_.reserve(5000);
}

const std::shared_ptr<MonteCarloTreeNode> MonteCarloTreeNodeCollection::CreateNode(const std::shared_ptr<const Board> &board) {
  ZobristValue zobrist_value = board->GetZobristValue();

  QWriteLocker locker(&nodes_lock_);

  QHash<ZobristValue, std::weak_ptr<MonteCarloTreeNode>>::const_iterator i = nodes_.find(zobrist_value);
  const std::weak_ptr<MonteCarloTreeNode> &weakTreeNode = i.value();

  if (i != nodes_.end()) {
    std::shared_ptr<MonteCarloTreeNode> node = weakTreeNode.lock();
    if (node)
      return node;
  }

  const std::shared_ptr<MonteCarloTreeNode> node = std::make_shared<MonteCarloTreeNode>(board);
  nodes_.insert(zobrist_value, node);

  if (nodes_.size() % 10000 == 0)
    qDebug() << nodes_.size();

  return node;
}

const std::shared_ptr<MonteCarloTreeNode> MonteCarloTreeNodeCollection::GetNode(const std::shared_ptr<const Board> &board) {
  ZobristValue zobrist_value = board->GetZobristValue();

  QReadLocker locker(&nodes_lock_);

  QHash<ZobristValue, std::weak_ptr<MonteCarloTreeNode>>::const_iterator i = nodes_.find(zobrist_value);

  const std::weak_ptr<MonteCarloTreeNode> &weak_tree_node = i.value();

  if (i == nodes_.end() || weak_tree_node.expired()) {
    locker.unlock();
    return CreateNode(board);
  }

  if (board->GetNumMovesMade() != weak_tree_node.lock()->GetBoard()->GetNumMovesMade())
    qDebug() << "mismatch";

  return weak_tree_node.lock();
}

void MonteCarloTreeNodeCollection::RemoveExpiredNodes() {
  QWriteLocker locker(&nodes_lock_);

  qDebug() << "numNodes:" << nodes_.size();
  QMutableHashIterator<ZobristValue, std::weak_ptr<MonteCarloTreeNode>> iter(nodes_);
  while (iter.hasNext()) {
    iter.next();
    std::weak_ptr<MonteCarloTreeNode> &node = iter.value();
    if (node.expired())
      nodes_.remove(iter.key());
    else
      node.lock()->RemoveExpiredParents();
  }
  // qDebug() << "numNodes:" << nodes_.size();
}
