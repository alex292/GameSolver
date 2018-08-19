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

unsigned int GetUserInputString(const QString& command,
                                const std::vector<QString>& possibleValues) {
  unsigned int index;
  QString word;
  bool is_number;
  do {
    std::cout << command.toStdString() << " [0 to "
              << (possibleValues.size() - 1) << "]:" << std::endl;
    for (size_t i = 0; i < possibleValues.size(); ++i)
      std::cout << "\t(" << i << ") " << possibleValues[i].toStdString()
                << std::endl;

    QTextStream qtin(stdin);

    qtin >> word;

    index = word.toUInt(&is_number);

  } while (!is_number || index >= possibleValues.size());
  return index;
}

int GetUserInputInt(const QString& command, int min, int max) {
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

bool GetUserInputBool(const QString& command) {
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

std::unique_ptr<Board> CreateBoard() {
  unsigned int index =
      GetUserInputString("Choose board", {"tic-tac-toe", "3D-connect-four"});
  if (index == 0)
    return std::make_unique<TicTacToeBoard>();
  else
    return std::make_unique<ConnectFour3DBoard>();
}

std::unique_ptr<Player> CreatePlayer(const QString& color,
                                     Player* other_player = nullptr) {
  std::vector<QString> player_names{"human", "random", "monte-carlo"};
  if (other_player)
    player_names.push_back("copy");

  unsigned int index =
      GetUserInputString("Choose player " + color, player_names);
  if (index == 0) {
    return std::make_unique<HumanPlayer>();
  } else if (index == 1) {
    return std::make_unique<RandomPlayer>();
  } else if (index == 2) {
    int time_per_move = GetUserInputInt("time per move (ms)", -1,
                                        std::numeric_limits<int>::max());
    int num_threads =
        GetUserInputInt("num threads", 1, QThread::idealThreadCount());
    bool use_pondering = GetUserInputBool("use pondering");
    return std::make_unique<MonteCarloPlayer>(time_per_move, num_threads,
                                              use_pondering);
  } else {
    return std::unique_ptr<Player>(
        other_player);  // dangerous hack (shared ownership of same pointer)
  }
}

int main(int, char*[]) {
  //  std::shared_ptr<Board> board = std::make_shared<ConnectFour3DBoard>();
  std::unique_ptr<Board> board = CreateBoard();

  std::unique_ptr<Player> player_white = CreatePlayer("white");
  std::unique_ptr<Player> player_black =
      CreatePlayer("black", player_white.get());

  Game game(std::move(board), std::move(player_white), std::move(player_black));
  game.Run();

  return 0;
}
