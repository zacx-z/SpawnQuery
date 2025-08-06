#pragma once
#include "SpawnQuery/SpawnQueryNode_Sampler.h"
#include "SpawnQuerySampler_BlueprintBase.generated.h"

UCLASS(Abstract, Blueprintable, MinimalAPI)
class USpawnQuerySampler_BlueprintBase : public USpawnQueryNode_Sampler
{
    GENERATED_UCLASS_BODY()

public:
    SPAWNQUERY_API virtual bool IsActive(const USpawnQueryContext& Context) override;
    SPAWNQUERY_API virtual TObjectPtr<USpawnEntryBase> Query(USpawnQueryContext& Context) override;

    /**
     * Tells whether the sampler can yield an entry.
     */
    UFUNCTION(BlueprintImplementableEvent)
    SPAWNQUERY_API bool ReceiveCheckIsActive(const USpawnQueryContext* Context);
    /**
     * Yield an entry based on the state in Context.
     */
    UFUNCTION(BlueprintImplementableEvent)
    SPAWNQUERY_API USpawnEntryBase* ReceiveQuery(USpawnQueryContext* Context);

private:

    bool HasReceiveCheckIsActiveImplementation : 1;
    bool HasReceiveQueryImplementation : 1;
};
