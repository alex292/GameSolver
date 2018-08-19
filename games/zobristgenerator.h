#ifndef ZOBRISTGENERATOR_H
#define ZOBRISTGENERATOR_H

#include <QSet>

#include <memory>
#include <random>
#include <vector>

#include "types.h"

class ZobristGenerator {
 public:
  static ZobristGenerator& GetInstance();
  ZobristGenerator(ZobristGenerator const&) = delete;
  void operator=(ZobristGenerator const&) = delete;

  void GenerateUniqueZobristValues(unsigned int num,
                                   std::vector<ZobristValue>& values);

 private:
  ZobristGenerator();

  QSet<ZobristValue> used_zobrist_values_;
  std::random_device random_device_;
  std::mt19937_64 random_engine_;
  std::uniform_int_distribution<ZobristValue> zobrist_distribution_;
};

#endif  // ZOBRISTGENERATOR_H
