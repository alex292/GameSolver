#ifndef CONNECTEDROWSBOARD_H
#define CONNECTEDROWSBOARD_H

#include "board.h"

class ConnectedRowsBoard : public virtual Board
{
public:
    ConnectedRowsBoard();

    void checkForGameOver(PositionIndex lastPosition);

protected:
    void initConnectedRows();

    virtual void generateRowsToPositions() = 0;
    void generatePositionsToRows();

    std::shared_ptr<std::vector<std::vector<PositionIndex>>> rowsToPositions_;
    std::shared_ptr<std::vector<std::vector<unsigned char>>> positionsToRows_;

};

#endif // CONNECTEDROWSBOARD_H
