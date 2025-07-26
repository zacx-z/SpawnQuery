#include "SpawnQuery.h"
#include "SpawnQuery/SpawnQueryNode.h"
#include "SpawnQuery/SpawnQueryContext.h"

USpawnQuery::USpawnQuery(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

TSharedPtr<ISpawnEntryBase> USpawnQuery::QueryEntry(USpawnQueryContext& context)
{
	return RootNode->Query(context);
}
