#pragma once

#include "AIGraphSchema.h"
#include "EdGraphSchema_SpawnQuery.generated.h"

struct FGraphContextMenuBuilder;
struct FPinConnectionResponse;

class UEdGraph;

UCLASS(MinimalAPI)
class UEdGraphSchema_SpawnQuery : public UAIGraphSchema
{
	GENERATED_UCLASS_BODY()

	//~ Begin EdGraphSchema Interface
	virtual void CreateDefaultNodesForGraph(UEdGraph& Graph) const override;
	//~ End EdGraphSchema Interface
};
