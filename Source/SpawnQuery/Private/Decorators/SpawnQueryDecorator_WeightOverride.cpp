#include "SpawnQuery/Decorators/SpawnQueryDecorator_WeightOverride.h"
#include "SpawnQuery/SpawnQueryContext.h"

FText USpawnQueryDecorator_WeightOverride::GetDescriptionDetails() const
{
    return FText::FromString(FString::Format(TEXT("Weight: {0}"), { Weight.ToString() }));
}

bool USpawnQueryDecorator_WeightOverride::IsActive(const USpawnQueryContext& Context)
{
    return Weight.GetValue(Context.GetBlackboard()) > 0;
}

double USpawnQueryDecorator_WeightOverride::MutateWeight(double InWeight, const USpawnQueryContext& Context)
{
    return Weight.GetValue(Context.GetBlackboard());
}
