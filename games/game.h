#ifndef GAME_H
#define GAME_H

#include "games/board.h"
#include "players/player.h"

class Game {
 public:
  Game(std::unique_ptr<Board> board,
       std::unique_ptr<Player> player_white,
       std::unique_ptr<Player> player_black);

  void Run();

 protected:
  std::unique_ptr<Board> board_;
  std::unique_ptr<Player> player_white_;
  std::unique_ptr<Player> player_black_;
};

#endif  // GAME_H
