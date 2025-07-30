#pragma once

#include "CoreMinimal.h"
#include "Stats/Stats.h"
#include "UObject/ObjectMacros.h"
#include "UObject/Object.h"
#include "Templates/SubclassOf.h"
#include "Engine/EngineTypes.h"

DECLARE_LOG_CATEGORY_EXTERN(LogSpawnQuery, Log, All);

namespace SpawnQueryTypes
{
    FText GetShortTypeName(const UObject* Ob);
}
