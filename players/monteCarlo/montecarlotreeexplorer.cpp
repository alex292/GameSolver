#include "montecarlotreeexplorer.h"

#include <QDebug>
#include <QThread>

MonteCarloTreeExplorer::MonteCarloTreeExplorer(const std::shared_ptr<MonteCarloTree> &tree, const std::shared_ptr<const QDeadlineTimer> &deadline_timer)
    : tree_(tree)
    , deadline_timer_(deadline_timer) {}

void MonteCarloTreeExplorer::run() {
  const std::shared_ptr<MonteCarloTreeNode> root = tree_->GetTreeRoot();

  while (!deadline_timer_->hasExpired() && !root->IsResultDecided()) {
    // selection
    std::shared_ptr<MonteCarloTreeNode> node = tree_->Selection();

    // expansion
    if (node->IsExpandable())
      node = tree_->Expansion(node);

    // random playout
    const std::shared_ptr<const Board> playout_board = RandomPlayout(node);

    // backpropagation
    node->StartBackpropagation(playout_board);
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
