#ifndef BOARD_H
#define BOARD_H

#include <memory>
#include <vector>

#include <QSet>
#include <QSize>
#include <QString>

#include "types.h"

class Board {
 public:
  Board() {}
  virtual ~Board() {}

  virtual QString MoveToReadableMove(Move move) const = 0;
  virtual Move ReadableMoveToMove(const QString& readable_move,
                                  bool& valid) const = 0;

  virtual void GetPossibleMoves(std::vector<Move>& moves) const = 0;
  virtual bool IsMovePossible(Move move) const = 0;
  virtual bool HasRemainingMoves() = 0;

  void MakeMove(Move move);
  virtual PositionIndex PerformMove(Move move) = 0;
  virtual std::unique_ptr<Board> Copy() const = 0;

  bool IsGameOver() const { return state_ != GameState::OPEN; }
  bool IsWin(Color color) const;

  GameState state() const { return state_; }
  Color active_color() const { return active_color_; }
  ZobristValue zobrist_value() const { return zobrist_value_; }

 protected:
  GameState state_ = GameState::OPEN;
  Color active_color_ = Color::WHITE;
  ZobristValue zobrist_value_ = 0;

  virtual void CheckForGameOver(PositionIndex last_position) = 0;
};

#endif
