#ifndef MONTECARLOTREENODE_H
#define MONTECARLOTREENODE_H

#include <atomic>

#include <QMutex>
#include <QReadWriteLock>
#include <QSet>

#include "games/board.h"
#include "montecarlotreenodecollection.h"

class MonteCarloTreeNodeCollection;

class MonteCarloTreeNode {
 public:
  MonteCarloTreeNode(const std::shared_ptr<const Board> &board);

  ZobristValue GetZobristValue() const { return board_->GetZobristValue(); }

  bool IsLeafNode();
  bool IsExpandable();
  const std::shared_ptr<MonteCarloTreeNode> SelectNextBestChild();
  double GetUCTValue(unsigned int num_parent_evaluations);

  Move GetNextUnexploredMove();
  void AddExploredMove(Move move, const std::shared_ptr<MonteCarloTreeNode> &node);

  void AddParent(Move move, const std::shared_ptr<MonteCarloTreeNode> &parent);
  void RemoveExpiredParents();

  void StartBackpropagation(const std::shared_ptr<const Board> &playout_board);
  void BackpropagateWin(QSet<ZobristValue> &propagated_nodes);
  void BackpropagateLoss(QSet<ZobristValue> &propagated_nodes);
  void BackpropagateTie(QSet<ZobristValue> &propagated_nodes);

  const std::shared_ptr<const Board> GetBoard() const { return board_; }

  Move GetBestMove();

  unsigned int GetNumEvaluations() const { return num_evaluations_; }
  unsigned int GetNumWins() const { return num_wins_; }

  bool IsLosingState() { return is_losing_state_; }
  bool IsWinningState() { return is_winning_state_; }
  bool IsResultDecided() { return (is_losing_state_ || is_winning_state_); }

  void HasWinningMove(Move winning_move);
  void HasLosingMove();

 protected:
  std::shared_ptr<const Board> board_;

  QReadWriteLock parents_lock_;
  QHash<Move, std::weak_ptr<MonteCarloTreeNode>> parents_;

  std::atomic<unsigned int> num_evaluations_;
  std::atomic<unsigned int> num_wins_;
  std::atomic<unsigned int> num_ties_;

  QMutex unexplored_moves_mutex_;
  std::vector<Move> unexplored_moves_;

  QReadWriteLock explored_moves_lock_;
  QHash<Move, std::shared_ptr<MonteCarloTreeNode>> explored_moves_;

  QMutex result_decided_mutex_;
  std::atomic<bool> is_losing_state_ = {false};
  std::atomic<bool> is_winning_state_ = {false};
  std::atomic<Move> winning_move_;
};

#endif  // MONTECARLOTREENODE_H
