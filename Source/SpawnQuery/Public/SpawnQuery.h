#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SpawnQuery/SpawnEntry.h"
#include "SpawnQuery.generated.h"

class USpawnQueryNode;
class USpawnQueryContext;

UCLASS(BlueprintType, MinimalAPI)
class USpawnQuery : public UDataAsset
{
    GENERATED_UCLASS_BODY()

#if WITH_EDITORONLY_DATA
    /** Graph for queries */
    UPROPERTY()
    TObjectPtr<UEdGraph> EdGraph;
#endif

    UPROPERTY()
    TObjectPtr<USpawnQueryNode> RootNode;

    /**
     * Defaults to true. The active state of this graph in a context by default.
     *
     * If a graph is inactive, it won't return any entry.
     * Invocation node to this graph will also be regarded inactive.
     * By toggling a graph's active state at runtime, customized behaviors at graph level are made possible.
     */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bActiveByDefault;

public:
    SPAWNQUERY_API bool IsActive(const USpawnQueryContext& Context) const;
    SPAWNQUERY_API void SetActiveState(bool bActiveState, USpawnQueryContext& Context) const;
    SPAWNQUERY_API TObjectPtr<USpawnEntryBase> QueryEntry(USpawnQueryContext& Context) const;

    // Blueprint version
    UFUNCTION(BlueprintPure)
    bool IsActive(const USpawnQueryContext* Context = nullptr);
    UFUNCTION(BlueprintCallable)
    void SetActiveState(bool bActiveState, USpawnQueryContext* Context = nullptr);
    UFUNCTION(BlueprintPure=false)
    USpawnEntryBase* QueryEntry(USpawnQueryContext* Context = nullptr);

private:
    USpawnQueryContext* GetGlobalContext();
    TWeakObjectPtr<USpawnQueryContext> CachedGlobalContext;
};
