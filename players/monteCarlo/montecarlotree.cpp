#include "montecarlotree.h"

#include <QDebug>

MonteCarloTree::MonteCarloTree() {
  node_collection_ = std::make_shared<MonteCarloTreeNodeCollection>();
}

const std::shared_ptr<MonteCarloTreeNode> MonteCarloTree::Selection() {
  std::shared_ptr<MonteCarloTreeNode> node = root_;
  while (!node->IsLeafNode())
    node = node->SelectNextBestChild();

  return node;
}

const std::shared_ptr<MonteCarloTreeNode> MonteCarloTree::Expansion(const std::shared_ptr<MonteCarloTreeNode> &selected_node) {
  std::shared_ptr<MonteCarloTreeNode> next_node = selected_node->ExpandNextChild(node_collection_);

  return next_node;
}

Move MonteCarloTree::GetBestMove() {
  return root_->GetBestMove();
}

void MonteCarloTree::SetRoot(const std::shared_ptr<const Board> &board) {
  root_ = node_collection_->GetNode(board);
  node_collection_->RemoveExpiredNodes();
}
