#include "humanplayer.h"

#include <iostream>

#include <QDebug>
#include <QTextStream>

HumanPlayer::HumanPlayer() : Player() {}

Move HumanPlayer::GetNextMove(const std::shared_ptr<const Board>& board) {
  Move move;
  do {
    std::cout << "HumanPlayer \t Enter Move: ";
    QTextStream qtin(stdin);
    QString word;
    qtin >> word;

    bool is_number;
    move = static_cast<Move>(word.toInt(&is_number));

    if (!is_number) {
      bool is_valid;
      move = board->ReadableMoveToMove(word, is_valid);

      if (!is_valid)
        continue;
    }
  } while (!board->IsMovePossible(move));

  return move;
}
