#include "SpawnQuery/Samplers/SpawnQuerySampler_Query.h"

#include "SpawnQuery.h"
#include "SpawnQuery/SpawnQueryContext.h"

bool USpawnQuerySampler_Query::IsActive(const USpawnQueryContext& context)
{
    return QueryGraph != nullptr && QueryGraph->IsActive(context);
}

TObjectPtr<USpawnEntryBase> USpawnQuerySampler_Query::Query(USpawnQueryContext& context)
{
    if (QueryGraph == nullptr || !QueryGraph->IsActive(context)) return nullptr;

    if (context.HasQueryInCallStack(QueryGraph))
    {
        UE_LOG(LogBlueprint, Error, TEXT("Recursion detected in SpawnQuery graph '%s'. Call Stack: %s"), *QueryGraph->GetName(), *context.GetCallStackInfo());

        return nullptr;
    }
    return QueryGraph->QueryEntry(context);
}
