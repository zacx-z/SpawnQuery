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
};

UCLASS(Abstract, MinimalAPI)
class USpawnQueryNode_Composite : public USpawnQueryNode
{
    GENERATED_UCLASS_BODY()

    /** child nodes */
    UPROPERTY()
    TArray<FSpawnQueryCompositeChild> Children;
};
