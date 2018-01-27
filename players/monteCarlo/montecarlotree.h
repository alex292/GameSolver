#ifndef MONTECARLOTREE_H
#define MONTECARLOTREE_H

#include "montecarlotreenode.h"
#include "montecarlotreenodecollection.h"

#include <QReadWriteLock>

class MonteCarloTree
{
public:
    MonteCarloTree(const std::shared_ptr<const Board> &board);


    const std::shared_ptr<MonteCarloTreeNode> getTreeRoot() const {return root_;}

    const std::shared_ptr<MonteCarloTreeNode> selection();
    const std::shared_ptr<MonteCarloTreeNode> expansion(const std::shared_ptr<MonteCarloTreeNode> &selectedNode);

    Move getBestMove();
    void update(Move move);


protected:
    std::shared_ptr<MonteCarloTreeNode> root_;
    std::shared_ptr<MonteCarloTreeNodeCollection> nodeCollection_;

};

#endif // MONTECARLOTREE_H
