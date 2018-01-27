#include "humanplayer.h"

#include <iostream>

#include <QDebug>
#include <QTextStream>

HumanPlayer::HumanPlayer(const std::shared_ptr<const Board> &board)
    : Player(board) {}

Move HumanPlayer::GetNextMove() {
  Move move;
  do {
    std::cout << "HumanPlayer \t Enter Move: ";
    QTextStream qtin(stdin);
    QString word;
    qtin >> word;

    bool is_number;
    move = word.toInt(&is_number);

    if (!is_number) {
      bool is_valid;
      move = board_->ReadableMoveToMove(word, is_valid);

      if (!is_valid) continue;
    }
  } while (!board_->IsMovePossible(move));

  return move;
}
