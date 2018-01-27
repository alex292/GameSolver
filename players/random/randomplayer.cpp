#include "randomplayer.h"

using namespace std;

RandomPlayer::RandomPlayer(const std::shared_ptr<const Board> &board) :
    randomEngine_(randomDevice_()),
    Player(board)
{

}


Move RandomPlayer::getNextMove()
{
    possibleMoves_.clear();

    board_->getPossibleMoves(possibleMoves_);

    uniform_int_distribution<int> randomIndexDistribution(0, possibleMoves_.size()-1);
    return possibleMoves_[randomIndexDistribution(randomEngine_)];

}
