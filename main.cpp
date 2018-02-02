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

int GetUserInputString(const QString &command, const std::vector<QString> &possibleValues) {
  int index;
  QString word;
  bool is_number;
  do {
    std::cout << command.toStdString() << " [0 to " << (possibleValues.size() - 1) << "]:" << std::endl;
    for (int i = 0; i < possibleValues.size(); ++i)
      std::cout << "\t(" << i << ") " << possibleValues[i].toStdString() << std::endl;

    QTextStream qtin(stdin);

    qtin >> word;

    index = word.toInt(&is_number);

  } while (!is_number || index < 0 || index >= possibleValues.size());
  return index;
}

int GetUserInputInt(const QString &command, int min, int max) {
  int value;
  QString word;
  bool is_number;
  do {
    std::cout << command.toStdString() << " [" << min << " to " << max << "]:";

    QTextStream qtin(stdin);
    qtin >> word;

    value = word.toInt(&is_number);

  } while (!is_number || value < min || value > max);
  return value;
}

bool GetUserInputBool(const QString &command) {
  QString word;
  bool valid;
  do {
    std::cout << command.toStdString() << "[true|false]:";

    QTextStream qtin(stdin);
    qtin >> word;

    valid = (word == "true" || word == "false");

  } while (!valid);
  return (word == "true");
}

std::shared_ptr<Board> CreateBoard() {
  int index = GetUserInputString("Choose board", {"tic-tac-toe", "3D-connect-four"});
  if (index == 0)
    return std::make_shared<TicTacToeBoard>();
  else
    return std::make_shared<ConnectFour3DBoard>();
}

std::shared_ptr<Player> CreatePlayer(const QString &color, const std::shared_ptr<Player> &other_player = nullptr) {
  std::vector<QString> player_names{"human", "random", "monte-carlo"};
  if (other_player)
    player_names.push_back("copy");

  int index = GetUserInputString("Choose player " + color, player_names);
  if (index == 0) {
    return std::make_shared<HumanPlayer>();
  } else if (index == 1) {
    return std::make_shared<RandomPlayer>();
  } else if (index == 2) {
    int time_per_move = GetUserInputInt("time per move (ms)", -1, std::numeric_limits<int>::max());
    int num_threads = GetUserInputInt("num threads", 1, QThread::idealThreadCount());
    bool use_pondering = GetUserInputBool("use pondering");
    return std::make_shared<MonteCarloPlayer>(time_per_move, num_threads, use_pondering);
  } else {
    return other_player;
  }
}

int main(int argc, char *argv[]) {
  std::shared_ptr<Board> board = std::make_shared<ConnectFour3DBoard>();
  //  std::shared_ptr<Board> board = CreateBoard();

  std::shared_ptr<Player> player_white = std::make_shared<MonteCarloPlayer>(10000, QThread::idealThreadCount(), false);
  std::shared_ptr<Player> player_black = std::make_shared<MonteCarloPlayer>(10000, QThread::idealThreadCount(), false);
  // std::make_shared<MonteCarloPlayer>(1000, QThread::idealThreadCount(), false);
  //  std::shared_ptr<Player> player_white = CreatePlayer("white");
  //  std::shared_ptr<Player> player_black = CreatePlayer("black", player_white);

  Game game(board, player_white, player_black);
  game.Run();

  return 0;
}
