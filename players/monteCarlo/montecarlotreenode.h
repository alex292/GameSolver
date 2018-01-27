#ifndef MONTECARLOTREENODE_H
#define MONTECARLOTREENODE_H

#include "games/board.h"
#include "montecarlotreenodecollection.h"
class MonteCarloTreeNodeCollection;

#include <QMutex>
#include <QReadWriteLock>
#include <QSet>

#include <atomic>

class MonteCarloTreeNode : public std::enable_shared_from_this<MonteCarloTreeNode>
{
public:
    MonteCarloTreeNode(const std::shared_ptr<const Board> &board);
    MonteCarloTreeNode(const std::shared_ptr<const Board> &board, const std::shared_ptr<MonteCarloTreeNode> &parent);

    ZobristValue getZobristValue() const {return board_->getZobristValue();}

    bool isLeafNode();
    bool isExpandable();
    const std::shared_ptr<MonteCarloTreeNode> selectNextBestChild();
    double getUCTValue(unsigned int numParentEvaluations);

    const std::shared_ptr<MonteCarloTreeNode> expandNextChild(const std::shared_ptr<MonteCarloTreeNodeCollection> &nodeCollection);

    const std::shared_ptr<Board> getBoardCopy() const {return board_->copy();}
    const std::shared_ptr<const Board> getBoard() const {return board_;}

    void addParent(const std::shared_ptr<MonteCarloTreeNode> &parent);
    void removeExpiredParents();

    const std::shared_ptr<const Board> randomPlayout() const;

    void startBackpropagation(const std::shared_ptr<const Board> &playoutBoard);
    void backpropagateWin(QSet<ZobristValue> &propagatedNodes);
    void backpropagateLoss(QSet<ZobristValue> &propagatedNodes);
    void backpropagateTie(QSet<ZobristValue> &propagatedNodes);

    Move getBestMove();

    void lockExpansion();
    void unlockExpansion();

    unsigned int getNumEvaluations() const {return numEvaluations_;}
    unsigned int getNumWins() const {return numWins_;}

    const std::shared_ptr<MonteCarloTreeNode> getChildNode(Move move);
    bool hasExploredChildNode(Move move);

    std::vector<Move> unexploredMoves_;

    bool isWinningState() {return isWinningState_;}
    bool isLosingState() {return isLosingState_;}
    bool isResultDecided() {return (isWinningState_ || isLosingState_);}

    void hasWinningMove(Move winningMove);
    void hasLosingMove();

protected:
    std::shared_ptr<const Board> board_;

    std::vector<std::weak_ptr<MonteCarloTreeNode>> parents_;

    std::atomic<unsigned int> numEvaluations_;
    std::atomic<unsigned int> numWins_;
    std::atomic<unsigned int> numTies_;

    QMutex expansionMutex_;

    QReadWriteLock exploredMovesLock_;
    QHash<Move, std::shared_ptr<MonteCarloTreeNode>> exploredMoves_;

    QMutex resultDecidedMutex_;
    std::atomic<bool> isWinningState_ = {false};
    std::atomic<bool> isLosingState_ = {false};
    std::atomic<Move> nextMove_;

};

#endif // MONTECARLOTREENODE_H
