#include "zobristgenerator.h"

ZobristGenerator &ZobristGenerator::GetInstance() {
  static ZobristGenerator instance;
  return instance;
}

void ZobristGenerator::GenerateUniqueZobristValues(int num, std::vector<ZobristValue> &values) {
  values.clear();
  values.reserve(num);

  for (int i = 0; i < num; ++i) {
    ZobristValue value;
    do {
      value = zobrist_distribution_(random_engine_);
    } while (used_zobrist_values_.contains(value) && value != 0);
    values.push_back(value);
    used_zobrist_values_.insert(value);
  }
}

ZobristGenerator::ZobristGenerator()
    : random_engine_(random_device_())
    , zobrist_distribution_(std::numeric_limits<ZobristValue>::min(), std::numeric_limits<ZobristValue>::max()) {}
