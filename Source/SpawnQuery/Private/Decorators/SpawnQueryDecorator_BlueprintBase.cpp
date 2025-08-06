#include "SpawnQuery/Decorators/SpawnQueryDecorator_BlueprintBase.h"

#include "BlueprintNodeHelpers.h"

USpawnQueryDecorator_BlueprintBase::USpawnQueryDecorator_BlueprintBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    const UClass* StopAtClass = USpawnQueryDecorator_BlueprintBase::StaticClass();
    HasReceiveCheckIsActiveImplementation = BlueprintNodeHelpers::HasBlueprintFunction(TEXT("ReceiveCheckIsActive"), *this, *StopAtClass);
    HasReceiveRewriteImplementation = BlueprintNodeHelpers::HasBlueprintFunction(TEXT("ReceiveRewrite"), *this, *StopAtClass);
    HasReceiveMutateWeightImplementation = BlueprintNodeHelpers::HasBlueprintFunction(TEXT("ReceiveMutateWeight"), *this, *StopAtClass);
}

bool USpawnQueryDecorator_BlueprintBase::IsActive(const USpawnQueryContext& Context)
{
    if (!HasReceiveCheckIsActiveImplementation) return true;

    return ReceiveCheckIsActive(&Context);
}

TObjectPtr<USpawnEntryBase> USpawnQueryDecorator_BlueprintBase::Rewrite(TObjectPtr<USpawnEntryBase> Result,
    USpawnQueryContext& Context)
{
    if (!HasReceiveRewriteImplementation) return Result;

    return ReceiveRewrite(Result, &Context);
}

float USpawnQueryDecorator_BlueprintBase::MutateWeight(float InWeight, const USpawnQueryContext& Context)
{
    if (!HasReceiveMutateWeightImplementation) return InWeight;

    return ReceiveMutateWeight(InWeight, &Context);
}
