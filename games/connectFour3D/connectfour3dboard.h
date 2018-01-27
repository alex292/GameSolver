#ifndef CONNECTFOUR3DBOARD_H
#define CONNECTFOUR3DBOARD_H

#include "games/board2dstacked.h"
#include "games/connectedrowsboard.h"

class ConnectFour3DBoard : public Board2DStacked, public ConnectedRowsBoard
{
public:
    ConnectFour3DBoard();

    PositionIndex performMove(Move move);

    std::shared_ptr<Board> copy() const;

protected:
    void generateRowsToPositions();

    std::shared_ptr<std::vector<ZobristValue>> zobristValuesPositionsWhite_;
    std::shared_ptr<std::vector<ZobristValue>> zobristValuesPositionsBlack_;
};

#endif // CONNECTFOUR3DBOARD_H
