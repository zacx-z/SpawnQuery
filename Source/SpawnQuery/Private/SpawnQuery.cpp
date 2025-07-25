#include "SpawnQuery.h"
#include "SpawnQuery/SpawnQueryContext.h"

USpawnQuery::USpawnQuery(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

ISpawnEntry* USpawnQuery::QueryEntry(USpawnQueryContext& context)
{
	return nullptr;
}
