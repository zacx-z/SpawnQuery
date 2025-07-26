#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "SpawnQuery/SpawnEntry.h"
#include "SpawnEntryTableRow.generated.h"

USTRUCT(BlueprintType)
struct FSpawnEntryTableRow : public FTableRowBase
{
    GENERATED_USTRUCT_BODY()

public:

    FSpawnEntryTableRow()
    : MinAmount(0)
    , MaxAmount(1)
    {
    }

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 MinAmount;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 MaxAmount;
};

class FSpawnEntryRowHandle : public ISpawnEntryBase
{
public:
    FSpawnEntryRowHandle(FSpawnEntryTableRow* Row, TObjectPtr<UDataTable> PoolTable)
        : Row(Row), PoolTable(PoolTable)
    {}

private:
    FSpawnEntryTableRow* Row;
    TObjectPtr<UDataTable> PoolTable;

public:
    template<typename T>
    T* GetTableRow()
    {
        if (!PoolTable->GetRowStruct()->IsA(T::StaticStruct()))
        {
            return nullptr;
        }
        return static_cast<T*>(Row);
    }
};
