#include "SpawnQueryGraphNode.h"
#include "EdGraphSchema_SpawnQuery.h"
#include "SGraphEditorActionMenuAI.h"
#include "SpawnQueryColors.h"
#include "SpawnQueryEditorTypes.h"
#include "SpawnQueryGraph.h"
#include "SpawnQueryGraphNode_Decorator.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(SpawnQueryGraphNode)

#define LOCTEXT_NAMESPACE "SpawnQueryEditor"

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

void USpawnQueryGraphNode::GetNodeContextMenuActions(UToolMenu* Menu, UGraphNodeContextMenuContext* Context) const
{
    if (!bSupportDecorators) return;

    // add decorator
    FToolMenuSection& Section = Menu->FindOrAddSection("SpawnQueryGraphNode");
    Section.AddSubMenu(
        "AddDecorator",
        LOCTEXT("AddDecorator", "Add Decorator..." ),
        LOCTEXT("AddDecoratorTooltip", "Adds new decorator as a subnode" ),
        FNewToolMenuDelegate::CreateUObject(this, &USpawnQueryGraphNode::CreateAddDecoratorSubMenu, (UEdGraph*)Context->Graph));
}

void USpawnQueryGraphNode::OnSubNodeAdded(UAIGraphNode* SubNode)
{
    UE_LOG(LogTemp, Warning, TEXT("Add Subnode"));
    if (USpawnQueryGraphNode_Decorator* DecoratorNode = Cast<USpawnQueryGraphNode_Decorator>(SubNode))
    {
        Decorators.Add(DecoratorNode);
    }
}

void USpawnQueryGraphNode::OnSubNodeRemoved(UAIGraphNode* SubNode)
{
    const int32 DecoratorIdx = Decorators.IndexOfByKey(SubNode);

    if (DecoratorIdx >= 0)
    {
        Decorators.RemoveAt(DecoratorIdx);
    }
}

void USpawnQueryGraphNode::RemoveAllSubNodes()
{
    Super::RemoveAllSubNodes();

    Decorators.Reset();
}

void USpawnQueryGraphNode::InsertSubNodeAt(UAIGraphNode* SubNode, int32 DropIndex)
{
    UE_LOG(LogTemp, Warning, TEXT("Insert Subnode"));
    USpawnQueryGraphNode_Decorator* TypedNode = Cast<USpawnQueryGraphNode_Decorator>(SubNode);

    if (TypedNode)
    {
        if (DropIndex >= 0)
        {
            Decorators.Insert(TypedNode, DropIndex);
        }
        else
        {
            Decorators.Add(TypedNode);
        }
    }
}

USpawnQueryGraph* USpawnQueryGraphNode::GetSpawnQueryGraph()
{
    return CastChecked<USpawnQueryGraph>(GetGraph());
}

bool USpawnQueryGraphNode::CanCreateUnderSpecifiedSchema(const UEdGraphSchema* DesiredSchema) const
{
    return DesiredSchema->GetClass()->IsChildOf(UEdGraphSchema_SpawnQuery::StaticClass());
}

FLinearColor USpawnQueryGraphNode::GetBackgroundColor() const
{
    return SpawnQueryColors::NodeBody::Default;
}

void USpawnQueryGraphNode::CreateAddDecoratorSubMenu(UToolMenu* Menu, UEdGraph* Graph) const
{
    TSharedRef<SGraphEditorActionMenuAI> Widget =
        SNew(SGraphEditorActionMenuAI)
        .GraphObj(Graph)
        .GraphNode(const_cast<USpawnQueryGraphNode*>(this))
        .SubNodeFlags(ESubNode::Decorator)
        .AutoExpandActionMenu(true);

    FToolMenuSection& Section = Menu->FindOrAddSection("Section");
    Section.AddEntry(FToolMenuEntry::InitWidget("DecoratorWidget", Widget, FText(), true));
}

#undef LOCTEXT_NAMESPACE
