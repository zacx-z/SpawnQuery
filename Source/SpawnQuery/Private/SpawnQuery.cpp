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
    return Context.IsSpawnQueryActive(this, bActiveByDefault);
}

void USpawnQuery::SetActiveState(bool bActiveState, USpawnQueryContext& Context) const
{
    Context.SetSpawnQueryActiveState(this, bActiveState);
}

TObjectPtr<USpawnEntryBase> USpawnQuery::QueryEntry(USpawnQueryContext& Context) const
{
    if (!IsActive(Context))
        return nullptr;

    return RootNode->GetQueryResult(Context);
}

bool USpawnQuery::IsActive(const USpawnQueryContext* Context)
{
    if (Context == nullptr)
    {
        Context = GetGlobalContext();
    }

    return IsActive(*Context);
}

void USpawnQuery::SetActiveState(bool bActiveState, USpawnQueryContext* Context)
{
    if (Context == nullptr)
    {
        Context = GetGlobalContext();
    }

    SetActiveState(bActiveState, *Context);
}

USpawnEntryBase* USpawnQuery::QueryEntry(USpawnQueryContext* Context)
{
    if (Context == nullptr)
    {
        Context = GetGlobalContext();
    }
    return QueryEntry(*Context);
}

USpawnQueryContext* USpawnQuery::GetGlobalContext()
{
    if (CachedGlobalContext == nullptr)
    {
        FSpawnQueryModule& SpawnQueryEditorModule = FModuleManager::LoadModuleChecked<FSpawnQueryModule>("SpawnQuery");
        CachedGlobalContext = SpawnQueryEditorModule.GetDefaultContext();
    }

    return CachedGlobalContext.Get();
}
