#include "SpawnQueryGraphNode.h"
#include "EdGraphSchema_SpawnQuery.h"
#include "SpawnQueryGraph.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(SpawnQueryGraphNode)

USpawnQueryGraphNode::USpawnQueryGraphNode(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void USpawnQueryGraphNode::AllocateDefaultPins()
{
    CreatePin(EGPD_Input, "MultipleNodes", TEXT("In"));
    CreatePin(EGPD_Output, "MultipleNodes", TEXT("Out"));
}

FText USpawnQueryGraphNode::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
    return FText::GetEmpty();
}

FText USpawnQueryGraphNode::GetDescription() const
{
    return FText::GetEmpty();
}

USpawnQueryGraph* USpawnQueryGraphNode::GetSpawnQueryGraph()
{
    return CastChecked<USpawnQueryGraph>(GetGraph());
}

bool USpawnQueryGraphNode::CanCreateUnderSpecifiedSchema(const UEdGraphSchema* DesiredSchema) const
{
    return DesiredSchema->GetClass()->IsChildOf(UEdGraphSchema_SpawnQuery::StaticClass());
}
