#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "UObject/Object.h"
#include "SpawnQueryContext.generated.h"

UCLASS(Abstract, EditInlineNew, MinimalAPI)
class USpawnQueryContext : public UObject
{
	GENERATED_BODY()
	
public:
	FRandomStream& GetRandomStream()
	{
		return RandomStream;
	}

private:
	FRandomStream RandomStream;
};
