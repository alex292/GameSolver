#ifndef MONTECARLOTREENODE_H
#define MONTECARLOTREENODE_H

#include <atomic>

#include <QMutex>
#include <QReadWriteLock>
#include <QSet>

#include "games/board.h"

class MonteCarloTreeNode {
 public:
  MonteCarloTreeNode(std::unique_ptr<const Board> board);

  ZobristValue GetZobristValue() const { return board_->GetZobristValue(); }

  bool IsLeafNode();
  bool IsExpandable();
  const std::shared_ptr<MonteCarloTreeNode> SelectNextBestChild();
  double GetUCTValue(unsigned int num_parent_evaluations) const;

  Move GetNextUnexploredMove();
  void AddExploredMove(Move move,
                       const std::shared_ptr<MonteCarloTreeNode>& node);

  void AddParent(Move move, const std::shared_ptr<MonteCarloTreeNode>& parent);
  void RemoveExpiredParents();

  const Board* GetBoard() const { return board_.get(); }

  Move GetBestMove();

  unsigned int GetNumEvaluations() const { return num_evaluations_; }
  unsigned int GetNumWins() const { return num_wins_; }

  bool IsLosingState() { return is_losing_state_; }
  bool IsWinningState() { return is_winning_state_; }
  bool IsResultDecided() { return (is_losing_state_ || is_winning_state_); }

  void HasWinningMove(Move winning_move);
  void HasLosingMove();

  void AddWin();
  void AddLoss();
  void AddTie();

 protected:
  std::unique_ptr<const Board> board_;

  QReadWriteLock parents_lock_;
  QHash<Move, std::weak_ptr<MonteCarloTreeNode>> parents_;

  std::atomic<unsigned int> num_evaluations_;
  std::atomic<unsigned int> num_wins_;
  std::atomic<unsigned int> num_ties_;

  QReadWriteLock unexplored_moves_lock_;
  std::vector<Move> unexplored_moves_;

  QReadWriteLock explored_moves_lock_;
  QHash<Move, std::shared_ptr<MonteCarloTreeNode>> explored_moves_;

  std::atomic<bool> is_losing_state_ = {false};
  std::atomic<bool> is_winning_state_ = {false};
  std::atomic<Move> winning_move_;
};

#endif  // MONTECARLOTREENODE_H
