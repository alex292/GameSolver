#ifndef ZOBRISTGENERATOR_H
#define ZOBRISTGENERATOR_H

#include "types.h"

#include <QSet>

#include <random>
#include <memory>
#include <vector>

class ZobristGenerator
{
public:
    static ZobristGenerator& getInstance();
    ZobristGenerator(ZobristGenerator const&) = delete;
    void operator=(ZobristGenerator const&) = delete;

    std::shared_ptr<std::vector<ZobristValue>> generateUniqueZobristValues(int num);

private:
    ZobristGenerator();

    QSet<ZobristValue> usedZobristValues_;
    std::random_device randomDevice_;
    std::mt19937_64 randomEngine_;
    std::uniform_int_distribution<ZobristValue> zobristDistribution_;
};

#endif // ZOBRISTGENERATOR_H
