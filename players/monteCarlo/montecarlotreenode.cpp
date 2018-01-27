#include "montecarlotreenode.h"
#include "players/random/randomplayer.h"
#include <QDebug>
#include <QReadLocker>

using namespace std;

MonteCarloTreeNode::MonteCarloTreeNode(const std::shared_ptr<const Board> &board) :
    board_(board)
{
    numEvaluations_ = 0;
    numWins_ = 0;
    numTies_ = 0;

    if(!board_->isGameOver())
        board_->getPossibleMoves(unexploredMoves_);
    else
        isWinningState_ = true;
}

MonteCarloTreeNode::MonteCarloTreeNode(const std::shared_ptr<const Board> &board, const std::shared_ptr<MonteCarloTreeNode> &parent) :
    board_(board)
{
    parents_.push_back(parent);
    numEvaluations_ = 0;
    numWins_ = 0;

    if(!board_->isGameOver())
        board_->getPossibleMoves(unexploredMoves_);
}

bool MonteCarloTreeNode::isLeafNode()
{
    QReadLocker locker(&exploredMovesLock_);

    return (unexploredMoves_.size() != 0 || exploredMoves_.size() == 0);
}

bool MonteCarloTreeNode::isExpandable()
{
    return (unexploredMoves_.size() > 0);
}

const std::shared_ptr<MonteCarloTreeNode> MonteCarloTreeNode::selectNextBestChild()
{
    if(isLosingState_)
        return exploredMoves_[nextMove_];

    if(isWinningState_)
        return exploredMoves_.begin().value();

    unsigned int numParentEvaluations = numEvaluations_;

    shared_ptr<MonteCarloTreeNode> nextNode;
    double bestValue = -1;

    QReadLocker locker(&exploredMovesLock_);
    QHashIterator<Move, shared_ptr<MonteCarloTreeNode>> iter(exploredMoves_);
    while(iter.hasNext()) {
        iter.next();
        const shared_ptr<MonteCarloTreeNode> &n = iter.value();

        double value = n->getUCTValue(numParentEvaluations);

        if(value > bestValue) {
            bestValue = value;
            nextNode = n;
        }
    }

    return nextNode;
}

double MonteCarloTreeNode::getUCTValue(unsigned int numParentEvaluations)
{
    if(numEvaluations_ == 0)
        return 0;

    if(isLosingState_)
        return 0;

    if(isWinningState_)
        return numeric_limits<double>::infinity();

    return ((numWins_ + 0.1*numTies_) / (double)numEvaluations_) + 1.414213562 * sqrt(log((double)numParentEvaluations) / numEvaluations_);
}

const std::shared_ptr<MonteCarloTreeNode> MonteCarloTreeNode::expandNextChild(const std::shared_ptr<MonteCarloTreeNodeCollection> &nodeCollection)
{
    // get next move
    Move nextMove = unexploredMoves_.back();
    unexploredMoves_.pop_back();

    // perform next move
    const shared_ptr<Board> nextBoard = board_->copy();
    nextBoard->makeMove(nextMove);

    // get node for next move
    shared_ptr<MonteCarloTreeNode> nextNode = nodeCollection->getNode(nextBoard);
    nextNode->addParent(shared_from_this());

    // add explored move
    QWriteLocker locker(&exploredMovesLock_);
    exploredMoves_.insert(nextMove, nextNode);

    expansionMutex_.unlock();

    if(nextNode->isWinningState())
        hasWinningMove(nextMove);

    return nextNode;
}

void MonteCarloTreeNode::addParent(const std::shared_ptr<MonteCarloTreeNode> &parent)
{
    parents_.push_back(parent);
}

void MonteCarloTreeNode::removeExpiredParents()
{
    parents_.erase(remove_if(parents_.begin(), parents_.end(), [](const weak_ptr<const MonteCarloTreeNode> &p){return p.expired();}), parents_.end());
}

const shared_ptr<const Board> MonteCarloTreeNode::randomPlayout() const
{
    shared_ptr<Board> board = board_->copy();
    RandomPlayer randomPlayer(board);
    while(!board->isGameOver()) {
        Move move = randomPlayer.getNextMove();
        board->makeMove(move);
    }
    return board;
}

void MonteCarloTreeNode::startBackpropagation(const std::shared_ptr<const Board> &playoutBoard)
{
    QSet<ZobristValue> propagatedNodes;

    if((board_->isTurnWhite() && playoutBoard->isWinWhite()) || (!board_->isTurnWhite() && playoutBoard->isWinBlack()))
        backpropagateLoss(propagatedNodes);
    else if((board_->isTurnWhite() && playoutBoard->isWinBlack()) || (!board_->isTurnWhite() && playoutBoard->isWinWhite()))
        backpropagateWin(propagatedNodes);
    else
        backpropagateTie(propagatedNodes);
}

void MonteCarloTreeNode::backpropagateWin(QSet<ZobristValue> &propagatedNodes)
{
    ZobristValue zobristValue = board_->getZobristValue();
    if(propagatedNodes.contains(zobristValue))
        return;
    propagatedNodes.insert(zobristValue);

    numEvaluations_++;
    numWins_++;

    for(const weak_ptr<MonteCarloTreeNode> &n : parents_) {
        if(!n.expired())
            n.lock()->backpropagateLoss(propagatedNodes);
    }
}

void MonteCarloTreeNode::backpropagateLoss(QSet<ZobristValue> &propagatedNodes)
{
    ZobristValue zobristValue = board_->getZobristValue();
    if(propagatedNodes.contains(zobristValue))
        return;
    propagatedNodes.insert(zobristValue);

    numEvaluations_++;

    for(const weak_ptr<MonteCarloTreeNode> &n : parents_) {
        if(!n.expired())
            n.lock()->backpropagateWin(propagatedNodes);
    }
}

void MonteCarloTreeNode::backpropagateTie(QSet<ZobristValue> &propagatedNodes)
{
    ZobristValue zobristValue = board_->getZobristValue();
    if(propagatedNodes.contains(zobristValue))
        return;
    propagatedNodes.insert(zobristValue);

    numEvaluations_++;
    numTies_++;

    for(const weak_ptr<MonteCarloTreeNode> &n : parents_) {
        if(!n.expired())
            n.lock()->backpropagateTie(propagatedNodes);
    }
}

Move MonteCarloTreeNode::getBestMove()
{
    if(isLosingState_)
        return nextMove_;

    Move bestMove;
    int bestMoveNumEvaluations = -1;

    QReadLocker locker(&exploredMovesLock_);
    QHashIterator<Move, shared_ptr<MonteCarloTreeNode>> iter(exploredMoves_);
    while(iter.hasNext()) {
        iter.next();
        const shared_ptr<MonteCarloTreeNode> &n = iter.value();

        int numEvaluations = n->getNumEvaluations();
        if(numEvaluations > bestMoveNumEvaluations) {
            bestMove = iter.key();
            bestMoveNumEvaluations = numEvaluations;
        }

        //qDebug() << "###" << iter.key() << iter.value()->getNumWins() << iter.value()->getNumEvaluations() << (iter.value()->getNumWins() / (double) iter.value()->getNumEvaluations());
    }

    return bestMove;
}

void MonteCarloTreeNode::lockExpansion()
{
    expansionMutex_.lock();
}

void MonteCarloTreeNode::unlockExpansion()
{
    expansionMutex_.unlock();
}

const std::shared_ptr<MonteCarloTreeNode> MonteCarloTreeNode::getChildNode(Move move)
{
    QReadLocker locker(&exploredMovesLock_);
    return exploredMoves_.value(move);
}

bool MonteCarloTreeNode::hasExploredChildNode(Move move)
{
    QReadLocker locker(&exploredMovesLock_);
    return exploredMoves_.contains(move);
}

void MonteCarloTreeNode::hasWinningMove(Move winningMove)
{
    QMutexLocker locker(&resultDecidedMutex_);

    if(isLosingState_)
        return;

    isLosingState_ = true;
    nextMove_ = winningMove;
    for(const weak_ptr<MonteCarloTreeNode> &n : parents_) {
        if(!n.expired())
            n.lock()->hasLosingMove();
    }
}

void MonteCarloTreeNode::hasLosingMove()
{
    QMutexLocker locker(&resultDecidedMutex_);

    if(isWinningState_)
        return;

    QMutexLocker expansionLocker(&expansionMutex_);
    if(!unexploredMoves_.empty())
        return;
    expansionLocker.unlock();

    bool allMovesLoosing = true;
    QReadLocker exploredLocker(&exploredMovesLock_);
    QHashIterator<Move, shared_ptr<MonteCarloTreeNode>> iter(exploredMoves_);
    while(iter.hasNext() && allMovesLoosing) {
        iter.next();
        const shared_ptr<MonteCarloTreeNode> &n = iter.value();
        allMovesLoosing &= n->isLosingState();
    }
    exploredLocker.unlock();

    if(allMovesLoosing) {
        isWinningState_ = true;
        Move lastMove = board_->getLastMove();
        for(const weak_ptr<MonteCarloTreeNode> &n : parents_) {
            if(!n.expired())
                n.lock()->hasWinningMove(lastMove);
        }
    }

}
