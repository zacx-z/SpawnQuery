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
     * Defaults to true. The active state of this graph when a context is just created.
     *
     * If a graph is inactive, it won't return any entry.
     * Invocation node to this graph will also be regarded inactive.
     * By toggling a graph's active state at runtime, customized behaviors at graph level are made possible.
     */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bActiveByDefault;

public:
    bool IsActive(const USpawnQueryContext& Context) const;
    TObjectPtr<USpawnEntryBase> QueryEntry(USpawnQueryContext& context) const;

    // Blueprint version that allocates an UObject
    UFUNCTION(BlueprintPure=false)
    USpawnEntryBase* QueryEntry(USpawnQueryContext* context = nullptr) const;
};
