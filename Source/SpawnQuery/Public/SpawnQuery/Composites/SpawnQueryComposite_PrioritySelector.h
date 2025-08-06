#pragma once
#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "SpawnQuery/SpawnQueryNode_Composite.h"
#include "SpawnQueryComposite_PrioritySelector.generated.h"

UCLASS(MinimalAPI)
class USpawnQueryComposite_PrioritySelector : public USpawnQueryNode_Composite
{
    GENERATED_BODY()
    
public:
    virtual TObjectPtr<USpawnEntryBase> Query(USpawnQueryContext& Context) override;

public:

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ToolTip="If set, prioritize the subtrees on the right."))
    bool bReverseDirection = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ToolTip="If set, lower the priority of a subtree after it yields a result."))
    bool bDynamic = false;

private:
    void BuildPriorityStates(USpawnQueryContext& Context);
};

UCLASS(MinimalAPI)
class USpawnQueryComposite_PrioritySelector_State : public UObject
{
    GENERATED_BODY()

public:
    /**
     * Each as the index of a child, ordered by their priorities from high to low.
     */
    TArray<int16> ChildOrder;
};
