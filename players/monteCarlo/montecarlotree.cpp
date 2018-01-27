#include "montecarlotree.h"

#include <QDebug>

MonteCarloTree::MonteCarloTree(const std::shared_ptr<const Board> &board) {
  node_collection_ = std::make_shared<MonteCarloTreeNodeCollection>();
  root_ = node_collection_->CreateNode(board->Copy());
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

void MonteCarloTree::Update(Move move) {
  if (root_->HasExploredChildNode(move)) {
    root_ = root_->GetChildNode(move);
  } else {
    const std::shared_ptr<Board> board = root_->GetBoard()->Copy();
    board->MakeMove(move);
    root_ = node_collection_->CreateNode(board);
  }

  node_collection_->RemoveExpiredNodes();
}
