#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "UObject/Object.h"
//#include "SpawnQuery/SpawnQueryTypes.h"
#include "SpawnQueryNode.generated.h"

class USpawnQueryNode_Decorator;
class USpawnEntryBase;
class USpawnQueryContext;
struct FPropertyChangedEvent;

UCLASS(Abstract, MinimalAPI)
class USpawnQueryNode : public UObject
{
    GENERATED_UCLASS_BODY()

    SPAWNQUERY_API virtual FText GetDescriptionTitle() const;
    SPAWNQUERY_API virtual FText GetDescriptionDetails() const;

    /**
     * To be extended by any Node who offloads its work to another thread.
     * Returns false by default, unless overridden.
     * If overridden, will return whether or not this Node is currently being processed asynchronously.
     */
    virtual FORCEINLINE bool IsCurrentlyRunningAsync() const { return false; }

public:
    /**
     * Implement this function to determine the node's active state
     */
    virtual bool IsActive(const USpawnQueryContext& Context) PURE_VIRTUAL(USpawnQueryNode::IsActive, return false;)
    /**
     * Implement this function for the behavior upon queries
     */
    virtual TObjectPtr<USpawnEntryBase> Query(USpawnQueryContext& Context) PURE_VIRTUAL(USpawnQueryNode::Query, return nullptr;)
    /**
     * The weight for being randomized by of parent nodes
     */
    virtual float GetWeight(const USpawnQueryContext& Context);


    /**
     * Whether to be considered as a valid path to generate a result entry
     */
    bool IsSubtreeActive(const USpawnQueryContext& Context);

    UPROPERTY()
    TArray<TObjectPtr<USpawnQueryNode_Decorator>> Decorators;

#if WITH_EDITOR

    /**
     * Only for the Editor to refresh the information on it
     */
    virtual void Refresh() {}
#endif

private:
    bool CheckDecoratorsActive(const USpawnQueryContext& Context);
};
