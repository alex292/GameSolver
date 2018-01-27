#include "montecarloplayer.h"

#include <QThread>
#include <QTimer>
#include <QDebug>

using namespace std;

MonteCarloPlayer::MonteCarloPlayer(const std::shared_ptr<const Board> &board, int timePerMove, int numThreads) :
    Player(board),
    timePerMove_(timePerMove),
    numThreads_(numThreads)
{
    tree_ = make_shared<MonteCarloTree>(board);

    deadlineTimer_ = make_shared<QDeadlineTimer>();

    for(int i=0; i<numThreads_; ++i) {
        shared_ptr<MonteCarloTreeExplorer> treeExplorer = make_shared<MonteCarloTreeExplorer>(tree_, deadlineTimer_);
        treeExplorer->setAutoDelete(false);
        treeExplorers_.push_back(treeExplorer);
    }
}


Move MonteCarloPlayer::getNextMove()
{
    deadlineTimer_->setRemainingTime(timePerMove_);

    for(const shared_ptr<MonteCarloTreeExplorer> &treeExplorer : treeExplorers_)
        threadPool_.start(treeExplorer.get());

    threadPool_.waitForDone();

    Move bestMove = tree_->getBestMove();

    return bestMove;
}

void MonteCarloPlayer::update(Move move)
{
    tree_->update(move);
}

