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

    virtual TObjectPtr<USpawnEntryBase> Query(USpawnQueryContext& context) override;

public:

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bReverseDirection = false;
};
