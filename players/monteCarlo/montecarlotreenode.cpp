#include "montecarlotreenode.h"

#include <QDebug>
#include <QReadLocker>
#include "players/random/randomplayer.h"

MonteCarloTreeNode::MonteCarloTreeNode(std::unique_ptr<const Board> board)
    : board_(std::move(board)) {
  num_evaluations_ = 0;
  num_wins_ = 0;
  num_ties_ = 0;

  if (board_->IsGameOver())
    is_losing_state_ = true;
  else
    board_->GetPossibleMoves(unexplored_moves_);
}

bool MonteCarloTreeNode::IsLeafNode() {
  QReadLocker locker_unexplored(&unexplored_moves_lock_);
  if (!unexplored_moves_.empty())
    return true;
  locker_unexplored.unlock();

  QReadLocker locker_explored(&explored_moves_lock_);
  if (explored_moves_.empty())
    return true;

  return false;
}

bool MonteCarloTreeNode::IsExpandable() {
  QReadLocker unexplored_read_locker(&unexplored_moves_lock_);
  if (!unexplored_moves_.empty()) {
    // upgrade lock to write-lock, check again and return with lock if still not
    // empty
    unexplored_read_locker.unlock();
    unexplored_moves_lock_.lockForWrite();
    if (!unexplored_moves_.empty())
      return true;  // keep write lock active
    unexplored_moves_lock_.unlock();
    return false;
  }
  return false;
}

const std::shared_ptr<MonteCarloTreeNode>
MonteCarloTreeNode::SelectNextBestChild() {
  QReadLocker locker(&explored_moves_lock_);

  if (is_winning_state_)
    return explored_moves_.value(winning_move_);

  if (is_losing_state_)
    return explored_moves_.constBegin().value();

  unsigned int num_parent_evaluations = num_evaluations_;

  std::shared_ptr<MonteCarloTreeNode> next_node =
      explored_moves_.constBegin().value();
  double best_value = -1;

  QHashIterator<Move, std::shared_ptr<MonteCarloTreeNode>> iter(
      explored_moves_);
  while (iter.hasNext()) {
    iter.next();
    const std::shared_ptr<MonteCarloTreeNode>& n = iter.value();
    double value = n->GetUCTValue(num_parent_evaluations);

    if (value > best_value) {
      best_value = value;
      next_node = n;
    }
  }

  return next_node;
}

double MonteCarloTreeNode::GetUCTValue(
    unsigned int num_parent_evaluations) const {
  if (num_evaluations_ == 0)
    return 0;

  if (is_winning_state_)
    return 0;

  if (is_losing_state_)
    return std::numeric_limits<double>::infinity();

  double num_evaluations = static_cast<double>(num_evaluations_);
  return ((num_wins_ + 0.1 * num_ties_) / num_evaluations) +
         1.414213562 * sqrt(log(num_parent_evaluations) / num_evaluations);
}

Move MonteCarloTreeNode::GetNextUnexploredMove() {
  Move next_move = unexplored_moves_.back();
  unexplored_moves_.pop_back();
  return next_move;
}

void MonteCarloTreeNode::AddExploredMove(
    Move move,
    const std::shared_ptr<MonteCarloTreeNode>& node) {
  // add explored move
  QWriteLocker locker(&explored_moves_lock_);
  explored_moves_.insert(move, node);

  // unlock
  unexplored_moves_lock_.unlock();
  locker.unlock();

  // check for winning/losing moves
  if (node->IsLosingState())
    HasWinningMove(move);
}

void MonteCarloTreeNode::AddParent(
    Move move,
    const std::shared_ptr<MonteCarloTreeNode>& parent) {
  QWriteLocker locker(&parents_lock_);
  parents_.insert(move, parent);
}

void MonteCarloTreeNode::RemoveExpiredParents() {
  QWriteLocker locker(&parents_lock_);
  QMutableHashIterator<Move, std::weak_ptr<MonteCarloTreeNode>> iter(parents_);
  while (iter.hasNext()) {
    iter.next();
    if (iter.value().expired())
      iter.remove();
  }
}

Move MonteCarloTreeNode::GetBestMove() {
  if (is_winning_state_)
    return winning_move_;

  QReadLocker locker(&explored_moves_lock_);

  Move best_move = explored_moves_.constBegin().key();
  unsigned int best_move_num_evaluations = 0;

  QHashIterator<Move, std::shared_ptr<MonteCarloTreeNode>> iter(
      explored_moves_);
  while (iter.hasNext()) {
    iter.next();
    const std::shared_ptr<MonteCarloTreeNode>& n = iter.value();

    unsigned int numEvaluations = n->GetNumEvaluations();
    if (numEvaluations > best_move_num_evaluations) {
      best_move = iter.key();
      best_move_num_evaluations = numEvaluations;
    }
  }

  return best_move;
}

void MonteCarloTreeNode::HasWinningMove(Move winning_move) {
  if (is_winning_state_)
    return;

  winning_move_ = winning_move;
  is_winning_state_ = true;

  QReadLocker parents_locker(&parents_lock_);
  QHashIterator<Move, std::weak_ptr<MonteCarloTreeNode>> iter(parents_);
  while (iter.hasNext()) {
    iter.next();
    iter.value().lock()->HasLosingMove();
  }
}

void MonteCarloTreeNode::HasLosingMove() {
  if (is_losing_state_)
    return;

  // check if unexplored nodes exist
  QReadLocker unexplored_locker(&unexplored_moves_lock_);
  if (!unexplored_moves_.empty())
    return;
  unexplored_locker.unlock();

  // check if any child nodes are not winning
  QReadLocker explored_locker(&explored_moves_lock_);
  QHashIterator<Move, std::shared_ptr<MonteCarloTreeNode>> iter_explored(
      explored_moves_);
  while (iter_explored.hasNext()) {
    iter_explored.next();
    const std::shared_ptr<MonteCarloTreeNode>& n = iter_explored.value();
    if (!n->IsWinningState())
      return;
  }
  explored_locker.unlock();

  // all childs are winning -> this = losing state
  is_losing_state_ = true;

  QReadLocker parents_locker(&parents_lock_);
  QHashIterator<Move, std::weak_ptr<MonteCarloTreeNode>> iter_parents(parents_);
  while (iter_parents.hasNext()) {
    iter_parents.next();
    iter_parents.value().lock()->HasWinningMove(iter_parents.key());
  }
}

void MonteCarloTreeNode::AddWin() {
  num_evaluations_++;
  num_wins_++;
}

void MonteCarloTreeNode::AddLoss() {
  num_evaluations_++;
}

void MonteCarloTreeNode::AddTie() {
  num_evaluations_++;
  num_ties_++;
}
