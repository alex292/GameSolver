#include "board2d.h"

using namespace std;

Board2D::Board2D(int sizeX, int sizeY) :
    Board2D(sizeX, sizeY, sizeX*sizeY, sizeX*sizeY)
{
}

Board2D::Board2D(int sizeX, int sizeY, int numMoves, int numPositions) :
    sizeX_(sizeX),
    sizeY_(sizeY),
    numMoves_(numMoves),
    numPositions_(numPositions)
{
    positions_ = vector<PositionValue>(numPositions, POSITION_VALUE_FREE);
}

QString Board2D::move2readableMove(Move move) const
{
    int y = move / sizeX_;
    int x = move % sizeX_;

    return ((char)'A' + x) + QString::number(y + 1);
}

Move Board2D::readableMove2move(const QString &readableMove, bool &valid) const
{
    int x = (readableMove.at(0).toUpper().toLatin1() - 'A');
    int y = readableMove.mid(1).toInt() - 1;

    valid = (x < 0 || x >= sizeX_ || y < 0 || y >= sizeY_);

    return y*sizeX_ + x;
}

void Board2D::getPossibleMoves(std::vector<Move> &moves) const
{
    if(gameOver_)
        return;

    for(int i=0; i<numMoves_; ++i) {
        if(isMovePossible(i))
            moves.push_back(i);
    }
}

bool Board2D::isMovePossible(Move move) const
{
    return (positions_[move] == POSITION_VALUE_FREE);
}
