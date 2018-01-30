#include <algorithm>
#include <array>
#include <bitset>
#include <cstdlib>
#include <deque>
#include <vector>

#include <QCoreApplication>
#include <QDebug>
#include <QElapsedTimer>
#include <QFuture>
#include <QThread>
#include <QVector>
#include <QtConcurrent>

#include "games/connectFour3D/connectfour3dboard.h"
#include "games/game.h"
#include "games/tictactoe/tictactoeboard.h"
#include "players/human/humanplayer.h"
#include "players/monteCarlo/montecarloplayer.h"
#include "players/random/randomplayer.h"

int main(int argc, char *argv[]) {
  // arguments
  int time_per_move = 10000;  // 0000000;
  if (argc > 1)
    time_per_move = atoi(argv[1]);

  int num_threads = QThread::idealThreadCount();
  if (argc > 2)
    num_threads = atoi(argv[2]);

  qDebug() << "time per move" << time_per_move;
  qDebug() << "numThreads" << num_threads << "/" << QThread::idealThreadCount();

  std::shared_ptr<Board> board = std::make_shared<ConnectFour3DBoard>();

  std::shared_ptr<Player> player_white = std::make_shared<MonteCarloPlayer>(time_per_move, num_threads);
  std::shared_ptr<Player> player_black = std::make_shared<HumanPlayer>();

  Game game(board, player_white, player_black);
  game.Run();

  return 0;
}
