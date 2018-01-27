#include "connectedrowsboard.h"

using namespace std;

ConnectedRowsBoard::ConnectedRowsBoard()
{
}


void ConnectedRowsBoard::checkForGameOver(PositionIndex lastPosition)
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

void ConnectedRowsBoard::initConnectedRows()
{
    generateRowsToPositions();
    generatePositionsToRows();
}

void ConnectedRowsBoard::generatePositionsToRows()
{
    positionsToRows_ = make_shared<vector<vector<unsigned char>>>(positions_.size());

    for(int row=0; row<rowsToPositions_->size(); ++row) {
        for(PositionIndex positionIndex : rowsToPositions_->at(row)) {
            positionsToRows_->at(positionIndex).push_back(row);
        }
    }
}
