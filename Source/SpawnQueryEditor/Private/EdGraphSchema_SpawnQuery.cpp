#include "EdGraphSchema_SpawnQuery.h"
#include "Modules/ModuleManager.h"
#include "EdGraph/EdGraph.h"
#include "SpawnQueryGraphNode_Root.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(EdGraphSchema_SpawnQuery)

#define LOCTEXT_NAMESPACE "SpawnQueryEditor"

UEdGraphSchema_SpawnQuery::UEdGraphSchema_SpawnQuery(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UEdGraphSchema_SpawnQuery::CreateDefaultNodesForGraph(UEdGraph& Graph) const
{
	FGraphNodeCreator<USpawnQueryGraphNode_Root> NodeCreator(Graph);
	USpawnQueryGraphNode_Root* MyNode = NodeCreator.CreateNode();
	NodeCreator.Finalize();
	SetNodeMetaData(MyNode, FNodeMetadata::DefaultGraphNode);
}
