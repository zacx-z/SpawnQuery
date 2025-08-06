#pragma once
#include "SpawnQuery/SpawnQueryNode.h"
#include "SpawnQueryNode_Decorator.generated.h"

UCLASS(Abstract, MinimalAPI)
class USpawnQueryNode_Decorator : public USpawnQueryNode
{
    GENERATED_BODY()

public:

    /**
     * Unused
     */
    virtual TObjectPtr<USpawnEntryBase> Query(USpawnQueryContext& Context) override { return nullptr; }
    /**
     * If false, prevent querying into this branch.
     */
    virtual bool IsActive(const USpawnQueryContext& Context) override { return true; }
    /**
     * Change the result after it is generated.
     */
    virtual TObjectPtr<USpawnEntryBase> Rewrite(TObjectPtr<USpawnEntryBase> Result, USpawnQueryContext& Context) { return Result; }
    /**
     * Change the weight of this branch.
     */
    virtual float MutateWeight(float InWeight, const USpawnQueryContext& Context) { return InWeight; }
};
