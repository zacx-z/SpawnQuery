#pragma once
#include "BehaviorTree/ValueOrBBKey.h"
#include "SpawnQuery/SpawnQueryNode_Decorator.h"
#include "SpawnQueryDecorator_WeightOverride.generated.h"

UCLASS(MinimalAPI)
class USpawnQueryDecorator_WeightOverride : public USpawnQueryNode_Decorator
{
    GENERATED_BODY()

public:

    SPAWNQUERY_API virtual FText GetDescriptionDetails() const override;

public:

    virtual bool IsActive(const USpawnQueryContext& Context) override;
    virtual double MutateWeight(double InWeight, const USpawnQueryContext& Context) override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FValueOrBBKey_Float Weight = 1.0f;
};
