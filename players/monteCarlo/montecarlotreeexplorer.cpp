#include "montecarlotreeexplorer.h"

#include <QDebug>
#include <QThread>

MonteCarloTreeExplorer::MonteCarloTreeExplorer(
    MonteCarloTree* tree,
    const QDeadlineTimer* deadline_timer)
    : tree_(tree), deadline_timer_(deadline_timer) {}

void MonteCarloTreeExplorer::run() {
  const std::shared_ptr<MonteCarloTreeNode> root = tree_->root();
  std::vector<std::shared_ptr<MonteCarloTreeNode>> node_path;

  while (!deadline_timer_->hasExpired() && !root->IsDecidedState()) {
    node_path.clear();
    Selection(node_path);
    Expansion(node_path);
    std::unique_ptr<const Board> playout_board =
        RandomPlayout(node_path.back());
    BackpropagateResult(node_path, playout_board.get());
  }
}

void MonteCarloTreeExplorer::Selection(
    std::vector<std::shared_ptr<MonteCarloTreeNode>>& nodes) {
  std::shared_ptr<MonteCarloTreeNode> node = tree_->root();
  nodes.push_back(node);

  while (!node->IsLeafNode()) {
    node = node->SelectNextBestChild();
    nodes.push_back(node);
  }
}

void MonteCarloTreeExplorer::Expansion(
    std::vector<std::shared_ptr<MonteCarloTreeNode>>& nodes) {
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
      tree_->GetNode(std::move(next_board));

  // add links
  next_node->AddParent(next_move, node);
  node->AddExploredMove(next_move, next_node);

  nodes.push_back(next_node);
}

std::unique_ptr<const Board> MonteCarloTreeExplorer::RandomPlayout(
    const std::shared_ptr<const MonteCarloTreeNode>& node) {
  std::unique_ptr<Board> board = node->GetBoard()->Copy();
  while (!board->IsGameOver()) {
    Move move = random_player_.GetNextMove(board.get());
    board->MakeMove(move);
  }
  return board;
}

void MonteCarloTreeExplorer::BackpropagateResult(
    const std::vector<std::shared_ptr<MonteCarloTreeNode>>& node_path,
    const Board* playout_board) {
  for (const std::shared_ptr<MonteCarloTreeNode>& node : node_path) {
    const Board* board = node->GetBoard();
    if (playout_board->state() == GameState::TIE)
      node->AddTie();
    else if (playout_board->IsWin(board->active_color()))
      node->AddLoss();
    else
      node->AddWin();
  }
}
