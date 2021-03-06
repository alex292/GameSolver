#include "game.h"

#include <QDebug>

Game::Game(std::unique_ptr<Board> board,
           std::unique_ptr<Player> player_white,
           std::unique_ptr<Player> player_black)
    : board_(std::move(board)),
      player_white_(std::move(player_white)),
      player_black_(std::move(player_black)) {}

void Game::Run() {
  while (!board_->IsGameOver()) {
    Move move = (board_->active_color() == Color::WHITE)
                    ? player_white_->GetNextMove(board_.get())
                    : player_black_->GetNextMove(board_.get());
    qDebug() << "Player"
             << ((board_->active_color() == Color::WHITE) ? "white" : "black")
             << "plays" << move << "(" << board_->MoveToReadableMove(move)
             << ")";

    board_->MakeMove(move);
  }

  GameState state = board_->state();
  qDebug() << "### Game Over ###";
  if (state == GameState::WIN_WHITE)
    qDebug() << "Player white wins!";
  else if (state == GameState::WIN_BLACK)
    qDebug() << "Player black wins!";
  else
    qDebug() << "tie";
}
