#pragma once
#include "SpawnQuery/SpawnQueryNode.h"
#include "SpawnQueryNode_Decorator.generated.h"

UCLASS(Abstract, MinimalAPI)
class USpawnQueryNode_Decorator : public USpawnQueryNode
{
    GENERATED_BODY()

public:

    virtual bool IsActive(const USpawnQueryContext& Context) override { return true; }
    virtual TObjectPtr<USpawnEntryBase> Query(USpawnQueryContext& Context) override { return nullptr; }

    virtual double MutateWeight(double InWeight) { return InWeight; }
};
