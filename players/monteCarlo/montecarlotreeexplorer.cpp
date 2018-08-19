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
    tree_->Selection(node_path);
    tree_->Expansion(node_path);
    std::unique_ptr<const Board> playout_board =
        RandomPlayout(node_path.back());
    BackpropagateResult(node_path, playout_board.get());
  }
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
