#ifndef MONTECARLOPLAYER_H
#define MONTECARLOPLAYER_H

#include "players/player.h"
#include "montecarlotree.h"
#include "montecarlotreeexplorer.h"

#include <QHash>
#include <QReadWriteLock>
#include <QThreadPool>

class MonteCarloPlayer : public Player
{

public:
    MonteCarloPlayer(const std::shared_ptr<const Board> &board, int timePerMove, int numThreads = QThread::idealThreadCount());

    Move getNextMove();
    void update(Move move);

protected:
    const int numThreads_;
    const int timePerMove_;

    std::shared_ptr<MonteCarloTree> tree_;

    std::vector<std::shared_ptr<MonteCarloTreeExplorer>> treeExplorers_;

    std::shared_ptr<QDeadlineTimer> deadlineTimer_;

    QThreadPool threadPool_;

};

#endif // MONTECARLOPLAYER_H
