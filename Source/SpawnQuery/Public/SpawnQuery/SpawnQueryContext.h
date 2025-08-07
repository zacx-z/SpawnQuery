#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardAssetProvider.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "UObject/ObjectMacros.h"
#include "UObject/Object.h"
#include "SpawnQueryContext.generated.h"

class USpawnQuery;

UCLASS(BlueprintType, MinimalAPI, Transient)
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

    bool IsSpawnQueryActive(const USpawnQuery* SpawnQuery, bool bDefault) const;
    void SetSpawnQueryActiveState(const USpawnQuery* SpawnQuery, bool bActiveState);

    /**
     * Reset all the internal states, with the Blackboard set to its initial state.
     */
    UFUNCTION(BlueprintCallable, Category="SpawnQuery")
    SPAWNQUERY_API void Reset();

    /**
     * Reset all the internal states and reinitialize with `Seed`. Reset the Blackboard to its initial state.
     */
    UFUNCTION(BlueprintCallable, Category="SpawnQuery")
    SPAWNQUERY_API void ResetSeed(int32 Seed);

    /**
     * Return the Blackboard component.
     */
    UFUNCTION(BlueprintPure, Category="SpawnQuery")
    UBlackboardComponent* GetBlackboard()
    {
        if (BlackboardPtr == nullptr)
        {
            CreateActor();
        }
        return BlackboardPtr;
    }

    /**
     * Set the Blackboard asset and reinitialize the Blackboard component.
     */
    UFUNCTION(BlueprintCallable, Category="SpawnQuery")
    void SetBlackboardAsset(UBlackboardData* InBlackboardAsset);

    const UBlackboardComponent& GetBlackboardRef() const
    {
        if (BlackboardPtr == nullptr)
        {
            CreateActor();
        }
        return *BlackboardPtr;
    }

    template<typename T>
    T* GetStateObject(UObject* Owner)
    {
        static_assert(TIsDerivedFrom<T, UObject>::Value, "GetStorage can only be used with UObject-derived types.");

        return Cast<T>(GetStateObjectInternal(Owner, T::StaticClass()));
    }

public:
    void PushCall(USpawnQuery* Query);
    void PopCall(USpawnQuery* Query);
    bool HasQueryInCallStack(USpawnQuery* Query) const;
    FString GetCallStackInfo();
    int32 GetWorldID() const;

protected:
    UObject* GetStateObjectInternal(UObject* Owner, UClass* StateObjectClass);
    void ResetStates();

private:
    FRandomStream RandomStream;

    UPROPERTY()
    TMap<const USpawnQuery*, bool> QueryActiveStateMap;

    // store the currently invoked SpawnQuery graphs during a query to avoid recursion
    UPROPERTY()
    TArray<TWeakObjectPtr<USpawnQuery>> QueryCallStack;

    UPROPERTY()
    TMap<TObjectPtr<UObject>, TObjectPtr<UObject>> StateObjectMap;

    UPROPERTY()
    mutable TObjectPtr<UBlackboardComponent> BlackboardPtr;

    UPROPERTY()
    TWeakObjectPtr<UBlackboardData> BlackboardAsset;

    void CreateActor() const;
    UWorld* GetRelevantWorld() const;
};
