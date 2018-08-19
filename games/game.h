#ifndef GAME_H
#define GAME_H

#include "games/board.h"
#include "players/player.h"

class Game {
 public:
  Game(const std::shared_ptr<Board>& board,
       const std::shared_ptr<Player>& player_white,
       const std::shared_ptr<Player>& player_black);

  void Run();

 protected:
  std::shared_ptr<Board> board_;
  std::shared_ptr<Player> player_white_;
  std::shared_ptr<Player> player_black_;
};

#endif  // GAME_H
