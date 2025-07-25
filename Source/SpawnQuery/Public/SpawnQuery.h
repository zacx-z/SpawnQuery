#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SpawnQuery.generated.h"

UCLASS(BlueprintType, MinimalAPI)
class USpawnQuery : public UDataAsset
{
	GENERATED_UCLASS_BODY()

#if WITH_EDITORONLY_DATA
	/** Graph for query */
	UPROPERTY()
	TObjectPtr<UEdGraph>	EdGraph;
#endif

public:
	UPROPERTY(EditAnywhere)
	int TestData;
};
