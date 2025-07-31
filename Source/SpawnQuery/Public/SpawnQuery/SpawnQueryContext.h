#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardAssetProvider.h"
#include "BehaviorTree/BlackboardComponent.h"
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

    UFUNCTION(BlueprintPure, Category="SpawnQuery")
    UBlackboardComponent* GetBlackboard()
    {
        if (BlackboardPtr == nullptr)
        {
            CreateActor();
        }
        return BlackboardPtr;
    }

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

    UFUNCTION(BlueprintCallable, Category="SpawnQuery")
    void SetSeed(int32 Seed);

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
