#include "SpawnQuery/SpawnQueryContext.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(SpawnQueryContext)

void USpawnQueryContext::PostLoad()
{
    UObject::PostLoad();

    RandomStream = FRandomStream(RandomSeed);
}

void USpawnQueryContext::PostInitProperties()
{
    UObject::PostInitProperties();

    RandomStream = FRandomStream(RandomSeed);
}

void USpawnQueryContext::Reset()
{
    RandomStream.Reset();
}
