#pragma once
#include "CoreMinimal.h"
#include "SpawnQueryTypes.h"
#include "UObject/ObjectMacros.h"
#include "SpawnQuery/SpawnQueryNode_Sampler.h"
#include "SpawnQuery/RandomizationTypes.h"
#include "SpawnQuerySampler_Pool.generated.h"

struct FSpawnEntryTableRowBase;

struct FSpawnerQueryPool_InfluencerEntry
{
    FName InfluencerName;
    float Factor;
};

struct FSpawnerQueryPool_EntryCache
{
    TArray<FSpawnerQueryPool_InfluencerEntry> Influencers;
    FSpawnEntryTableRowBase* Row;

    // for template algorithms
    bool IsActive(const USpawnQueryContext& Context);
    float GetWeight(USpawnQueryContext& Context);
};

UCLASS(MinimalAPI)
class USpawnQuerySampler_Pool : public USpawnQueryNode_Sampler
{
    GENERATED_UCLASS_BODY()
    
public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (RequiredAssetDataTags = "RowStructure=/Script/SpawnQuery.SpawnEntryTableRowBase"))
    TObjectPtr<UDataTable> PoolTable;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TEnumAsByte<ERandomizationPolicy> RandomizationPolicy;

public:

    //~ Begin UObject Interface
    virtual void PostLoad() override;
    virtual void BeginDestroy() override;
#if WITH_EDITOR
    virtual void PreEditChange(FProperty* PropertyAboutToChange) override;
    virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
    //~ End UObject Interface

    //~ Begin USpawnQueryNode Interface
    virtual FText GetDescriptionDetails() const override;

    virtual bool IsActive(const USpawnQueryContext& Context) override;
    virtual TObjectPtr<USpawnEntryBase> Query(USpawnQueryContext& Context) override;

#if WITH_EDITOR
    virtual void Refresh() override;
#endif
    //~ End USpawnQueryNode Interface

private:

    void BuildTableCache();
    void BuildWeightCache(const USpawnQueryContext& Context);
    int32 SearchEntryByWeightPosition(double WeightPosition);

    void OnPoolTableDataChanged();

    bool HasTableCacheBuilt = false;
    bool WeightMapDirty = false;
    bool UsingInfluencers;

    TArray<FSpawnerQueryPool_EntryCache> EntryCache;
    TArray<float> WeightMap;
    double TotalWeights = 0;
    int32 EntryNum;

#if WITH_EDITOR
    int32 CachedWorldID;
#endif
};

UCLASS()
class USpawnQuerySampler_Pool_State : public UObject
{
    GENERATED_BODY()

public:

    /**
     * Primarily for ShuffledSequence randomization
     */
    TArray<FRandomizationWeightState> WeightBase;
};
