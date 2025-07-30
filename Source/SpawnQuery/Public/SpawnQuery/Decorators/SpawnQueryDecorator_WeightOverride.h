#pragma once
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
    virtual double MutateWeight(double InWeight) override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    double Weight = 1.0;
};
