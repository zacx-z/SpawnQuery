#pragma once

#include "CoreMinimal.h"
#include "RandomizationTypes.generated.h"

USTRUCT()
struct FRandomizationWeightState
{
    GENERATED_BODY()

    float Base;

    // cache
    float CachedWeight;
    bool CachedActive;
};

