#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "UObject/Object.h"
#include "SpawnQueryContext.generated.h"

UCLASS(EditInlineNew, Blueprintable)
class USpawnQueryContext : public UObject
{
    GENERATED_BODY()

    virtual void PostLoad() override;
    virtual void PostInitProperties() override;

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 RandomSeed;

    FRandomStream& GetRandomStream()
    {
        return RandomStream;
    }

    SPAWNQUERY_API void Reset();

private:
    FRandomStream RandomStream;
};
