#ifndef BOARD_H
#define BOARD_H

#include "types.h"

#include <vector>
#include <memory>
#include <array>

#include <QString>
#include <QSize>
#include <QSet>

class Board
{
public:
    Board();

    virtual QString move2readableMove(Move move) const = 0;
    virtual Move readableMove2move(const QString &readableMove, bool &valid) const = 0;

    virtual void getPossibleMoves(std::vector<Move> &moves) const = 0;
    virtual bool isMovePossible(Move move) const = 0;

    void makeMove(Move move);
    virtual PositionIndex performMove(Move move) = 0;
    virtual std::shared_ptr<Board> copy() const = 0;

    bool isTurnWhite() const {return turnWhite_;}
    bool isGameOver() const {return gameOver_;}
    bool isWinWhite() const {return winWhite_;}
    bool isWinBlack() const {return winBlack_;}
    bool isTie() const {return gameOver_ && !winWhite_ && !winBlack_;}

    ZobristValue getZobristValue() const {return zobristValue_;}
    int getNumMovesMade() const {return moveSequence_.size();}
    Move getLastMove() const {return moveSequence_.back();}

protected:
    bool turnWhite_ = true;
    bool gameOver_ = false;
    bool winWhite_ = false;
    bool winBlack_ = false;

    ZobristValue zobristValue_ = 0;

    std::vector<Move> moveSequence_;

    std::vector<PositionValue> positions_;

    virtual void checkForGameOver(PositionIndex lastPosition) = 0;
};



    /*
public:
    Board(unsigned int sizeX, unsigned int sizeY);

    QString move2readableMove(MoveIndex move) const;
    MoveIndex readableMove2move(const QString &readableMove, bool &valid) const;

    void getPossibleMoves(std::vector<MoveIndex> &moves) const;
    virtual bool isMovePossible(MoveIndex move) const;

    virtual void makeMove(MoveIndex move);
    virtual std::shared_ptr<Board> copy() const = 0;

    bool isTurnWhite() const {return turnWhite_;}
    bool isGameOver() const {return gameOver_;}
    bool isWinWhite() const {return winWhite_;}
    bool isWinBlack() const {return winBlack_;}
    bool isTie() const {return gameOver_ && !winWhite_ && !winBlack_;}

    ZobristValue getZobristValue() const {return zobristValue_;}
    int getNumMoves() const {return moveSequence_.size();}
    MoveIndex getLastMove() const {return moveSequence_.back();}

protected:
    const unsigned int sizeX_;
    const unsigned int sizeY_;
    const unsigned int numMoves_;

    std::vector<PositionValue> positions_;
    std::vector<MoveIndex> moveSequence_;


    virtual void setPosition(PositionIndex move);
    virtual void checkForGameOver(PositionIndex lastPosition);

    virtual void generateRowsToPositions() = 0;
    void generatePositionsToRows();

    static bool staticBoardInitialized_;

    static std::shared_ptr<std::vector<ZobristValue>> zobristValuesPositionsWhite_;
    static std::shared_ptr<std::vector<ZobristValue>> zobristValuesPositionsBlack_;

    static std::shared_ptr<std::vector<std::vector<PositionIndex>>> rowsToPositions_;
    static std::shared_ptr<std::vector<std::vector<unsigned char>>> positionsToRows_;*/


#endif // BOARD_H
