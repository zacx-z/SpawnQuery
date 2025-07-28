#include "SpawnQuery.h"

#include "SpawnQueryModule.h"
#include "SpawnQuery/SpawnQueryNode.h"
#include "SpawnQuery/SpawnQueryContext.h"

USpawnQuery::USpawnQuery(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
    , bActiveByDefault(true)
{
}

bool USpawnQuery::IsActive(const USpawnQueryContext& Context) const
{
    return bActiveByDefault;
}

TObjectPtr<USpawnEntryBase> USpawnQuery::QueryEntry(USpawnQueryContext& context) const
{
    if (!IsActive(context))
        return nullptr;

    return RootNode->Query(context);
}

USpawnEntryBase* USpawnQuery::QueryEntry(USpawnQueryContext* context) const
{
    if (context == nullptr)
    {
        FSpawnQueryModule& SpawnQueryEditorModule = FModuleManager::LoadModuleChecked<FSpawnQueryModule>("SpawnQuery");
        context = SpawnQueryEditorModule.GetDefaultContext();
    }
    return QueryEntry(*context);
}
