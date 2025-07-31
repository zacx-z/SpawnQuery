#pragma once
#include "SpawnQueryTypes.h"
#include "SpawnQuery/SpawnQueryNode_Composite.h"
#include "SpawnQuery/RandomizationTypes.h"
#include "SpawnQueryComposite_RandomSelector.generated.h"

UCLASS(MinimalAPI)
class USpawnQueryComposite_RandomSelector : public USpawnQueryNode_Composite
{
    GENERATED_BODY()

public:
    virtual TObjectPtr<USpawnEntryBase> Query(USpawnQueryContext& Context) override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TEnumAsByte<ERandomizationPolicy> RandomizationPolicy;
};

UCLASS()
class USpawnQueryComposite_RandomSelector_State : public UObject
{
    GENERATED_BODY()

public:

    /**
     * Primarily for ShuffledSequence randomization
     */
    TArray<FRandomizationWeightState> WeightBase;
};
