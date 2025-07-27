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

public:
    TObjectPtr<USpawnEntryBase> QueryEntry(USpawnQueryContext& context) const;

    // Blueprint version that allocates an UObject
    UFUNCTION(BlueprintCallable)
    USpawnEntryBase* QueryEntry(USpawnQueryContext* context) const;
};
