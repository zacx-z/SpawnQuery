#include "SpawnQuery.h"
#include "SpawnQuery/SpawnQueryNode.h"
#include "SpawnQuery/SpawnQueryContext.h"

USpawnQuery::USpawnQuery(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
}

TObjectPtr<USpawnEntryBase> USpawnQuery::QueryEntry(USpawnQueryContext& context) const
{
    return RootNode->Query(context);
}

USpawnEntryBase* USpawnQuery::QueryEntry(USpawnQueryContext* context) const
{
    return QueryEntry(*context);
}
