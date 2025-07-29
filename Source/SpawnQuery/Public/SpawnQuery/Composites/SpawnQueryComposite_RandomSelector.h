#pragma once
#include "SpawnQuery/SpawnQueryNode_Composite.h"
#include "SpawnQueryComposite_RandomSelector.generated.h"

UCLASS(MinimalAPI)
class USpawnQueryComposite_RandomSelector : public USpawnQueryNode_Composite
{
    GENERATED_BODY()

public:
    virtual TObjectPtr<USpawnEntryBase> Query(USpawnQueryContext& Context) override;
};
