#include "SpawnQuery/Samplers/SpawnQuerySampler_Pool.h"
#include "SpawnQuery/SpawnQueryContext.h"
#include "SpawnQuery/Pools/SpawnEntryTableRow.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(SpawnQuerySampler_Pool)

USpawnQuerySampler_Pool::USpawnQuerySampler_Pool(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

#if WITH_EDITOR

void USpawnQuerySampler_Pool::PreEditChange(FProperty* PropertyAboutToChange)
{
    Super::PreEditChange(PropertyAboutToChange);

    if (PropertyAboutToChange && PropertyAboutToChange->GetName() == GET_MEMBER_NAME_CHECKED(USpawnQuerySampler_Pool, PoolTable))
    {
        PoolTable->OnDataTableChanged().RemoveAll(this);
        HasWeightMapBuilt = false;
    }
}

#endif

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

    if (!HasWeightMapBuilt || WeightMapDirty)
    {
        BuildWeightMap();
    }

    const double WeightPosition = context.GetRandomStream().FRandRange(0, TotalWeights);

    int32 Index = SearchEntryByWeightPosition(WeightPosition);

    TArray<FName> Names = PoolTable->GetRowNames();
    FName Name = Names[Index];
    FSpawnEntryTableRowBase* Row = reinterpret_cast<FSpawnEntryTableRowBase*>(PoolTable->FindRowUnchecked(Name));

    USpawnEntryRowHandle* handle = NewObject<USpawnEntryRowHandle>(this); // maybe pool the struct because this function is frequently called
    handle->InitializeData(Row, Name, PoolTable);
    return handle;
}

void USpawnQuerySampler_Pool::BuildWeightMap()
{
    TotalWeights = 0;
    WeightMap.Reset();

    auto RowMap = PoolTable->GetRowMap();

    for (auto Row : RowMap)
    {
        TotalWeights += reinterpret_cast<FSpawnEntryTableRowBase*>(Row.Value)->Weight;
        WeightMap.Add(TotalWeights);
    }

    if (!HasWeightMapBuilt)
    {
        // first time building a weight map
        PoolTable->OnDataTableChanged().AddUObject(this, &USpawnQuerySampler_Pool::OnPoolTableDataChanged);
    }

    HasWeightMapBuilt = true;
    WeightMapDirty = false;
}

int32 USpawnQuerySampler_Pool::SearchEntryByWeightPosition(double WeightPosition)
{
    int32 Left = 0;
    int32 Right = WeightMap.Num() - 1;

    while (Left < Right)
    {
        int32 Mid = Left + (Right - Left) / 2;
        if (WeightMap[Mid] < WeightPosition)
        {
            Left = Mid + 1;
        }
        else
        {
            Right = Mid;
        }
    }

    return Left;
}

void USpawnQuerySampler_Pool::OnPoolTableDataChanged()
{
    WeightMapDirty = true;
}
