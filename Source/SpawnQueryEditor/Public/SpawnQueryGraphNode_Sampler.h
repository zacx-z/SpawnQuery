#pragma once

#include "SpawnQueryGraphNode.h"
#include "CoreMinimal.h"
#include "EdGraph/EdGraphNode.h"
#include "Internationalization/Text.h"
#include "UObject/ObjectMacros.h"
#include "UObject/UObjectGlobals.h"

#include "SpawnQueryGraphNode_Sampler.generated.h"

class UObject;

UCLASS()
class SPAWNQUERYEDITOR_API USpawnQueryGraphNode_Sampler : public USpawnQueryGraphNode
{
	GENERATED_UCLASS_BODY()
	virtual void AllocateDefaultPins() override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FText GetDescription() const override;
	virtual FText GetTooltipText() const override;
	virtual bool RefreshNodeClass() override { return false; }

	/** Gets a list of actions that can be done to this particular node */
	virtual void GetNodeContextMenuActions(class UToolMenu* Menu, class UGraphNodeContextMenuContext* Context) const override;
};
