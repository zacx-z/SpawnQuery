#pragma once
#include "SpawnQuery/SpawnQueryNode_Sampler.h"
#include "SpawnQuerySampler_BlueprintBase.generated.h"

UCLASS(Abstract, Blueprintable, MinimalAPI)
class USpawnQuerySampler_BlueprintBase : public USpawnQueryNode_Sampler
{
    GENERATED_BODY()

public:
    SPAWNQUERY_API virtual bool IsActive(const USpawnQueryContext& Context) override;
    SPAWNQUERY_API virtual TObjectPtr<USpawnEntryBase> Query(USpawnQueryContext& Context) override;

    UFUNCTION(BlueprintImplementableEvent)
    SPAWNQUERY_API bool ReceiveCheckIsActive(const USpawnQueryContext* Context);
    UFUNCTION(BlueprintImplementableEvent)
    SPAWNQUERY_API USpawnEntryBase* ReceiveQuery(USpawnQueryContext* Context);
};
