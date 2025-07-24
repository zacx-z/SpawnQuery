#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SpawnQuery.generated.h"

UCLASS(BlueprintType, MinimalAPI)
class USpawnQuery : public UDataAsset
{
	GENERATED_UCLASS_BODY()

public:
	UPROPERTY(EditAnywhere)
	int TestData;
};
