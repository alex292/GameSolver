#include <algorithm>
#include <array>
#include <bitset>
#include <cstdlib>
#include <deque>
#include <iostream>
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

QString GetUserInputString(const QString &command, const QStringList &possibleValues) {
  QString word;
  do {
    std::cout << command.toStdString() << " [" << possibleValues.join(", ").toStdString() << "]:";

    QTextStream qtin(stdin);

    qtin >> word;
  } while (!possibleValues.contains(word));
  return word;
}

int GetUserInputInt(const QString &command, int min, int max) {
  int value;
  QString word;
  bool is_number;
  do {
    std::cout << command.toStdString() << " [" << min << "-" << max << "]:";

    QTextStream qtin(stdin);
    qtin >> word;

    value = word.toInt(&is_number);

  } while (!is_number || value < min || value > max);
  return value;
}

std::shared_ptr<Board> CreateBoard() {
  QString board_name = GetUserInputString("Choose board", {"tic-tac-toe", "3D-connect-four"});
  if (board_name == "tic-tac-toe")
    return std::make_shared<TicTacToeBoard>();
  else
    return std::make_shared<ConnectFour3DBoard>();
}

std::shared_ptr<Player> CreatePlayer(const QString &color) {
  QString player_name = GetUserInputString("Choose player " + color, {"human", "random", "monte-carlo"});
  if (player_name == "human") {
    return std::make_shared<HumanPlayer>();
  } else if (player_name == "random") {
    return std::make_shared<RandomPlayer>();
  } else {
    int time_per_move = GetUserInputInt("time per move (ms)", 1, std::numeric_limits<int>::max());
    int num_threads = GetUserInputInt("num threads", 1, QThread::idealThreadCount());
    return std::make_shared<MonteCarloPlayer>(time_per_move, num_threads);
  }
}

int main(int argc, char *argv[]) {
  // std::shared_ptr<Board> board = std::make_shared<ConnectFour3DBoard>();
  std::shared_ptr<Board> board = CreateBoard();

  // std::shared_ptr<Player> player_white = std::make_shared<MonteCarloPlayer>(time_per_move, num_threads);
  std::shared_ptr<Player> player_white = CreatePlayer("white");
  // std::shared_ptr<Player> player_black = std::make_shared<HumanPlayer>();
  std::shared_ptr<Player> player_black = CreatePlayer("black");

  Game game(board, player_white, player_black);
  game.Run();

  return 0;
}
