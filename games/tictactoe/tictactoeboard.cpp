#include "tictactoeboard.h"
#include "games/zobristgenerator.h"

#include <QDebug>

using namespace std;

TicTacToeBoard::TicTacToeBoard() :
    Board2D(3,3),
    ConnectedRowsBoard()
{
    // zobrist values
    ZobristGenerator &zobristGenerator = ZobristGenerator::getInstance();
    zobristValuesPositionsWhite_ = zobristGenerator.generateUniqueZobristValues(numPositions_);
    zobristValuesPositionsBlack_ = zobristGenerator.generateUniqueZobristValues(numPositions_);

    // connected rows
    initConnectedRows();
}

PositionIndex TicTacToeBoard::performMove(Move move)
{
    PositionIndex position = move;

    // set position
    if(turnWhite_) {
        positions_[position] = POSITION_VALUE_WHITE;
        zobristValue_ ^= zobristValuesPositionsWhite_->at(position);
    } else {
        positions_[position] = POSITION_VALUE_BLACK;
        zobristValue_ ^= zobristValuesPositionsBlack_->at(position);
    }

    return position;
}

std::shared_ptr<Board> TicTacToeBoard::copy() const
{
    return make_shared<TicTacToeBoard>(*this);
}

void TicTacToeBoard::generateRowsToPositions()
{
    rowsToPositions_ = make_shared<vector<vector<PositionIndex>>>();
    rowsToPositions_->push_back({0,1,2}); // 0
    rowsToPositions_->push_back({3,4,5}); // 1
    rowsToPositions_->push_back({6,7,8}); // 2
    rowsToPositions_->push_back({0,3,6}); // 3
    rowsToPositions_->push_back({1,4,7}); // 4
    rowsToPositions_->push_back({2,5,8}); // 5
    rowsToPositions_->push_back({0,4,8}); // 6
    rowsToPositions_->push_back({2,4,6}); // 7
}
