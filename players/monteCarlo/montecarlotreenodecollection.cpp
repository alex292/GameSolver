#include "montecarlotreenodecollection.h"

#include <QDebug>
#include <QtConcurrent>

const std::shared_ptr<MonteCarloTreeNode>
MonteCarloTreeNodeCollection::CreateNode(std::unique_ptr<const Board> board) {
  ZobristValue zobrist_value = board->zobrist_value();

  QWriteLocker locker(&nodes_lock_);

  if (nodes_.contains(zobrist_value))
    return nodes_[zobrist_value].lock();

  const std::shared_ptr<MonteCarloTreeNode> node =
      std::make_shared<MonteCarloTreeNode>(std::move(board));
  nodes_.insert(zobrist_value, node);

  return node;
}

const std::shared_ptr<MonteCarloTreeNode> MonteCarloTreeNodeCollection::GetNode(
    std::unique_ptr<const Board> board) {
  ZobristValue zobrist_value = board->zobrist_value();

  QReadLocker locker(&nodes_lock_);

  if (!nodes_.contains(zobrist_value)) {
    locker.unlock();
    return CreateNode(std::move(board));
  }

  return nodes_[zobrist_value].lock();
}

void MonteCarloTreeNodeCollection::RemoveExpiredNodes() {
  QWriteLocker locker(&nodes_lock_);

  QElapsedTimer timer;
  timer.start();

  int num_before = nodes_.size();
  QMutableHashIterator<ZobristValue, std::weak_ptr<MonteCarloTreeNode>> iter(
      nodes_);
  while (iter.hasNext()) {
    iter.next();
    std::weak_ptr<MonteCarloTreeNode>& node = iter.value();
    if (node.expired())
      iter.remove();
    else
      node.lock()->RemoveExpiredParents();
  }

  qDebug() << "\tnodes:" << num_before << "->" << nodes_.size()
           << "(cleanup time:" << timer.elapsed() << "ms)";
}
