#ifndef BOARD_H
#define BOARD_H

#include <array>
#include <memory>
#include <vector>

#include <QBitArray>
#include <QSet>
#include <QSize>
#include <QString>
#include <QVector>

#include "types.h"

class Board {
 public:
  Board(int num_positions, int num_moves);

  virtual QString MoveToReadableMove(Move move) const = 0;
  virtual Move ReadableMoveToMove(const QString &readable_move, bool &valid) const = 0;

  virtual void GetPossibleMoves(std::vector<Move> &moves) const = 0;
  virtual bool IsMovePossible(Move move) const = 0;
  virtual bool HasRemainingMoves() = 0;

  void MakeMove(Move move);
  virtual PositionIndex PerformMove(Move move) = 0;
  virtual std::shared_ptr<Board> Copy() const = 0;

  bool IsTurnWhite() const { return is_turn_white_; }
  bool IsGameOver() const { return is_game_over_; }
  bool IsWinWhite() const { return is_win_white_; }
  bool IsWinBlack() const { return is_win_black_; }
  bool IsTie() const { return is_game_over_ && !is_win_white_ && !is_win_black_; }

  ZobristValue GetZobristValue() const { return zobrist_value_; }

 protected:
  const int num_positions_;
  const int num_moves_;

  bool is_turn_white_ = true;
  bool is_game_over_ = false;
  bool is_win_white_ = false;
  bool is_win_black_ = false;

  ZobristValue zobrist_value_ = 0;

  QBitArray positions_white_;
  QBitArray positions_black_;

  virtual void CheckForGameOver(PositionIndex last_position) = 0;
};

#endif
