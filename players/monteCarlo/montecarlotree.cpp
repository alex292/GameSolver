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
  // get next move
  Move next_move = selected_node->GetNextUnexploredMove();

  // perform next move
  const std::shared_ptr<Board> next_board = selected_node->GetBoard()->Copy();
  next_board->MakeMove(next_move);

  // get node for next move
  std::shared_ptr<MonteCarloTreeNode> next_node = node_collection_->GetNode(next_board);

  // add links
  next_node->AddParent(next_move, selected_node);
  selected_node->AddExploredMove(next_move, next_node);

  return next_node;
}

Move MonteCarloTree::GetBestMove() {
  return root_->GetBestMove();
}

void MonteCarloTree::SetRoot(const std::shared_ptr<const Board> &board) {
  root_ = node_collection_->GetNode(board);
  node_collection_->RemoveExpiredNodes();
}
