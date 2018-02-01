#include "montecarlotree.h"

#include <QDebug>

MonteCarloTree::MonteCarloTree() {
  node_collection_ = std::make_shared<MonteCarloTreeNodeCollection>();
}

void MonteCarloTree::Selection(std::vector<const std::shared_ptr<MonteCarloTreeNode>> &nodes) {
  nodes.push_back(root_);
  std::shared_ptr<MonteCarloTreeNode> node = root_;

  while (!node->IsLeafNode()) {
    node = node->SelectNextBestChild();
    nodes.push_back(node);
  }
}

void MonteCarloTree::Expansion(std::vector<const std::shared_ptr<MonteCarloTreeNode>> &nodes) {
  const std::shared_ptr<MonteCarloTreeNode> &node = nodes.back();

  if (!node->IsExpandable())
    return;

  // get next move
  Move next_move = node->GetNextUnexploredMove();

  // perform next move
  const std::shared_ptr<Board> next_board = node->GetBoard()->Copy();
  next_board->MakeMove(next_move);

  // get node for next move
  std::shared_ptr<MonteCarloTreeNode> next_node = node_collection_->GetNode(next_board);

  // add links
  next_node->AddParent(next_move, node);
  node->AddExploredMove(next_move, next_node);

  nodes.push_back(next_node);
}

Move MonteCarloTree::GetBestMove() {
  return root_->GetBestMove();
}

void MonteCarloTree::SetRoot(const std::shared_ptr<const Board> &board) {
  root_ = node_collection_->GetNode(board);
  node_collection_->RemoveExpiredNodes();
}
