#include "game.h"

#include <QDebug>

Game::Game(const std::shared_ptr<Board>& board,
           const std::shared_ptr<Player>& player_white,
           const std::shared_ptr<Player>& player_black)
    : board_(board), player_white_(player_white), player_black_(player_black) {}

void Game::Run() {
  while (!board_->IsGameOver()) {
    Move move = (board_->IsTurnWhite()) ? player_white_->GetNextMove(board_)
                                        : player_black_->GetNextMove(board_);
    qDebug() << "Player" << ((board_->IsTurnWhite()) ? "white" : "black")
             << "plays" << move << "(" << board_->MoveToReadableMove(move)
             << ")";

    board_->MakeMove(move);
  }

  qDebug() << "### Game Over ###";
  if (board_->IsWinWhite())
    qDebug() << "Player white wins!";
  else if (board_->IsWinBlack())
    qDebug() << "Player black wins!";
  else
    qDebug() << "tie";
}
