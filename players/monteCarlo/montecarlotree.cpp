#include "montecarlotree.h"

#include <QDebug>

void MonteCarloTree::SetRoot(std::unique_ptr<const Board> board) {
  ZobristValue old_zobrist_value = root_->GetBoard()->zobrist_value();
  ZobristValue new_zobrist_value = board->zobrist_value();
  if (root_ && old_zobrist_value == new_zobrist_value)
    return;

  root_ = node_collection_.GetNode(std::move(board));
  node_collection_.RemoveExpiredNodes();
}

const std::shared_ptr<MonteCarloTreeNode> MonteCarloTree::GetNode(
    std::unique_ptr<const Board> board) {
  return node_collection_.GetNode(std::move(board));
}
