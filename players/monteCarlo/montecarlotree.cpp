#include "montecarlotree.h"

#include <QDebug>

MonteCarloTree::MonteCarloTree() {
  node_collection_ = std::make_unique<MonteCarloTreeNodeCollection>();
}

void MonteCarloTree::Selection(
    std::vector<std::shared_ptr<MonteCarloTreeNode> >& nodes) {
  std::shared_ptr<MonteCarloTreeNode> node = root_;
  nodes.push_back(node);

  while (!node->IsLeafNode()) {
    node = node->SelectNextBestChild();
    nodes.push_back(node);
  }
}

void MonteCarloTree::Expansion(
    std::vector<std::shared_ptr<MonteCarloTreeNode> >& nodes) {
  const std::shared_ptr<MonteCarloTreeNode>& node = nodes.back();

  if (!node->IsExpandable())
    return;

  // get next move
  Move next_move = node->GetNextUnexploredMove();

  // perform next move
  std::unique_ptr<Board> next_board = node->GetBoard()->Copy();
  next_board->MakeMove(next_move);

  // get node for next move
  std::shared_ptr<MonteCarloTreeNode> next_node =
      node_collection_->GetNode(std::move(next_board));

  // add links
  next_node->AddParent(next_move, node);
  node->AddExploredMove(next_move, next_node);

  nodes.push_back(next_node);
}

void MonteCarloTree::SetRoot(std::unique_ptr<const Board> board) {
  root_ = node_collection_->GetNode(std::move(board));
  node_collection_->RemoveExpiredNodes();
}
