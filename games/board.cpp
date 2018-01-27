#include "board.h"

#include <QDebug>

using namespace std;


Board::Board()
{

}

void Board::makeMove(Move move)
{
    moveSequence_.push_back(move);

    PositionIndex lastPosition = performMove(move);
    checkForGameOver(lastPosition);

    turnWhite_ = !turnWhite_;
}

/*

// static definitions
bool Board::staticBoardInitialized_ = false;
shared_ptr<vector<ZobristValue>> Board::zobristValuesPositionsWhite_;
shared_ptr<vector<ZobristValue>> Board::zobristValuesPositionsBlack_;
shared_ptr<vector<vector<PositionIndex>>> Board::rowsToPositions_;
shared_ptr<vector<vector<unsigned char>>> Board::positionsToRows_;

Board::Board(unsigned int sizeX, unsigned int sizeY) :
    sizeX_(sizeX),
    sizeY_(sizeY),
    numMoves_(sizeX * sizeY)
{
}

QString Board::move2readableMove(MoveIndex move) const
{
    int y = move / sizeX_;
    int x = move % sizeX_;

    return ((char)'A' + x) + QString::number(y + 1);
}

MoveIndex Board::readableMove2move(const QString &readableMove, bool &valid) const
{
    int x = (readableMove.at(0).toUpper().toLatin1() - 'A');
    int y = readableMove.mid(1).toInt() - 1;

    valid = (x < 0 || x >= sizeX_ || y < 0 || y >= sizeY_);

    return y*sizeX_ + x;
}

void Board::getPossibleMoves(std::vector<MoveIndex> &moves) const
{
    moves.clear();

    if(gameOver_)
        return;

    for(int i=0; i<numMoves_; ++i) {
        if(isMovePossible(i))
            moves.push_back(i);
    }
}

bool Board::isMovePossible(MoveIndex move) const
{
    return (positions_[move] == POSITION_VALUE_FREE);
}

void Board::makeMove(MoveIndex move)
{
    moveSequence_.push_back(move);

    setPosition(move);
    checkForGameOver(move);

    turnWhite_ = !turnWhite_;
}

void Board::setPosition(PositionIndex move)
{
    if(turnWhite_) {
        positions_[move] = POSITION_VALUE_WHITE;
        zobristValue_ ^= zobristValuesPositionsWhite_->at(move);
    } else {
        positions_[move] = POSITION_VALUE_BLACK;
        zobristValue_ ^= zobristValuesPositionsBlack_->at(move);
    }
}

void Board::checkForGameOver(PositionIndex lastPosition)
{
    // check for win
    PositionValue desiredPositionValue = (turnWhite_) ? POSITION_VALUE_WHITE : POSITION_VALUE_BLACK;
    for(unsigned char row : positionsToRows_->at(lastPosition)) {
        const vector<PositionIndex> &rowPositions = rowsToPositions_->at(row);

        bool allDesired = true;
        for(int i=0; i<rowPositions.size() && allDesired; ++i)
            allDesired = (positions_[rowPositions[i]] == desiredPositionValue);

        if(allDesired) {
            gameOver_ = true;
            winWhite_ = turnWhite_;
            winBlack_ = !turnWhite_;
            return;
        }
    }

    // check for tie
    if(moveSequence_.size() == positions_.size()) {
        gameOver_ = true;
        winWhite_ = false;
        winBlack_ = false;
        return;
    }
}

void Board::generatePositionsToRows()
{
    positionsToRows_ = make_shared<vector<vector<unsigned char>>>(positions_.size());

    for(int row=0; row<rowsToPositions_->size(); ++row) {
        for(PositionIndex positionIndex : rowsToPositions_->at(row)) {
            positionsToRows_->at(positionIndex).push_back(row);
        }
    }
}*/
