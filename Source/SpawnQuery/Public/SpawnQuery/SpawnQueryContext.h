#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "UObject/ObjectMacros.h"
#include "UObject/Object.h"
#include "SpawnQueryContext.generated.h"

class USpawnQuery;

UCLASS(BlueprintType)
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

    UFUNCTION(BlueprintPure, Category = "SpawnQuery")
    UBlackboardComponent* GetBlackboard()
    {
        if (BlackboardPtr == nullptr)
        {
            CreateActor();
        }
        return BlackboardPtr;
    }

    const UBlackboardComponent& GetBlackboardRef() const
    {
        if (BlackboardPtr == nullptr)
        {
            CreateActor();
        }
        return *BlackboardPtr;
    }

    void PushCall(USpawnQuery* Query);
    void PopCall(USpawnQuery* Query);
    bool HasQueryInCallStack(USpawnQuery* Query) const;
    FString GetCallStackInfo();

private:
    FRandomStream RandomStream;

    UPROPERTY()
    TMap<const USpawnQuery*, bool> QueryActiveStateMap;

    // store the currently invoked SpawnQuery graphs during a query to avoid recursion
    UPROPERTY()
    TArray<TWeakObjectPtr<USpawnQuery>> QueryCallStack;

    UPROPERTY()
    mutable TObjectPtr<UBlackboardComponent> BlackboardPtr;

    void CreateActor() const;
    UWorld* GetRelevantWorld() const;
};
