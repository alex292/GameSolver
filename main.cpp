#include <QCoreApplication>

#include <vector>
#include <deque>
#include <array>
#include <algorithm>
#include <cstdlib>
#include <bitset>

#include <QElapsedTimer>
#include <QDebug>
#include <QVector>
#include <QThread>
#include <QFuture>
#include <QtConcurrent>

#include "players/random/randomplayer.h"
#include "players/human/humanplayer.h"
#include "players/monteCarlo/montecarloplayer.h"
#include "games/tictactoe/tictactoeboard.h"
#include "games/connectFour3D/connectfour3dboard.h"
#include "games/game.h"

using namespace std;

int main(int argc, char *argv[])
{
    // arguments
    int timePerMove = 100000000000;
    if(argc > 1)
        timePerMove = atoi(argv[1]);

    int numThreads = QThread::idealThreadCount();
    if(argc > 2)
        numThreads = atoi(argv[2]);

    qDebug() << "time per move" << timePerMove;
    qDebug() << "numThreads" << numThreads << "/" << QThread::idealThreadCount();

    shared_ptr<Board> board = make_shared<ConnectFour3DBoard>();

    shared_ptr<Player> playerWhite = make_shared<MonteCarloPlayer>(board, timePerMove, numThreads);
    shared_ptr<Player> playerBlack = make_shared<HumanPlayer>(board);

    Game game(board, playerWhite, playerBlack);
    game.run();

    return 0;
}
