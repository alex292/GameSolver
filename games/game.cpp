#include "game.h"

#include <QDebug>

using namespace std;

Game::Game(const shared_ptr<Board> &board, const shared_ptr<Player> &playerWhite, const shared_ptr<Player> &playerBlack) :
    board_(board),
    playerWhite_(playerWhite),
    playerBlack_(playerBlack)
{

}

void Game::run()
{
    while(!board_->isGameOver()) {
        Move move = (board_->isTurnWhite()) ? playerWhite_->getNextMove() : playerBlack_->getNextMove();
        qDebug() << "Player" << ((board_->isTurnWhite()) ? "white" : "black") << "plays" << move << "(" << board_->move2readableMove(move) << ")";

        board_->makeMove(move);
        playerWhite_->update(move);
        playerBlack_->update(move);
    }

    qDebug() << "### Game Over ###";
    if(board_->isWinWhite())
        qDebug() << "Player white wins!";
    else if(board_->isWinBlack())
        qDebug() << "Player black wins!";
    else
        qDebug() << "tie";
}
