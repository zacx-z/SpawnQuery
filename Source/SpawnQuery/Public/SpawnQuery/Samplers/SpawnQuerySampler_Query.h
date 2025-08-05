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

    //~ Begin USpawnQueryNode Interface
    virtual FText GetDescriptionDetails() const override;

    virtual bool IsActive(const USpawnQueryContext& Context) override;
    virtual TObjectPtr<USpawnEntryBase> Query(USpawnQueryContext& Context) override;
    virtual FString GetErrorMessage() const override;
    //~ End USpawnQueryNode Interface

public:

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TObjectPtr<USpawnQuery> QueryGraph;
};
