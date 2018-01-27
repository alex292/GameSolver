#include "connectfour3dboard.h"
#include "games/zobristgenerator.h"

using namespace std;


ConnectFour3DBoard::ConnectFour3DBoard() :
    Board2DStacked(4,4,4),
    ConnectedRowsBoard()
{
    // zobrist values
    ZobristGenerator &zobristGenerator = ZobristGenerator::getInstance();
    zobristValuesPositionsWhite_ = zobristGenerator.generateUniqueZobristValues(numPositions_);
    zobristValuesPositionsBlack_ = zobristGenerator.generateUniqueZobristValues(numPositions_);

    // init connected rows
    initConnectedRows();
}

PositionIndex ConnectFour3DBoard::performMove(Move move)
{
    PositionIndex position = move + 16*stackHeights_[move];

    // set position
    stackHeights_[move] += 1;
    if(turnWhite_) {
        positions_[position] = POSITION_VALUE_WHITE;
        zobristValue_ ^= zobristValuesPositionsWhite_->at(position);
    } else {
        positions_[position] = POSITION_VALUE_BLACK;
        zobristValue_ ^= zobristValuesPositionsBlack_->at(position);
    }

    return position;
}

std::shared_ptr<Board> ConnectFour3DBoard::copy() const
{
    return make_shared<ConnectFour3DBoard>(*this);
}

void ConnectFour3DBoard::generateRowsToPositions()
{
    rowsToPositions_ = make_shared<vector<vector<PositionIndex>>>();

    // straight rows X,Y,Z
    for(int i=0; i<4; ++i) {
        for(int j=0; j<4; ++j) {
            vector<PositionIndex> rowX;
            vector<PositionIndex> rowY;
            vector<PositionIndex> rowZ;

            for(int k=0; k<4; ++k) {
                rowX.push_back(mapXYZtoPositionIndex(k,j,i));
                rowY.push_back(mapXYZtoPositionIndex(j,k,i));
                rowZ.push_back(mapXYZtoPositionIndex(j,i,k));
            }

            rowsToPositions_->push_back(rowX);
            rowsToPositions_->push_back(rowY);
            rowsToPositions_->push_back(rowZ);
        }
    }

    // 2D diagonals
    for(int i=0; i<4; ++i) {
        vector<PositionIndex> diagX;
        vector<PositionIndex> diagXinv;

        vector<PositionIndex> diagY;
        vector<PositionIndex> diagYinv;

        vector<PositionIndex> diagZ;
        vector<PositionIndex> diagZinv;

        for(int j=0; j<4; ++j) {
            diagX.push_back(mapXYZtoPositionIndex(i,j,j));
            diagXinv.push_back(mapXYZtoPositionIndex(i,3-j,j));

            diagY.push_back(mapXYZtoPositionIndex(j,i,j));
            diagYinv.push_back(mapXYZtoPositionIndex(j,i,3-j));

            diagZ.push_back(mapXYZtoPositionIndex(j,j,i));
            diagZinv.push_back(mapXYZtoPositionIndex(j,3-j,i));
        }

        rowsToPositions_->push_back(diagX);
        rowsToPositions_->push_back(diagXinv);
        rowsToPositions_->push_back(diagY);
        rowsToPositions_->push_back(diagYinv);
        rowsToPositions_->push_back(diagZ);
        rowsToPositions_->push_back(diagZinv);
    }

    // 3D diagonals
    vector<PositionIndex> diagTL;
    vector<PositionIndex> diagTR;
    vector<PositionIndex> diagBR;
    vector<PositionIndex> diagBL;

    for(int i=0; i<4; ++i) {
        diagTL.push_back(mapXYZtoPositionIndex(i,i,i));
        diagTR.push_back(mapXYZtoPositionIndex(3-i,i,i));
        diagBR.push_back(mapXYZtoPositionIndex(3-i,3-i,i));
        diagBL.push_back(mapXYZtoPositionIndex(i,3-i,i));
    }

    rowsToPositions_->push_back(diagTL);
    rowsToPositions_->push_back(diagTR);
    rowsToPositions_->push_back(diagBR);
    rowsToPositions_->push_back(diagBL);
}
