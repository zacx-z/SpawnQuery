#include "SpawnQuery/SpawnQueryContext.h"

#include "SpawnQuery.h"

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

void USpawnQueryContext::PushCall(USpawnQuery* Query)
{
    QueryCallStack.Add(Query);
}

void USpawnQueryContext::PopCall(USpawnQuery* Query)
{
    int32 Num = QueryCallStack.Num();
    if (Num > 0 && QueryCallStack[Num - 1] == Query)
    {
        QueryCallStack.RemoveAt(Num - 1);
    }
}

bool USpawnQueryContext::HasQueryInCallStack(USpawnQuery* Query) const
{
    return QueryCallStack.Contains(Query);
}

FString USpawnQueryContext::GetCallStackInfo()
{
    FString CallStackInfo;
    for (int32 i = 0; i < QueryCallStack.Num(); ++i)
    {
        CallStackInfo += QueryCallStack[i]->GetName();
        CallStackInfo += TEXT("\n");
    }
    return CallStackInfo;
}
