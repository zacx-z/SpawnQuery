#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SpawnQuery/ISpawnEntry.h"
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
	
	ISpawnEntry* QueryEntry(USpawnQueryContext& context);
};
