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

    std::shared_ptr<const Board> playout_board;
    node->LockExpansion();
    if (node->IsExpandable()) {
      // expansion
      node = tree_->Expansion(node);

      // playout
      playout_board = node->RandomPlayout();
    } else {
      node->UnlockExpansion();
      playout_board = node->GetBoard();
    }

    // backpropagation
    node->StartBackpropagation(playout_board);
  }
}
