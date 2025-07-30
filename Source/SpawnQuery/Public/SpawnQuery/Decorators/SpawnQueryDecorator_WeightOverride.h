#pragma once
#include "SpawnQuery/SpawnQueryNode_Decorator.h"
#include "SpawnQueryDecorator_WeightOverride.generated.h"

UCLASS(MinimalAPI)
class USpawnQueryDecorator_WeightOverride : public USpawnQueryNode_Decorator
{
    GENERATED_BODY()

public:

    virtual double MutateWeight(double InWeight) override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    double Weight = 1.0;
};
