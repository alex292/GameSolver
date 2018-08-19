#include "connectfour3dboard.h"

#include "games/zobristgenerator.h"

bool ConnectFour3DBoard::initialized_static_data_ = false;
std::vector<std::vector<PositionIndex>> ConnectFour3DBoard::lines_to_positions_;
std::vector<std::vector<LineIndex>> ConnectFour3DBoard::positions_to_lines_;
std::vector<ZobristValue> ConnectFour3DBoard::zobrist_values_positions_white_;
std::vector<ZobristValue> ConnectFour3DBoard::zobrist_values_positions_black_;

ConnectFour3DBoard::ConnectFour3DBoard() {
  if (!initialized_static_data_)
    InitializeStaticData();
}

PositionIndex ConnectFour3DBoard::PerformMove(Move move) {
  PositionIndex position = GetResultingPositionIndex(move);

  // set position
  if (is_turn_white_) {
    positions_white_[position] = true;
    zobrist_value_ ^= zobrist_values_positions_white_[position];
  } else {
    positions_black_[position] = true;
    zobrist_value_ ^= zobrist_values_positions_black_[position];
  }

  return position;
}

std::unique_ptr<Board> ConnectFour3DBoard::Copy() const {
  return std::make_unique<ConnectFour3DBoard>(*this);
}

void ConnectFour3DBoard::InitializeStaticData() {
  // generate lines to positions (for ConnectedLinesBoard)
  // straight lines X,Y,Z
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      std::vector<PositionIndex> line_x;
      std::vector<PositionIndex> line_y;
      std::vector<PositionIndex> line_z;

      for (int k = 0; k < 4; ++k) {
        line_x.push_back(MapXYZtoPositionIndex(k, j, i));
        line_y.push_back(MapXYZtoPositionIndex(j, k, i));
        line_z.push_back(MapXYZtoPositionIndex(j, i, k));
      }

      lines_to_positions_.push_back(line_x);
      lines_to_positions_.push_back(line_y);
      lines_to_positions_.push_back(line_z);
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

    lines_to_positions_.push_back(diag_x);
    lines_to_positions_.push_back(diag_x_inv);
    lines_to_positions_.push_back(diag_y);
    lines_to_positions_.push_back(diag_y_inv);
    lines_to_positions_.push_back(diag_z);
    lines_to_positions_.push_back(diag_z_inv);
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

  lines_to_positions_.push_back(diag_tl);
  lines_to_positions_.push_back(diag_tr);
  lines_to_positions_.push_back(diag_br);
  lines_to_positions_.push_back(diag_bl);

  // generate positions to lines (for ConnectedLinesBoard)
  GeneratePositionsToLinesFromLinesToPositions(lines_to_positions_,
                                               positions_to_lines_);

  // zobrist values
  ZobristGenerator& zobrist_generator = ZobristGenerator::GetInstance();
  zobrist_generator.GenerateUniqueZobristValues(
      num_positions_, zobrist_values_positions_white_);
  zobrist_generator.GenerateUniqueZobristValues(
      num_positions_, zobrist_values_positions_black_);

  // mark as initialized
  initialized_static_data_ = true;
}

const std::vector<PositionIndex>& ConnectFour3DBoard::GetPositionsForLine(
    LineIndex line) {
  return lines_to_positions_[line];
}

const std::vector<LineIndex>& ConnectFour3DBoard::GetLinesForPosition(
    PositionIndex position) {
  return positions_to_lines_[position];
}
