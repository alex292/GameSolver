#ifndef MONTECARLOTREENODE_H
#define MONTECARLOTREENODE_H

#include <atomic>

#include <QMutex>
#include <QReadWriteLock>
#include <QSet>

#include "games/board.h"
#include "montecarlotreenodecollection.h"
class MonteCarloTreeNodeCollection;

class MonteCarloTreeNode : public std::enable_shared_from_this<MonteCarloTreeNode> {
 public:
  MonteCarloTreeNode(const std::shared_ptr<const Board> &board);
  MonteCarloTreeNode(const std::shared_ptr<const Board> &board, const std::shared_ptr<MonteCarloTreeNode> &parent);

  ZobristValue GetZobristValue() const { return board_->GetZobristValue(); }

  bool IsLeafNode();
  bool IsExpandable();
  const std::shared_ptr<MonteCarloTreeNode> SelectNextBestChild();
  double GetUCTValue(unsigned int num_parent_evaluations);

  const std::shared_ptr<MonteCarloTreeNode> ExpandNextChild(const std::shared_ptr<MonteCarloTreeNodeCollection> &node_collection);

  const std::shared_ptr<const Board> GetBoard() const { return board_; }

  void AddParent(const std::shared_ptr<MonteCarloTreeNode> &parent);
  void RemoveExpiredParents();

  void StartBackpropagation(const std::shared_ptr<const Board> &playout_board);
  void BackpropagateWin(QSet<ZobristValue> &propagated_nodes);
  void BackpropagateLoss(QSet<ZobristValue> &propagated_nodes);
  void BackpropagateTie(QSet<ZobristValue> &propagated_nodes);

  Move GetBestMove();

  void LockExpansion();
  void UnlockExpansion();

  unsigned int GetNumEvaluations() const { return num_evaluations_; }
  unsigned int GetNumWins() const { return num_wins_; }

  std::vector<Move> unexplored_moves_;

  bool IsWinningState() { return is_winning_state_; }
  bool IsLosingState() { return is_losing_state_; }
  bool IsResultDecided() { return (is_winning_state_ || is_losing_state_); }

  void HasWinningMove(Move winning_move);
  void HasLosingMove();

 protected:
  std::shared_ptr<const Board> board_;

  std::vector<std::weak_ptr<MonteCarloTreeNode>> parents_;

  std::atomic<unsigned int> num_evaluations_;
  std::atomic<unsigned int> num_wins_;
  std::atomic<unsigned int> num_ties_;

  QMutex expansion_mutex_;

  QReadWriteLock explored_moves_lock_;
  QHash<Move, std::shared_ptr<MonteCarloTreeNode>> explored_moves_;

  QMutex result_decided_mutex_;
  std::atomic<bool> is_winning_state_ = {false};
  std::atomic<bool> is_losing_state_ = {false};
  std::atomic<Move> next_move_;
};

#endif  // MONTECARLOTREENODE_H
