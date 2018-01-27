#include "zobristgenerator.h"

using namespace std;

ZobristGenerator &ZobristGenerator::getInstance()
{
    static ZobristGenerator instance;
    return instance;
}

std::shared_ptr<std::vector<ZobristValue> > ZobristGenerator::generateUniqueZobristValues(int num)
{
    shared_ptr<vector<ZobristValue>> values = make_shared<vector<ZobristValue>>();
    values->reserve(num);

    for(int i=0; i<num; ++i) {
        ZobristValue value;
        do {
            value = zobristDistribution_(randomEngine_);
        } while(usedZobristValues_.contains(value));
        values->push_back(value);
        usedZobristValues_.insert(value);
    }

    return values;
}

ZobristGenerator::ZobristGenerator() :
    randomEngine_(randomDevice_()),
    zobristDistribution_(uniform_int_distribution<ZobristValue>(numeric_limits<ZobristValue>::min(), numeric_limits<ZobristValue>::max()))
{

}
