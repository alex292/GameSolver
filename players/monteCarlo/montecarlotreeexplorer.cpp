#include "montecarlotreeexplorer.h"

#include <QDebug>
#include <QThread>

MonteCarloTreeExplorer::MonteCarloTreeExplorer(const std::shared_ptr<MonteCarloTree> &tree, const std::shared_ptr<const QDeadlineTimer> &deadline_timer)
    : tree_(tree)
    , deadline_timer_(deadline_timer) {}

void MonteCarloTreeExplorer::run() {
  const std::shared_ptr<MonteCarloTreeNode> root = tree_->GetTreeRoot();
  std::vector<const std::shared_ptr<MonteCarloTreeNode>> node_path;

  while (!deadline_timer_->hasExpired() && !root->IsResultDecided()) {
    node_path.clear();
    tree_->Selection(node_path);
    tree_->Expansion(node_path);
    const std::shared_ptr<const Board> playout_board = RandomPlayout(node_path.back());
    BackpropagateResult(node_path, playout_board);
  }
}

const std::shared_ptr<const Board> MonteCarloTreeExplorer::RandomPlayout(const std::shared_ptr<const MonteCarloTreeNode> &node) {
  if (node->GetBoard()->IsGameOver())
    return node->GetBoard();  // avoid copy

  std::shared_ptr<Board> board = node->GetBoard()->Copy();
  while (!board->IsGameOver()) {
    Move move = random_player_.GetNextMove(board);
    board->MakeMove(move);
  }
  return board;
}

void MonteCarloTreeExplorer::BackpropagateResult(const std::vector<const std::shared_ptr<MonteCarloTreeNode>> &node_path, const std::shared_ptr<const Board> &playout_board) {
  for (const std::shared_ptr<MonteCarloTreeNode> &node : node_path) {
    const std::shared_ptr<const Board> &board = node->GetBoard();
    if ((board->IsTurnWhite() && playout_board->IsWinWhite()) || (!board->IsTurnWhite() && playout_board->IsWinBlack()))
      node->AddLoss();
    else if ((board->IsTurnWhite() && playout_board->IsWinBlack()) || (!board->IsTurnWhite() && playout_board->IsWinWhite()))
      node->AddWin();
    else
      node->AddTie();
  }
}
