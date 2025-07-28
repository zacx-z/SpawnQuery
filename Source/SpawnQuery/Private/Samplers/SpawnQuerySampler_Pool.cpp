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
    if (!PoolTable->GetRowStruct()->IsChildOf(FSpawnEntryTableRowBase::StaticStruct()))
    {
        UE_LOG(LogBlueprint, Warning, TEXT("Mismatched row struct type in USpawnQuerySampler_Pool::Query"));
        return nullptr;
    }

    const int32 Num = PoolTable->GetRowMap().Num();
    const int32 Index = context.GetRandomStream().RandRange(0, Num - 1);

    TArray<FName> Names = PoolTable->GetRowNames();
    FName Name = Names[Index];
    FSpawnEntryTableRowBase* Row = PoolTable->FindRow<FSpawnEntryTableRowBase>(Name, "");
    USpawnEntryRowHandle* handle = NewObject<USpawnEntryRowHandle>(this); // maybe pool the struct because this function is frequently called
    handle->InitializeData(Row, Name, PoolTable);
    return handle;
}
