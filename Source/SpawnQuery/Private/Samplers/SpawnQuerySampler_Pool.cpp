#include "SpawnQuery/Samplers/SpawnQuerySampler_Pool.h"
#include "SpawnQuery/SpawnQueryContext.h"
#include "SpawnQuery/Pools/SpawnEntryTableRow.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(SpawnQuerySampler_Pool)

USpawnQuerySampler_Pool::USpawnQuerySampler_Pool(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

bool USpawnQuerySampler_Pool::IsActive(USpawnQueryContext& context)
{
    return PoolTable != nullptr;
}

TObjectPtr<USpawnEntryBase> USpawnQuerySampler_Pool::Query(USpawnQueryContext& context)
{
    if (PoolTable == nullptr) return nullptr;
    if (!PoolTable->GetRowStruct()->IsChildOf(FSpawnEntryTableRow::StaticStruct()))
        return nullptr;

    const int32 num = PoolTable->GetRowMap().Num();
    const int32 index = context.GetRandomStream().RandRange(0, num - 1);

    TArray<FName> names = PoolTable->GetRowNames();
    FSpawnEntryTableRow* Row = PoolTable->FindRow<FSpawnEntryTableRow>(names[index], "");
    USpawnEntryRowHandle* handle = NewObject<USpawnEntryRowHandle>(this); // maybe pool the struct because this function is frequently called
    handle->InitializeData(Row, PoolTable);
    return handle;
}
