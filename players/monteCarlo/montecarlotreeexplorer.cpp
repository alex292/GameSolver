#include "montecarlotreeexplorer.h"
#include <QDebug>
#include <QThread>

using namespace std;

MonteCarloTreeExplorer::MonteCarloTreeExplorer(const std::shared_ptr<MonteCarloTree> &tree, const std::shared_ptr<const QDeadlineTimer> &deadlineTimer)  :
    tree_(tree),
    deadlineTimer_(deadlineTimer)
{

}

void MonteCarloTreeExplorer::run()
{
    const shared_ptr<MonteCarloTreeNode> root = tree_->getTreeRoot();

    while(!deadlineTimer_->hasExpired() && !root->isResultDecided()) {
        // selection
        shared_ptr<MonteCarloTreeNode> node = tree_->selection();

        shared_ptr<const Board> playoutBoard;
        node->lockExpansion();
        if(node->isExpandable()) {
            // expansion
            node = tree_->expansion(node);

            // playout
            playoutBoard = node->randomPlayout();
        } else {
            node->unlockExpansion();
            playoutBoard = node->getBoard();
        }

        // backpropagation
        node->startBackpropagation(playoutBoard);
    }
}
