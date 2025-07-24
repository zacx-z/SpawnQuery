#pragma once

#include "AIGraphNode.h"
#include "SpawnQueryGraphNode.generated.h"

class UEdGraphSchema;

UCLASS()
class USpawnQueryGraphNode : public UAIGraphNode
{
	GENERATED_UCLASS_BODY()

	virtual class USpawnQueryGraph* GetSpawnQueryGraph();

	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FText GetDescription() const override;

	virtual bool CanCreateUnderSpecifiedSchema(const UEdGraphSchema* DesiredSchema) const override;
};
