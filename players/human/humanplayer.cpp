#include "humanplayer.h"

#include <iostream>

#include <QTextStream>
#include <QDebug>

HumanPlayer::HumanPlayer(const std::shared_ptr<const Board> &board) :
    Player(board)
{

}


Move HumanPlayer::getNextMove()
{
    Move move;
    do {
        std::cout << "HumanPlayer \t Enter Move: ";
        QTextStream qtin(stdin);
        QString word;
        qtin >> word;

        bool isNumber;
        move = word.toInt(&isNumber);

        if(!isNumber) {
            bool isValid;
            move = board_->readableMove2move(word, isValid);

            if(!isValid)
                continue;
        }
    } while(!board_->isMovePossible(move));

    return move;
}
