#include "EdGraphSchema_SpawnQuery.h"
#include "Modules/ModuleManager.h"
#include "EdGraph/EdGraph.h"
#include "SpawnQueryEditorModule.h"

#include "SpawnQueryGraphNode_Root.h"
#include "SpawnQueryGraphNode_Composite.h"
#include "SpawnQueryGraphNode_Sampler.h"

#include "SpawnQuery/SpawnQueryNode.h"
#include "SpawnQuery/SpawnQueryNode_Sampler.h"
#include "SpawnQuery/SpawnQueryNode_Composite.h"

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

void UEdGraphSchema_SpawnQuery::GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const
{
	USpawnQueryGraphNode* ParentGraphNode = ContextMenuBuilder.FromPin ? Cast<USpawnQueryGraphNode>(ContextMenuBuilder.FromPin->GetOuter()) : NULL;

	FSpawnQueryEditorModule& EditorModule = FModuleManager::GetModuleChecked<FSpawnQueryEditorModule>(TEXT("SpawnQueryEditor"));
	FGraphNodeClassHelper* ClassCache = EditorModule.GetClassCache().Get();

	// samplers

	TArray<FGraphNodeClassData> SamplerClasses;
	ClassCache->GatherClasses(USpawnQueryNode_Sampler::StaticClass(), SamplerClasses);

	FCategorizedGraphActionListBuilder SamplersBuilder(TEXT("Samplers"));
	for (auto& NodeClass : SamplerClasses)
	{
		const FText NodeTypeName = FText::FromString(FName::NameToDisplayString(NodeClass.ToString(), false));

		USpawnQueryGraphNode* OpNode = NewObject<USpawnQueryGraphNode_Sampler>(ContextMenuBuilder.OwnerOfTemporaries);
		OpNode->ClassData = NodeClass;

		TSharedPtr<FAISchemaAction_NewNode> AddOpAction = AddNewNodeAction(SamplersBuilder, NodeClass.GetCategory(), NodeTypeName, FText::GetEmpty());
		AddOpAction->NodeTemplate = OpNode;
	}

	ContextMenuBuilder.Append(SamplersBuilder);

	// composites
	
	TArray<FGraphNodeClassData> CompositeClasses;
	ClassCache->GatherClasses(USpawnQueryNode_Composite::StaticClass(), CompositeClasses);
	
	FCategorizedGraphActionListBuilder CompositesBuilder(TEXT("Composites"));
	for (auto& NodeClass : CompositeClasses)
	{
		const FText NodeTypeName = FText::FromString(FName::NameToDisplayString(NodeClass.ToString(), false));

		USpawnQueryGraphNode* OpNode = NewObject<USpawnQueryGraphNode_Composite>(ContextMenuBuilder.OwnerOfTemporaries);
		OpNode->ClassData = NodeClass;

		TSharedPtr<FAISchemaAction_NewNode> AddOpAction = AddNewNodeAction(CompositesBuilder, NodeClass.GetCategory(), NodeTypeName, FText::GetEmpty());
		AddOpAction->NodeTemplate = OpNode;
	}

	ContextMenuBuilder.Append(CompositesBuilder);
}

const FPinConnectionResponse UEdGraphSchema_SpawnQuery::CanCreateConnection(const UEdGraphPin* PinA, const UEdGraphPin* PinB) const
{
	// Make sure the pins are not on the same node
	if (PinA->GetOwningNode() == PinB->GetOwningNode())
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("Both are on the same node"));
	}

	if ((PinA->Direction == EGPD_Input && PinA->LinkedTo.Num()>0) || 
		(PinB->Direction == EGPD_Input && PinB->LinkedTo.Num()>0))
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT(""));
	}

	// Compare the directions
	bool bDirectionsOK = false;

	if ((PinA->Direction == EGPD_Input) && (PinB->Direction == EGPD_Output))
	{
		bDirectionsOK = true;
	}
	else if ((PinB->Direction == EGPD_Input) && (PinA->Direction == EGPD_Output))
	{
		bDirectionsOK = true;
	}

	if (bDirectionsOK)
	{
		if ( (PinA->Direction == EGPD_Input && PinA->LinkedTo.Num()>0) || (PinB->Direction == EGPD_Input && PinB->LinkedTo.Num()>0))
		{
			return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("Already connected with other"));
		}
	}
	else
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT(""));
	}

	return FPinConnectionResponse(CONNECT_RESPONSE_MAKE, TEXT(""));
}
