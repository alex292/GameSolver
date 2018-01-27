#ifndef TICTACTOEBOARD_H
#define TICTACTOEBOARD_H

#include "games/board2d.h".h"
#include "games/connectedrowsboard.h"


class TicTacToeBoard : public Board2D, public ConnectedRowsBoard
{
public:
    TicTacToeBoard();

    PositionIndex performMove(Move move);

    std::shared_ptr<Board> copy() const;

protected:
    void generateRowsToPositions();

    std::shared_ptr<std::vector<ZobristValue>> zobristValuesPositionsWhite_;
    std::shared_ptr<std::vector<ZobristValue>> zobristValuesPositionsBlack_;
};

#endif // TICTACTOEBOARD_H
