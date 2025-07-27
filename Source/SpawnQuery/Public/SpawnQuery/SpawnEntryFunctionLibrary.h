#pragma once
#include "SpawnQuery/Pools/SpawnEntryTableRow.h"
#include "SpawnEntryFunctionLibrary.generated.h"

UCLASS(MinimalAPI)
class USpawnEntryFunctionLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    // Get SpawnEntryRow of any subtype from the Entry UClass.
    UFUNCTION(BlueprintCallable, CustomThunk, Category = "SpawnQuery", meta=(CustomStructureParam = "OutRow", BlueprintInternalUseOnly="true"))
    static SPAWNQUERY_API bool GetSpawnEntryRow(USpawnEntryRowHandle* Entry, FSpawnEntryTableRow& OutRow);
    static SPAWNQUERY_API bool Generic_GetSpawnEntryRow(const USpawnEntryRowHandle* Entry, void* OutRowPtr);
    DECLARE_FUNCTION(execGetSpawnEntryRow);
};
