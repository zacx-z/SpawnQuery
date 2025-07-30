#include "SpawnQuery/Decorators/SpawnQueryDecorator_WeightOverride.h"

double USpawnQueryDecorator_WeightOverride::MutateWeight(double InWeight)
{
    return Weight;
}
