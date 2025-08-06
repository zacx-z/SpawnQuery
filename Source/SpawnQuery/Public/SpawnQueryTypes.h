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
    /**
     * Traditional randomization method that aims for independent probabilities.
     */
    Independent,
    /**
     * Every choice shows up a certain times (dictated by weight) in a random order before repeating.
     */
    ShuffledSequence
};
