#include "SpawnQueryGraph.h"
#include "EdGraphSchema_SpawnQuery.h"
#include "SpawnQueryEditorTypes.h"

#include "SpawnQuery.h"
#include "SpawnQuery/SpawnQueryNode.h"
#include "SpawnQueryGraphNode_Root.h"
#include "SpawnQuery/SpawnQueryNode_Composite.h"
#include "SpawnQuery/SpawnQueryNode_Sampler.h"

USpawnQueryGraph::USpawnQueryGraph(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	Schema = UEdGraphSchema_SpawnQuery::StaticClass();
}

void USpawnQueryGraph::Initialize()
{
	Super::Initialize();

	LockUpdates();
	SpawnMissingNodes();
	UnlockUpdates();
}

void USpawnQueryGraph::OnLoaded()
{
	Super::OnLoaded();
}

void USpawnQueryGraph::UpdateAsset(int32 UpdateFlags)
{
	if (bLockUpdates)
	{
		return;
	}

	USpawnQueryGraphNode_Root* RootNode = nullptr;

	for (auto UntypedNode : Nodes)
	{
		USpawnQueryGraphNode* Node = Cast<USpawnQueryGraphNode>(UntypedNode);
		if (Node == nullptr)
		{
			continue;
		}

		// cache root
		if (RootNode == nullptr)
		{
			RootNode = Cast<USpawnQueryGraphNode_Root>(Node);
		}
	}

	USpawnQueryGraphNode* Node = nullptr;
	
	if (RootNode && RootNode->Pins.Num() > 0 && RootNode->Pins[0]->LinkedTo.Num() > 0)
	{
		UEdGraphPin* Pin = RootNode->Pins[0];
		Node = Cast<USpawnQueryGraphNode>(Pin->LinkedTo[0]->GetOwningNode()); // the node under root node
	}

	BuildQueryTree(Node);
}

namespace SpawnQueryGraphHelpers
{
	void CreateSubtree(USpawnQuery* Asset, USpawnQueryNode* RootNode, const USpawnQueryGraphNode* RootEdNode, uint8 TreeDepth)
	{
		if (RootEdNode == nullptr || RootNode == nullptr)
		{
			return;
		}

		if (Cast<USpawnQuery>(RootNode->GetOuter()) == nullptr)
		{
			RootNode->Rename(nullptr, Asset);
		}

		if (USpawnQueryNode_Composite* CompositeNode = Cast<USpawnQueryNode_Composite>(RootNode))
		{
			CompositeNode->Children.Reset();

			for (UEdGraphPin* Pin : RootEdNode->Pins)
			{
				if (Pin->Direction != EGPD_Output)
				{
					continue;
				}

				Pin->LinkedTo.Sort(FCompareNodeXLocation());

				for (UEdGraphPin* ChildPin : Pin->LinkedTo)
				{
					USpawnQueryGraphNode* Child = Cast<USpawnQueryGraphNode>(ChildPin->GetOwningNode());
					if (Child == nullptr)
					{
						continue;
					}

					USpawnQueryNode* NodeInstance = Cast<USpawnQueryNode>(Child->NodeInstance);

					if (NodeInstance == nullptr)
					{
						continue;
					}

					FSpawnQueryCompositeChild& ChildInfo = CompositeNode->Children.AddDefaulted_GetRef();
					ChildInfo.ChildComposite = Cast<USpawnQueryNode_Composite>(NodeInstance);
					ChildInfo.ChildSampler = Cast<USpawnQueryNode_Sampler>(NodeInstance);

					CreateSubtree(Asset, NodeInstance, Child, TreeDepth + 1);
				}
			}
		}
	}
} // namespace SpawnQueryGraphHelpers

void USpawnQueryGraph::SpawnMissingNodes()
{
	// This function is intended to spawn any missing nodes in the graph.
	// The actual implementation would depend on the specific requirements of the SpawnQuery system.
	// For now, we can leave it empty or implement a basic logic to ensure all necessary nodes are present.
}

void USpawnQueryGraph::BuildQueryTree(USpawnQueryGraphNode* RootEdNode)
{
	USpawnQuery* Asset = Cast<USpawnQuery>(GetOuter());

	Asset->RootNode = nullptr;

	if (RootEdNode)
	{
		Asset->RootNode = Cast<USpawnQueryNode>(RootEdNode->NodeInstance);
		SpawnQueryGraphHelpers::CreateSubtree(Asset, Asset->RootNode, RootEdNode, 1);
	}
}
