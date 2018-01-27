#ifndef MONTECARLOTREEEXPLORER_H
#define MONTECARLOTREEEXPLORER_H

#include "montecarlotree.h"

#include <QRunnable>
#include <QDeadlineTimer>

class MonteCarloTreeExplorer : public QRunnable
{

public:
    MonteCarloTreeExplorer(const std::shared_ptr<MonteCarloTree> &tree, const std::shared_ptr<const QDeadlineTimer> &deadlineTimer);

    void run();

protected:
    const std::shared_ptr<const QDeadlineTimer> deadlineTimer_;

    std::shared_ptr<MonteCarloTree> tree_;
};

#endif // MONTECARLOTREEEXPLORER_H
