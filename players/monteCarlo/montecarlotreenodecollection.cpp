#include "montecarlotreenodecollection.h"

#include <QDebug>
#include <QtConcurrent>

MonteCarloTreeNodeCollection::MonteCarloTreeNodeCollection() {
  // nodes_.reserve(1000000);
}

const std::shared_ptr<MonteCarloTreeNode> MonteCarloTreeNodeCollection::CreateNode(const std::shared_ptr<const Board> &board) {
  ZobristValue zobrist_value = board->GetZobristValue();

  QWriteLocker locker(&nodes_lock_);

  if (nodes_.contains(zobrist_value))
    return nodes_[zobrist_value].lock();

  const std::shared_ptr<MonteCarloTreeNode> node = std::make_shared<MonteCarloTreeNode>(board);
  nodes_.insert(zobrist_value, node);

  /*if (nodes_.size() % 100000 == 0)
    qDebug() << "nodes:" << nodes_.size();*/

  return node;
}

const std::shared_ptr<MonteCarloTreeNode> MonteCarloTreeNodeCollection::GetNode(const std::shared_ptr<const Board> &board) {
  ZobristValue zobrist_value = board->GetZobristValue();

  QReadLocker locker(&nodes_lock_);

  if (!nodes_.contains(zobrist_value)) {
    locker.unlock();
    return CreateNode(board);
  }

  return nodes_[zobrist_value].lock();
}

void MonteCarloTreeNodeCollection::RemoveExpiredNodes() {
  QWriteLocker locker(&nodes_lock_);

  QElapsedTimer timer;
  timer.start();

  int num_before = nodes_.size();
  QMutableHashIterator<ZobristValue, std::weak_ptr<MonteCarloTreeNode>> iter(nodes_);
  while (iter.hasNext()) {
    iter.next();
    std::weak_ptr<MonteCarloTreeNode> &node = iter.value();
    if (node.expired())
      iter.remove();
    /*else
      node.lock()->RemoveExpiredParents();*/
  }

  qint64 time1 = timer.restart();

  QtConcurrent::blockingMap(nodes_.begin(), nodes_.end(), [](const std::weak_ptr<MonteCarloTreeNode> &node) { node.lock()->RemoveExpiredParents(); });

  qDebug() << "timer" << time1 << timer.elapsed();

  qDebug() << "nodes:" << num_before << "->" << nodes_.size();
}
