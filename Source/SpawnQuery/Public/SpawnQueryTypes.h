#pragma once

#include "CoreMinimal.h"
#include "Stats/Stats.h"
#include "UObject/ObjectMacros.h"
#include "UObject/Object.h"
#include "Templates/SubclassOf.h"
#include "Engine/EngineTypes.h"

#include "SpawnQueryTypes.generated.h"

UCLASS(Abstract, MinimalAPI)
class USpawnQueryTypes : public UObject
{
	GENERATED_BODY()

public:

	static SPAWNQUERY_API FText GetShortTypeName(const UObject* Ob);
};
