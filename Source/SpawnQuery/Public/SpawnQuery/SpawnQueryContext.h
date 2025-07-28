#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "UObject/Object.h"
#include "SpawnQueryContext.generated.h"

class USpawnQuery;

UCLASS(MinimalAPI)
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

    bool IsSpawnQueryActive(const USpawnQuery* SpawnQuery, bool bDefault) const;
    void SetSpawnQueryActiveState(const USpawnQuery* SpawnQuery, bool bActiveState);

    void PushCall(USpawnQuery* Query);
    void PopCall(USpawnQuery* Query);
    bool HasQueryInCallStack(USpawnQuery* Query) const;
    FString GetCallStackInfo();

private:
    FRandomStream RandomStream;

    TMap<const USpawnQuery*, bool> QueryActiveStateMap;
    // store the currently invoked SpawnQuery graphs during a query to avoid recursion
    TArray<TWeakObjectPtr<USpawnQuery>> QueryCallStack;
};
