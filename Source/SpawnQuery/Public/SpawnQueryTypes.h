#pragma once

#include "CoreMinimal.h"
#include "Stats/Stats.h"
#include "UObject/ObjectMacros.h"
#include "UObject/Object.h"
#include "Templates/SubclassOf.h"
#include "Engine/EngineTypes.h"
#include "SpawnQueryTypes.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogSpawnQuery, Log, All);

namespace SpawnQueryTypes
{
    FText GetShortTypeName(const UObject* Ob);

}

UENUM(BlueprintType)
enum ERandomizationPolicy : uint8
{
    Independent,
    ShuffledSequence
};
