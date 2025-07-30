#pragma once

#include "SpawnQueryGraphNode.h"
#include "CoreMinimal.h"
#include "EdGraph/EdGraphNode.h"
#include "Internationalization/Text.h"
#include "UObject/ObjectMacros.h"
#include "UObject/UObjectGlobals.h"

#include "SpawnQueryGraphNode_Composite.generated.h"

class UObject;

UCLASS(MinimalAPI)
class USpawnQueryGraphNode_Composite : public USpawnQueryGraphNode
{
    GENERATED_UCLASS_BODY()

    SPAWNQUERYEDITOR_API virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
    SPAWNQUERYEDITOR_API virtual FText GetDescription() const override;
    SPAWNQUERYEDITOR_API virtual FText GetTooltipText() const override;
    SPAWNQUERYEDITOR_API virtual bool RefreshNodeClass() override { return false; }

    /** Gets a list of actions that can be done to this particular node */
    SPAWNQUERYEDITOR_API virtual void GetNodeContextMenuActions(class UToolMenu* Menu, class UGraphNodeContextMenuContext* Context) const override;
};
