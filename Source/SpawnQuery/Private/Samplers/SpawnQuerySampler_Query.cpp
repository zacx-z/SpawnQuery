#include "SpawnQuery/Samplers/SpawnQuerySampler_Query.h"

#include "SpawnQuery.h"
#include "SpawnQuery/SpawnQueryContext.h"

FText USpawnQuerySampler_Query::GetDescriptionDetails() const
{
    if (QueryGraph)
    {
        return FText::FromString(QueryGraph->GetName());
    }
    else
    {
        return FText::FromString(TEXT("Graph is not set"));
    }
}

bool USpawnQuerySampler_Query::IsActive(const USpawnQueryContext& Context)
{
    return QueryGraph != nullptr && QueryGraph->IsActive(Context);
}

TObjectPtr<USpawnEntryBase> USpawnQuerySampler_Query::Query(USpawnQueryContext& Context)
{
    if (QueryGraph == nullptr || !QueryGraph->IsActive(Context)) return nullptr;

    if (Context.HasQueryInCallStack(QueryGraph))
    {
        UE_LOG(LogBlueprint, Error, TEXT("Recursion detected in SpawnQuery graph '%s'. Call Stack: %s"), *QueryGraph->GetName(), *Context.GetCallStackInfo());

        return nullptr;
    }
    return QueryGraph->QueryEntry(Context);
}

FString USpawnQuerySampler_Query::GetErrorMessage() const
{
    if (QueryGraph == nullptr)
    {
        return TEXT("QueryGraph is not set");
    }
    return Super::GetErrorMessage();
}
