#include "SpawnQuery/Decorators/SpawnQueryDecorator_WeightOverride.h"

FText USpawnQueryDecorator_WeightOverride::GetDescriptionDetails() const
{
    return FText::FromString(FString::Format(TEXT("Weight: {0}"), { Weight }));
}

bool USpawnQueryDecorator_WeightOverride::IsActive(const USpawnQueryContext& Context)
{
    return Weight > 0;
}

double USpawnQueryDecorator_WeightOverride::MutateWeight(double InWeight)
{
    return Weight;
}
