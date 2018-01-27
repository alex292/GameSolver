#ifndef BOARD2D_H
#define BOARD2D_H

#include "board.h"

class Board2D : public virtual Board
{
public:
    Board2D(int sizeX, int sizeY);

    QString move2readableMove(Move move) const;
    Move readableMove2move(const QString &readableMove, bool &valid) const;

    void getPossibleMoves(std::vector<Move> &moves) const;
    virtual bool isMovePossible(Move move) const;

protected:
    const int sizeX_;
    const int sizeY_;

    const int numMoves_;
    const int numPositions_;

    Board2D(int sizeX, int sizeY, int numMoves, int numPositions);

};

#endif // BOARD2D_H
