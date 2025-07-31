#pragma once

#include "CoreMinimal.h"
#include "SpawnQuery/SpawnQueryNode.h"
#include "SpawnQueryNode_Composite.generated.h"

class USpawnQueryNode_Sampler;

USTRUCT()
struct FSpawnQueryCompositeChild
{
    GENERATED_USTRUCT_BODY()

    /** child node */
    UPROPERTY()
    TObjectPtr<USpawnQueryNode> ChildNode = nullptr;

public:

    bool IsActive(USpawnQueryContext& Context)
    {
        return ChildNode->IsSubtreeActive(Context);
    }

    float GetWeight(const USpawnQueryContext& Context)
    {
        return ChildNode->GetWeight(Context);
    }
};

UCLASS(Abstract, MinimalAPI)
class USpawnQueryNode_Composite : public USpawnQueryNode
{
    GENERATED_UCLASS_BODY()

    virtual bool IsActive(const USpawnQueryContext& Context) override;

    /** child nodes */
    UPROPERTY()
    TArray<FSpawnQueryCompositeChild> Children;
};
