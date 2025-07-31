#include "SpawnQuery/Samplers/SpawnQuerySampler_Pool.h"

#include "SpawnQueryTypes.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Misc/DefaultValueHelper.h"
#include "SpawnQuery/SpawnQueryContext.h"
#include "SpawnQuery/Pools/SpawnEntryTableRow.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(SpawnQuerySampler_Pool)

USpawnQuerySampler_Pool::USpawnQuerySampler_Pool(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}


void USpawnQuerySampler_Pool::PostLoad()
{
    Super::PostLoad();

    if (PoolTable)
    {
        EntryNum = PoolTable->GetRowNames().Num();
        PoolTable->OnDataTableChanged().AddUObject(this, &USpawnQuerySampler_Pool::OnPoolTableDataChanged);
    }
}

void USpawnQuerySampler_Pool::BeginDestroy()
{
    if (PoolTable)
    {
        PoolTable->OnDataTableChanged().RemoveAll(this);
    }

    Super::BeginDestroy();
}

#if WITH_EDITOR

void USpawnQuerySampler_Pool::PreEditChange(FProperty* PropertyAboutToChange)
{
    Super::PreEditChange(PropertyAboutToChange);

    if (PropertyAboutToChange && PropertyAboutToChange->GetName() == GET_MEMBER_NAME_CHECKED(USpawnQuerySampler_Pool, PoolTable))
    {
        if (PoolTable)
        {
            PoolTable->OnDataTableChanged().RemoveAll(this);
        }
        HasTableCacheBuilt = false;
    }
}

void USpawnQuerySampler_Pool::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
    Super::PostEditChangeProperty(PropertyChangedEvent);

    if (PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(USpawnQuerySampler_Pool, PoolTable))
    {
        if (PoolTable)
        {
            EntryNum = PoolTable->GetRowNames().Num();
        }
    }
}

#endif

FText USpawnQuerySampler_Pool::GetDescriptionDetails() const
{
    if (PoolTable)
    {
        return FText::FromString(FString::Format(TEXT("Pool Table: {0}\nTotal Entries: {1}"), { PoolTable->GetName(), EntryNum }));
    }
    else
    {
        return FText::FromString("Pool table not set");
    }
}

bool USpawnQuerySampler_Pool::IsActive(const USpawnQueryContext& Context)
{
    return PoolTable != nullptr;
}

TObjectPtr<USpawnEntryBase> USpawnQuerySampler_Pool::Query(USpawnQueryContext& Context)
{
    if (PoolTable == nullptr) return nullptr;
    if (!PoolTable->GetRowStruct()->IsChildOf(FSpawnEntryTableRowBase::StaticStruct()))
    {
        UE_LOG(LogSpawnQuery, Warning, TEXT("Mismatched row struct type in USpawnQuerySampler_Pool::Query"));
        return nullptr;
    }

#if WITH_EDITOR
    int32 CurrentWorldID = Context.GetWorldID();
    if (CachedWorldID != CurrentWorldID)
    {
        HasTableCacheBuilt = false;
        CachedWorldID = CurrentWorldID;
    }
#endif

    if (!HasTableCacheBuilt)
    {
        BuildTableCache();
    }

    if (UsingInfluencers)
    {
        WeightMapDirty = true;
    }

    if (WeightMapDirty)
    {
        BuildWeightCache(Context);
    }

    const double WeightPosition = Context.GetRandomStream().FRandRange(0, TotalWeights);

    int32 Index = SearchEntryByWeightPosition(WeightPosition);

    TArray<FName> Names = PoolTable->GetRowNames();
    FName Name = Names[Index];
    FSpawnEntryTableRowBase* Row = reinterpret_cast<FSpawnEntryTableRowBase*>(PoolTable->FindRowUnchecked(Name));

    USpawnEntryRowHandle* handle = NewObject<USpawnEntryRowHandle>(this); // maybe pool the struct because this function is frequently called
    handle->InitializeData(Row, Name, PoolTable);
    return handle;
}

#if WITH_EDITOR

void USpawnQuerySampler_Pool::Refresh()
{
    Super::Refresh();

    if (PoolTable)
    {
        EntryNum = PoolTable->GetRowNames().Num();
    }
}

#endif

void USpawnQuerySampler_Pool::BuildTableCache()
{
    if (HasTableCacheBuilt) return;

    EntryCache.Reset();
    auto RowMap = PoolTable->GetRowMap();

    for (auto Row : RowMap)
    {
        FSpawnEntryTableRowBase* RowValue = reinterpret_cast<FSpawnEntryTableRowBase*>(Row.Value);
        FSpawnerQueryPool_EntryCache Entry;
        TArray<FString> InfluencerStrings;

        RowValue->Influencers.ParseIntoArray(InfluencerStrings, TEXT(","), true);

        for (const FString& InfluencerString : InfluencerStrings)
        {
            FString InfluencerName;
            FString FactorString;
            if (float Factor = 0;
                InfluencerString.Split(TEXT(":"), &InfluencerName, &FactorString)
                && !InfluencerName.IsEmpty()
                && FDefaultValueHelper::ParseFloat(FactorString, Factor))
            {
                Entry.Influencers.Add(FSpawnerQueryPool_InfluencerEntry{ FName(*InfluencerName), Factor });
            }
            else
            {
                UE_LOG(LogBlueprint, Warning, TEXT("Invalid influencer format: %s"), *InfluencerString);
            }
        }

        if (Entry.Influencers.Num() > 0)
            UsingInfluencers = true;

        EntryCache.Add(Entry);
    }

    HasTableCacheBuilt = true;
    WeightMapDirty = true;
}

void USpawnQuerySampler_Pool::BuildWeightCache(const USpawnQueryContext& Context)
{
    TotalWeights = 0;
    EntryNum = 0;
    UsingInfluencers = false;
    WeightMap.Reset();

    auto RowMap = PoolTable->GetRowMap();
    const UBlackboardComponent& Blackboard = Context.GetBlackboardRef();

    for (auto Row : RowMap)
    {
        FSpawnEntryTableRowBase* RowValue = reinterpret_cast<FSpawnEntryTableRowBase*>(Row.Value);
        float Weight = RowValue->Weight;

        for (auto Influencer : EntryCache[EntryNum].Influencers)
        {
            Weight += Blackboard.GetValueAsFloat(Influencer.InfluencerName) * Influencer.Factor;
        }

        if (Weight > 0)
        {
            TotalWeights += Weight;
        }

        WeightMap.Add(TotalWeights);
        EntryNum++;
    }

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
