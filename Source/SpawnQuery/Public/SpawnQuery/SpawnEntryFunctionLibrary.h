#pragma once
#include "SpawnQuery/Pools/SpawnEntryTableRow.h"
#include "SpawnEntryFunctionLibrary.generated.h"

class UBlackboardData;
class USpawnQueryContext;

UCLASS(MinimalAPI)
class USpawnEntryFunctionLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "SpawnQuery", meta=(WorldContext = "Outer"))
    static SPAWNQUERY_API USpawnQueryContext* ConstructSpawnQueryContext(const FName Name, UObject* Outer, UBlackboardData* BlackboardAsset);
    UFUNCTION(BlueprintPure, Category = "SpawnQuery")
    static SPAWNQUERY_API USpawnQueryContext* GetDefaultSpawnQueryContext();
    /**
     * Get SpawnEntryRow of any subtype from the USpawnEntryRowHandle.
     */
    UFUNCTION(BlueprintCallable, CustomThunk, Category = "SpawnQuery", meta=(CustomStructureParam = "OutRow", BlueprintInternalUseOnly="true"))
    static SPAWNQUERY_API bool GetSpawnEntryRow(USpawnEntryRowHandle* Entry, FSpawnEntryTableRowBase& OutRow);
    static SPAWNQUERY_API bool Generic_GetSpawnEntryRow(const USpawnEntryRowHandle* Entry, void* OutRowPtr);
    DECLARE_FUNCTION(execGetSpawnEntryRow);

private:

    static TWeakObjectPtr<USpawnQueryContext> CachedGlobalContext;
};
