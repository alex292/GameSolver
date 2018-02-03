#include "connectfour3dboard.h"

#include "games/zobristgenerator.h"

ConnectFour3DBoard::ConnectFour3DBoard()
    : Board(64, 16)
    , Board2DStacked(4, 4, 4)
    , ConnectedRowsBoard() {
  // zobrist values
  ZobristGenerator &zobrist_generator = ZobristGenerator::GetInstance();
  zobrist_values_positions_white_ = zobrist_generator.GenerateUniqueZobristValues(num_positions_);
  zobrist_values_positions_black_ = zobrist_generator.GenerateUniqueZobristValues(num_positions_);

  // init connected rows
  InitConnectedRows();
}

PositionIndex ConnectFour3DBoard::PerformMove(Move move) {
  PositionIndex position = move + 16 * stack_heights_[move];

  // set position
  stack_heights_[move] += 1;
  if (is_turn_white_) {
    positions_white_[position] = true;
    zobrist_value_ ^= zobrist_values_positions_white_->at(position);
  } else {
    positions_black_[position] = true;
    zobrist_value_ ^= zobrist_values_positions_black_->at(position);
  }

  return position;
}

std::shared_ptr<Board> ConnectFour3DBoard::Copy() const {
  return std::make_shared<ConnectFour3DBoard>(*this);
}

void ConnectFour3DBoard::GenerateRowsToPositions() {
  rows_to_positions_ = std::make_shared<std::vector<std::vector<PositionIndex>>>();

  // straight rows X,Y,Z
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      std::vector<PositionIndex> row_x;
      std::vector<PositionIndex> row_y;
      std::vector<PositionIndex> row_z;

      for (int k = 0; k < 4; ++k) {
        row_x.push_back(MapXYZtoPositionIndex(k, j, i));
        row_y.push_back(MapXYZtoPositionIndex(j, k, i));
        row_z.push_back(MapXYZtoPositionIndex(j, i, k));
      }

      rows_to_positions_->push_back(row_x);
      rows_to_positions_->push_back(row_y);
      rows_to_positions_->push_back(row_z);
    }
  }

  // 2D diagonals
  for (int i = 0; i < 4; ++i) {
    std::vector<PositionIndex> diag_x;
    std::vector<PositionIndex> diag_x_inv;

    std::vector<PositionIndex> diag_y;
    std::vector<PositionIndex> diag_y_inv;

    std::vector<PositionIndex> diag_z;
    std::vector<PositionIndex> diag_z_inv;

    for (int j = 0; j < 4; ++j) {
      diag_x.push_back(MapXYZtoPositionIndex(i, j, j));
      diag_x_inv.push_back(MapXYZtoPositionIndex(i, 3 - j, j));

      diag_y.push_back(MapXYZtoPositionIndex(j, i, j));
      diag_y_inv.push_back(MapXYZtoPositionIndex(j, i, 3 - j));

      diag_z.push_back(MapXYZtoPositionIndex(j, j, i));
      diag_z_inv.push_back(MapXYZtoPositionIndex(j, 3 - j, i));
    }

    rows_to_positions_->push_back(diag_x);
    rows_to_positions_->push_back(diag_x_inv);
    rows_to_positions_->push_back(diag_y);
    rows_to_positions_->push_back(diag_y_inv);
    rows_to_positions_->push_back(diag_z);
    rows_to_positions_->push_back(diag_z_inv);
  }

  // 3D diagonals
  std::vector<PositionIndex> diag_tl;
  std::vector<PositionIndex> diag_tr;
  std::vector<PositionIndex> diag_br;
  std::vector<PositionIndex> diag_bl;

  for (int i = 0; i < 4; ++i) {
    diag_tl.push_back(MapXYZtoPositionIndex(i, i, i));
    diag_tr.push_back(MapXYZtoPositionIndex(3 - i, i, i));
    diag_br.push_back(MapXYZtoPositionIndex(3 - i, 3 - i, i));
    diag_bl.push_back(MapXYZtoPositionIndex(i, 3 - i, i));
  }

  rows_to_positions_->push_back(diag_tl);
  rows_to_positions_->push_back(diag_tr);
  rows_to_positions_->push_back(diag_br);
  rows_to_positions_->push_back(diag_bl);
}
