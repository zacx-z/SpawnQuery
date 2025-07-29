#pragma once
#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "SpawnQuery/SpawnQueryNode_Sampler.h"
#include "SpawnQuerySampler_Pool.generated.h"

UCLASS(MinimalAPI)
class USpawnQuerySampler_Pool : public USpawnQueryNode_Sampler
{
    GENERATED_UCLASS_BODY()
    
public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (RequiredAssetDataTags = "RowStructure=/Script/SpawnQuery.SpawnEntryTableRowBase"))
    TObjectPtr<UDataTable> PoolTable;

public:
#if WITH_EDITOR
    //~ Begin UObject Interface
    virtual void PreEditChange(FProperty* PropertyAboutToChange) override;
    //~ End UObject Interface
#endif
    //~ Begin USpawnQueryNode Interface
    virtual bool IsActive(USpawnQueryContext& context) override;
    virtual TObjectPtr<USpawnEntryBase> Query(USpawnQueryContext& context) override;
    //~ End USpawnQueryNode Interface

private:

    void BuildWeightMap();
    int32 SearchEntryByWeightPosition(double WeightPosition);

    void OnPoolTableDataChanged();

    bool HasWeightMapBuilt = false;
    bool WeightMapDirty = false;
    TArray<float> WeightMap;
    double TotalWeights = 0;
};
