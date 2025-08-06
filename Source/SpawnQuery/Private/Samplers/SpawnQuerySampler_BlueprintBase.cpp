#include "SpawnQuery/Samplers/SpawnQuerySampler_BlueprintBase.h"

#include "BlueprintNodeHelpers.h"

USpawnQuerySampler_BlueprintBase::USpawnQuerySampler_BlueprintBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    const UClass* StopAtClass = USpawnQuerySampler_BlueprintBase::StaticClass();
    HasReceiveCheckIsActiveImplementation = BlueprintNodeHelpers::HasBlueprintFunction(TEXT("ReceiveCheckIsActive"), *this, *StopAtClass);
    HasReceiveQueryImplementation = BlueprintNodeHelpers::HasBlueprintFunction(TEXT("ReceiveQuery"), *this, *StopAtClass);
}

bool USpawnQuerySampler_BlueprintBase::IsActive(const USpawnQueryContext& Context)
{
    if (!HasReceiveCheckIsActiveImplementation) return HasReceiveQueryImplementation;

    return ReceiveCheckIsActive(&Context);
}

TObjectPtr<USpawnEntryBase> USpawnQuerySampler_BlueprintBase::Query(USpawnQueryContext& Context)
{
    return ReceiveQuery(&Context);
}
