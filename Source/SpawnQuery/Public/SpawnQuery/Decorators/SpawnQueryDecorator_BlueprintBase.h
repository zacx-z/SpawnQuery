#pragma once
#include "SpawnQuery/SpawnQueryNode_Decorator.h"
#include "SpawnQueryDecorator_BlueprintBase.generated.h"

UCLASS(Abstract, Blueprintable, MinimalAPI)
class USpawnQueryDecorator_BlueprintBase : public USpawnQueryNode_Decorator
{
    GENERATED_UCLASS_BODY()

public:
    virtual bool IsActive(const USpawnQueryContext& Context) override;
    virtual TObjectPtr<USpawnEntryBase> Rewrite(TObjectPtr<USpawnEntryBase> Result, USpawnQueryContext& Context) override;
    virtual float MutateWeight(float InWeight, const USpawnQueryContext& Context) override;

    /**
     * If false, prevent querying into this branch.
     */
    UFUNCTION(BlueprintImplementableEvent)
    SPAWNQUERY_API bool ReceiveCheckIsActive(const USpawnQueryContext* Context);
    /**
     * Change the result after it is generated.
     */
    UFUNCTION(BlueprintImplementableEvent)
    SPAWNQUERY_API USpawnEntryBase* ReceiveRewrite(USpawnEntryBase* Result, USpawnQueryContext* Context);
    /**
     * Change the weight of this branch.
     */
    UFUNCTION(BlueprintImplementableEvent)
    SPAWNQUERY_API float ReceiveMutateWeight(float InWeight, const USpawnQueryContext* Context);

private:

    bool HasReceiveCheckIsActiveImplementation : 1;
    bool HasReceiveRewriteImplementation : 1;
    bool HasReceiveMutateWeightImplementation : 1;
};
