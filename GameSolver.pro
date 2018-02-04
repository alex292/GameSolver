QT += core concurrent
QT -= gui

CONFIG += c++11

TARGET = GameSolver
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0



QMAKE_CXXFLAGS_RELEASE -= -O
QMAKE_CXXFLAGS_RELEASE -= -O1
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE *= -O3

SUBDIRS += \
    GameSolver.pro

HEADERS += \
    games/tictactoe/tictactoeboard.h \
    games/board.h \
    games/game.h \
    players/human/humanplayer.h \
    players/monteCarlo/montecarloplayer.h \
    players/monteCarlo/montecarlotree.h \
    players/monteCarlo/montecarlotreeexplorer.h \
    players/monteCarlo/montecarlotreenode.h \
    players/monteCarlo/montecarlotreenodecollection.h \
    players/random/randomplayer.h \
    players/player.h \
    types.h \
    games/connectFour3D/connectfour3dboard.h \
    games/zobristgenerator.h \
    games/connectedrowsboard.h \
    games/board3d.h

SOURCES += \
    games/tictactoe/tictactoeboard.cpp \
    games/board.cpp \
    games/game.cpp \
    players/human/humanplayer.cpp \
    players/monteCarlo/montecarloplayer.cpp \
    players/monteCarlo/montecarlotree.cpp \
    players/monteCarlo/montecarlotreeexplorer.cpp \
    players/monteCarlo/montecarlotreenode.cpp \
    players/monteCarlo/montecarlotreenodecollection.cpp \
    players/random/randomplayer.cpp \
    players/player.cpp \
    main.cpp \
    games/connectFour3D/connectfour3dboard.cpp \
    games/zobristgenerator.cpp \
    games/board3d.tpp \
    games/connectedrowsboard.tpp
