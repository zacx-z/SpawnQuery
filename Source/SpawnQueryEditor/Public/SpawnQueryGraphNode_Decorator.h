#pragma once
#include "SpawnQueryGraphNode.h"
#include "SpawnQueryGraphNode_Decorator.generated.h"

UCLASS(MinimalAPI)
class USpawnQueryGraphNode_Decorator : public USpawnQueryGraphNode
{
    GENERATED_UCLASS_BODY()

public:
    SPAWNQUERYEDITOR_API virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
    SPAWNQUERYEDITOR_API virtual FText GetDescription() const override;
    SPAWNQUERYEDITOR_API virtual void AllocateDefaultPins() override;

    SPAWNQUERYEDITOR_API virtual FLinearColor GetBackgroundColor() const override;
};
