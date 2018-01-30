#include "montecarlotreenode.h"

#include <QDebug>
#include <QReadLocker>
#include "players/random/randomplayer.h"

MonteCarloTreeNode::MonteCarloTreeNode(const std::shared_ptr<const Board> &board)
    : board_(board) {
  num_evaluations_ = 0;
  num_wins_ = 0;
  num_ties_ = 0;

  if (board_->IsGameOver())
    is_losing_state_ = true;
  else
    board_->GetPossibleMoves(unexplored_moves_);
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
  QMutexLocker locker_unexplored(&unexplored_moves_mutex_);
  if (!unexplored_moves_.empty())
    return true;

  QReadLocker locker_explored(&explored_moves_lock_);
  if (explored_moves_.empty())
    return true;

  return false;
}

bool MonteCarloTreeNode::IsExpandable() {
  unexplored_moves_mutex_.lock();
  if (!unexplored_moves_.empty()) {
    return true;  // keep lock!
  }

  unexplored_moves_mutex_.unlock();
  return false;

  return (unexplored_moves_.size() > 0);
}

const std::shared_ptr<MonteCarloTreeNode> MonteCarloTreeNode::SelectNextBestChild() {
  if (is_winning_state_)
    return explored_moves_[winning_move_];

  if (is_losing_state_)
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

  if (is_winning_state_)
    return 0;

  if (is_losing_state_)
    return std::numeric_limits<double>::infinity();

  return ((num_wins_ + 0.1 * num_ties_) / (double)num_evaluations_) + 1.414213562 * sqrt(log((double)num_parent_evaluations) / num_evaluations_);
}

Move MonteCarloTreeNode::GetNextUnexploredMove() {
  Move next_move = unexplored_moves_.back();
  unexplored_moves_.pop_back();
  return next_move;
}

void MonteCarloTreeNode::AddExploredMove(const std::shared_ptr<MonteCarloTreeNode> &node) {
  // add explored move
  Move move = node->GetBoard()->GetLastMove();
  QWriteLocker locker(&explored_moves_lock_);
  explored_moves_.insert(move, node);

  // unlock expansion
  unexplored_moves_mutex_.unlock();

  // check for winning/losing moves
  if (node->IsLosingState())
    HasWinningMove(move);
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
  if (is_winning_state_)
    return winning_move_;

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

void MonteCarloTreeNode::HasWinningMove(Move winning_move) {
  QMutexLocker locker(&result_decided_mutex_);

  if (is_winning_state_)
    return;

  is_winning_state_ = true;
  winning_move_ = winning_move;
  for (const std::weak_ptr<MonteCarloTreeNode> &n : parents_) {
    if (!n.expired())
      n.lock()->HasLosingMove();
  }
}

void MonteCarloTreeNode::HasLosingMove() {
  QMutexLocker locker(&result_decided_mutex_);

  if (is_losing_state_)
    return;

  // check if unexplored nodes exist
  QMutexLocker unexplored_locker(&unexplored_moves_mutex_);
  if (!unexplored_moves_.empty())
    return;
  unexplored_locker.unlock();

  // check if any child nodes are not winning
  QReadLocker explored_locker(&explored_moves_lock_);
  QHashIterator<Move, std::shared_ptr<MonteCarloTreeNode>> iter(explored_moves_);
  while (iter.hasNext()) {
    iter.next();
    const std::shared_ptr<MonteCarloTreeNode> &n = iter.value();
    if (!n->IsWinningState())
      return;
  }
  explored_locker.unlock();

  // all childs are winning -> this = losing state
  is_losing_state_ = true;
  Move lastMove = board_->GetLastMove();
  for (const std::weak_ptr<MonteCarloTreeNode> &n : parents_) {
    if (!n.expired())
      n.lock()->HasWinningMove(lastMove);
  }
}
