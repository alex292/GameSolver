#include "montecarlotreenode.h"

#include <QDebug>
#include <QReadLocker>
#include "players/random/randomplayer.h"

MonteCarloTreeNode::MonteCarloTreeNode(const std::shared_ptr<const Board> &board)
    : board_(board) {
  num_evaluations_ = 0;
  num_wins_ = 0;
  num_ties_ = 0;

  if (!board_->IsGameOver())
    board_->GetPossibleMoves(unexplored_moves_);
  else
    is_winning_state_ = true;
}

MonteCarloTreeNode::MonteCarloTreeNode(const std::shared_ptr<const Board> &board, const std::shared_ptr<MonteCarloTreeNode> &parent)
    : board_(board) {
  parents_.push_back(parent);
  num_evaluations_ = 0;
  num_wins_ = 0;

  if (!board_->IsGameOver())
    board_->GetPossibleMoves(unexplored_moves_);
}

bool MonteCarloTreeNode::IsLeafNode() {
  QReadLocker locker(&explored_moves_lock_);

  return (unexplored_moves_.size() != 0 || explored_moves_.size() == 0);
}

bool MonteCarloTreeNode::IsExpandable() {
  return (unexplored_moves_.size() > 0);
}

const std::shared_ptr<MonteCarloTreeNode> MonteCarloTreeNode::SelectNextBestChild() {
  if (is_losing_state_)
    return explored_moves_[next_move_];

  if (is_winning_state_)
    return explored_moves_.begin().value();

  unsigned int num_parent_evaluations = num_evaluations_;

  std::shared_ptr<MonteCarloTreeNode> next_node;
  double best_value = -1;

  QReadLocker locker(&explored_moves_lock_);
  QHashIterator<Move, std::shared_ptr<MonteCarloTreeNode>> iter(explored_moves_);
  while (iter.hasNext()) {
    iter.next();
    const std::shared_ptr<MonteCarloTreeNode> &n = iter.value();

    double value = n->GetUCTValue(num_parent_evaluations);

    if (value > best_value) {
      best_value = value;
      next_node = n;
    }
  }

  return next_node;
}

double MonteCarloTreeNode::GetUCTValue(unsigned int num_parent_evaluations) {
  if (num_evaluations_ == 0)
    return 0;

  if (is_losing_state_)
    return 0;

  if (is_winning_state_)
    return std::numeric_limits<double>::infinity();

  return ((num_wins_ + 0.1 * num_ties_) / (double)num_evaluations_) + 1.414213562 * sqrt(log((double)num_parent_evaluations) / num_evaluations_);
}

const std::shared_ptr<MonteCarloTreeNode> MonteCarloTreeNode::ExpandNextChild(const std::shared_ptr<MonteCarloTreeNodeCollection> &node_collection) {
  // get next move
  Move next_move = unexplored_moves_.back();
  unexplored_moves_.pop_back();

  // perform next move
  const std::shared_ptr<Board> next_board = board_->Copy();
  next_board->MakeMove(next_move);

  // get node for next move
  std::shared_ptr<MonteCarloTreeNode> next_node = node_collection->GetNode(next_board);
  next_node->AddParent(shared_from_this());

  // add explored move
  QWriteLocker locker(&explored_moves_lock_);
  explored_moves_.insert(next_move, next_node);

  expansion_mutex_.unlock();

  if (next_node->IsWinningState())
    HasWinningMove(next_move);

  return next_node;
}

void MonteCarloTreeNode::AddParent(const std::shared_ptr<MonteCarloTreeNode> &parent) {
  parents_.push_back(parent);
}

void MonteCarloTreeNode::RemoveExpiredParents() {
  parents_.erase(remove_if(parents_.begin(), parents_.end(), [](const std::weak_ptr<const MonteCarloTreeNode> &p) { return p.expired(); }), parents_.end());
}

void MonteCarloTreeNode::StartBackpropagation(const std::shared_ptr<const Board> &playout_board) {
  QSet<ZobristValue> propagated_nodes;

  if ((board_->IsTurnWhite() && playout_board->IsWinWhite()) || (!board_->IsTurnWhite() && playout_board->IsWinBlack()))
    BackpropagateLoss(propagated_nodes);
  else if ((board_->IsTurnWhite() && playout_board->IsWinBlack()) || (!board_->IsTurnWhite() && playout_board->IsWinWhite()))
    BackpropagateWin(propagated_nodes);
  else
    BackpropagateTie(propagated_nodes);
}

void MonteCarloTreeNode::BackpropagateWin(QSet<ZobristValue> &propagated_nodes) {
  ZobristValue zobrist_value = board_->GetZobristValue();
  if (propagated_nodes.contains(zobrist_value))
    return;
  propagated_nodes.insert(zobrist_value);

  num_evaluations_++;
  num_wins_++;

  for (const std::weak_ptr<MonteCarloTreeNode> &n : parents_) {
    if (!n.expired())
      n.lock()->BackpropagateLoss(propagated_nodes);
  }
}

void MonteCarloTreeNode::BackpropagateLoss(QSet<ZobristValue> &propagated_nodes) {
  ZobristValue zobrist_value = board_->GetZobristValue();
  if (propagated_nodes.contains(zobrist_value))
    return;
  propagated_nodes.insert(zobrist_value);

  num_evaluations_++;

  for (const std::weak_ptr<MonteCarloTreeNode> &n : parents_) {
    if (!n.expired())
      n.lock()->BackpropagateWin(propagated_nodes);
  }
}

void MonteCarloTreeNode::BackpropagateTie(QSet<ZobristValue> &propagated_nodes) {
  ZobristValue zobrist_value = board_->GetZobristValue();
  if (propagated_nodes.contains(zobrist_value))
    return;
  propagated_nodes.insert(zobrist_value);

  num_evaluations_++;
  num_ties_++;

  for (const std::weak_ptr<MonteCarloTreeNode> &n : parents_) {
    if (!n.expired())
      n.lock()->BackpropagateTie(propagated_nodes);
  }
}

Move MonteCarloTreeNode::GetBestMove() {
  if (is_losing_state_)
    return next_move_;

  Move best_move;
  int best_move_num_evaluations = -1;

  QReadLocker locker(&explored_moves_lock_);
  QHashIterator<Move, std::shared_ptr<MonteCarloTreeNode>> iter(explored_moves_);
  while (iter.hasNext()) {
    iter.next();
    const std::shared_ptr<MonteCarloTreeNode> &n = iter.value();

    int numEvaluations = n->GetNumEvaluations();
    if (numEvaluations > best_move_num_evaluations) {
      best_move = iter.key();
      best_move_num_evaluations = numEvaluations;
    }

    // qDebug() << "###" << iter.key() << iter.value()->getNumWins() << iter.value()->getNumEvaluations() << (iter.value()->getNumWins() / (double) iter.value()->getNumEvaluations());
  }

  return best_move;
}

void MonteCarloTreeNode::LockExpansion() {
  expansion_mutex_.lock();
}

void MonteCarloTreeNode::UnlockExpansion() {
  expansion_mutex_.unlock();
}

void MonteCarloTreeNode::HasWinningMove(Move winning_move) {
  QMutexLocker locker(&result_decided_mutex_);

  if (is_losing_state_)
    return;

  is_losing_state_ = true;
  next_move_ = winning_move;
  for (const std::weak_ptr<MonteCarloTreeNode> &n : parents_) {
    if (!n.expired())
      n.lock()->HasLosingMove();
  }
}

void MonteCarloTreeNode::HasLosingMove() {
  QMutexLocker locker(&result_decided_mutex_);

  if (is_winning_state_)
    return;

  QMutexLocker expansion_locker(&expansion_mutex_);
  if (!unexplored_moves_.empty())
    return;
  expansion_locker.unlock();

  bool all_moves_loosing = true;
  QReadLocker explored_locker(&explored_moves_lock_);
  QHashIterator<Move, std::shared_ptr<MonteCarloTreeNode>> iter(explored_moves_);
  while (iter.hasNext() && all_moves_loosing) {
    iter.next();
    const std::shared_ptr<MonteCarloTreeNode> &n = iter.value();
    all_moves_loosing &= n->IsLosingState();
  }
  explored_locker.unlock();

  if (all_moves_loosing) {
    is_winning_state_ = true;
    Move lastMove = board_->GetLastMove();
    for (const std::weak_ptr<MonteCarloTreeNode> &n : parents_) {
      if (!n.expired())
        n.lock()->HasWinningMove(lastMove);
    }
  }
}
