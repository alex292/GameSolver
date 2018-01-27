#include "montecarlotree.h"
#include <QDebug>

using namespace std;

MonteCarloTree::MonteCarloTree(const std::shared_ptr<const Board> &board)
{
    nodeCollection_ = make_shared<MonteCarloTreeNodeCollection>();
    root_ = nodeCollection_->createNode(board->copy());
}

const std::shared_ptr<MonteCarloTreeNode> MonteCarloTree::selection()
{
    shared_ptr<MonteCarloTreeNode> node = root_;
    while(!node->isLeafNode())
        node = node->selectNextBestChild();

    return node;
}

const std::shared_ptr<MonteCarloTreeNode> MonteCarloTree::expansion(const std::shared_ptr<MonteCarloTreeNode> &selectedNode)
{
    shared_ptr<MonteCarloTreeNode> nextNode = selectedNode->expandNextChild(nodeCollection_);

    return nextNode;
}

Move MonteCarloTree::getBestMove()
{
    return root_->getBestMove();
}

void MonteCarloTree::update(Move move)
{
    if(root_->hasExploredChildNode(move)) {
        root_ = root_->getChildNode(move);
    } else {
        const shared_ptr<Board> board = root_->getBoardCopy();
        board->makeMove(move);
        root_ = nodeCollection_->createNode(board);
    }

    nodeCollection_->removeExpiredNodes();
}
