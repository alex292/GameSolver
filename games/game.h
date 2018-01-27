#ifndef GAME_H
#define GAME_H

#include "games/board.h"
#include "players/player.h"

class Game
{
public:
    Game(const std::shared_ptr<Board> &board, const std::shared_ptr<Player> &playerWhite, const std::shared_ptr<Player> &playerBlack);

    void run();

protected:
    std::shared_ptr<Board> board_;
    std::shared_ptr<Player> playerWhite_;
    std::shared_ptr<Player> playerBlack_;
};

#endif // GAME_H
