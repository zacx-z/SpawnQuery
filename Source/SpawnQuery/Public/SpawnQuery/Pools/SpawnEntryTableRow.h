#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "SpawnQuery/SpawnEntry.h"
#include "SpawnEntryTableRow.generated.h"

USTRUCT(BlueprintType)
struct FSpawnEntryTableRowBase : public FTableRowBase
{
    GENERATED_USTRUCT_BODY()

public:

    FSpawnEntryTableRowBase()
        : Weight(1)
    {
    }

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Randomization Settings")
    float Weight;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Randomization Settings")
    FString Influencers;
};

UCLASS(BlueprintType, NotBlueprintable)
class USpawnEntryRowHandle : public USpawnEntryBase
{
    GENERATED_BODY()

public:
    void InitializeData(FSpawnEntryTableRowBase* InRow, FName InRowName, TObjectPtr<UDataTable> InPoolTable)
    {
        Row = InRow;
        RowName = InRowName;
        PoolTable = InPoolTable;
    }

private:
    FSpawnEntryTableRowBase* Row;
    FName RowName;

    UPROPERTY()
    TObjectPtr<UDataTable> PoolTable;

public:
    template<typename T>
    T* GetTableRow()
    {
        if (!PoolTable->GetRowStruct()->IsChildOf(T::StaticStruct()))
        {
            return nullptr;
        }
        return static_cast<T*>(Row);
    }

    UScriptStruct* GetRowStruct() const
    {
        return PoolTable->RowStruct;
    }

    UFUNCTION(BlueprintCallable)
    const FName& GetRowName() const
    {
        return RowName;
    }

    FSpawnEntryTableRowBase* GetTableRow() const
    {
        return Row;
    }
};
