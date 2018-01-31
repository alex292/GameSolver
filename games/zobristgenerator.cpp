#include "zobristgenerator.h"

ZobristGenerator &ZobristGenerator::GetInstance() {
  static ZobristGenerator instance;
  return instance;
}

std::shared_ptr<std::vector<ZobristValue>> ZobristGenerator::GenerateUniqueZobristValues(int num) {
  std::shared_ptr<std::vector<ZobristValue>> values = std::make_shared<std::vector<ZobristValue>>();
  values->reserve(num);

  for (int i = 0; i < num; ++i) {
    ZobristValue value;
    do {
      value = zobrist_distribution_(random_engine_);
    } while (used_zobrist_values_.contains(value) && value != 0);
    values->push_back(value);
    used_zobrist_values_.insert(value);
  }

  return values;
}

ZobristGenerator::ZobristGenerator()
    : random_engine_(random_device_())
    , zobrist_distribution_(std::numeric_limits<ZobristValue>::min(), std::numeric_limits<ZobristValue>::max()) {}
