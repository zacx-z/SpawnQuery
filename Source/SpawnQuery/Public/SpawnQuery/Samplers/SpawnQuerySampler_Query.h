#pragma once
#include "SpawnQuery/SpawnQueryNode_Sampler.h"
#include "SpawnQuerySampler_Query.generated.h"

class USpawnQuery;

/** Allows a SpawnQuery graph to use another graph as a sampler */
UCLASS(MinimalAPI)
class USpawnQuerySampler_Query : public USpawnQueryNode_Sampler
{
    GENERATED_BODY()

public:
    virtual bool IsActive(USpawnQueryContext& context) override;
    virtual TObjectPtr<USpawnEntryBase> Query(USpawnQueryContext& context) override;

public:

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TObjectPtr<USpawnQuery> QueryGraph;
};
