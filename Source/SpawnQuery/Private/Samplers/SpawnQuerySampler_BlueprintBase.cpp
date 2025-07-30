#include "SpawnQuery/Samplers/SpawnQuerySampler_BlueprintBase.h"

bool USpawnQuerySampler_BlueprintBase::IsActive(const USpawnQueryContext& Context)
{
    return ReceiveCheckIsActive(&Context);
}

TObjectPtr<USpawnEntryBase> USpawnQuerySampler_BlueprintBase::Query(USpawnQueryContext& Context)
{
    return ReceiveQuery(&Context);
}
