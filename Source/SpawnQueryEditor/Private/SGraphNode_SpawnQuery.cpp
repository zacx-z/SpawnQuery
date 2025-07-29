#include "SGraphNode_SpawnQuery.h"
#include "Widgets/Notifications/SErrorText.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Notifications/SProgressBar.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Input/SCheckBox.h"

#include "SpawnQueryGraph.h"
#include "SpawnQueryGraphNode.h"
#include "SpawnQuery/SpawnQueryNode.h"
#include "SpawnQuery/SpawnQueryNode_Sampler.h"
#include "SpawnQuery/SpawnQueryNode_Composite.h"

#include "GraphEditorSettings.h"
#include "SCommentBubble.h"
#include "NodeFactory.h"
#include "SGraphPanel.h"
#include "SpawnQueryColors.h"
#include "Widgets/SOverlay.h"
#include "Widgets/Text/SInlineEditableTextBlock.h"
#include "SLevelOfDetailBranchNode.h"
#include "SpawnQueryDebugger.h"
#include "Widgets/Text/STextBlock.h"

#define LOCTEXT_NAMESPACE "SpawnQueryEditor"

class SSpawnQueryPin : public SGraphPinAI
{
public:
    SLATE_BEGIN_ARGS(SSpawnQueryPin) {}
    SLATE_END_ARGS()

    void Construct(const FArguments& InArgs, UEdGraphPin* InPin);

    virtual FSlateColor GetPinColor() const override;
};

void SSpawnQueryPin::Construct(const FArguments& InArgs, UEdGraphPin* InPin)
{
    SGraphPinAI::Construct(SGraphPinAI::FArguments(), InPin);
}

FSlateColor SSpawnQueryPin::GetPinColor() const
{
    return IsHovered() ? SpawnQueryColors::Pin::Hover :
        SpawnQueryColors::Pin::Default;
}

/////////////////////////////////////////////////////
// SGraphNode_SpawnQuery

void SGraphNode_SpawnQuery::Construct(const FArguments& InArgs, USpawnQueryGraphNode* InNode)
{
    SGraphNodeAI::Construct(SGraphNodeAI::FArguments(), InNode);
}

void SGraphNode_SpawnQuery::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
    SGraphNodeAI::Tick(AllottedGeometry, InCurrentTime, InDeltaTime);
}

void SGraphNode_SpawnQuery::AddSubNode(TSharedPtr<SGraphNode> SubNodeWidget)
{
    SGraphNodeAI::AddSubNode(SubNodeWidget);

    TestBox->AddSlot().AutoHeight()
        [
            SubNodeWidget.ToSharedRef()
        ];
}

FSlateColor SGraphNode_SpawnQuery::GetBorderBackgroundColor() const
{
    const bool bSelected = GetOwnerPanel().IsValid() && GetOwnerPanel()->SelectionManager.SelectedNodes.Contains(GraphNode);

    return bSelected ? SpawnQueryColors::NodeBorder::Selected :
        SpawnQueryColors::NodeBorder::Default;
}

FSlateColor SGraphNode_SpawnQuery::GetBackgroundColor() const
{
    const USpawnQueryGraphNode* MyNode = Cast<USpawnQueryGraphNode>(GraphNode);
    const UClass* MyClass = MyNode && MyNode->NodeInstance ? MyNode->NodeInstance->GetClass() : NULL;

    FLinearColor NodeColor = SpawnQueryColors::NodeBody::Default;
    if (MyClass)
    {
        if (MyClass->IsChildOf(USpawnQueryNode_Sampler::StaticClass()))
        {
            NodeColor = SpawnQueryColors::NodeBody::Sample;
        }
    }

    if (FSpawnQueryDebugger::IsPIESimulating() && !MyNode->bDebuggerActiveState)
    {
        NodeColor = SpawnQueryColors::NodeBody::Inactive;
    }

    if (!MyNode || MyNode->HasErrors())
    {
        NodeColor = SpawnQueryColors::NodeBody::Error;
    }

    return NodeColor;
}

void SGraphNode_SpawnQuery::UpdateGraphNode()
{
    if (TestBox.IsValid())
    {
        TestBox->ClearChildren();
    }
    else
    {
        SAssignNew(TestBox, SVerticalBox);
    }

    InputPins.Empty();
    OutputPins.Empty();

    // Reset variables that are going to be exposed, in case we are refreshing an already setup node.
    RightNodeBox.Reset();
    LeftNodeBox.Reset();
    SubNodes.Reset();

    const FMargin NodePadding = FMargin(2.0f);

    const FSlateBrush* NodeTypeIcon = GetNameIcon();

    FLinearColor TitleShadowColor(1.0f, 0.0f, 0.0f);
    TSharedPtr<SErrorText> ErrorText;
    TSharedPtr<STextBlock> DescriptionText;
    TSharedPtr<SNodeTitle> NodeTitle = SNew(SNodeTitle, GraphNode);

    TWeakPtr<SNodeTitle> WeakNodeTitle = NodeTitle;
    auto GetNodeTitlePlaceholderWidth = [WeakNodeTitle]() -> FOptionalSize
        {
            TSharedPtr<SNodeTitle> NodeTitlePin = WeakNodeTitle.Pin();
            const float DesiredWidth = (NodeTitlePin.IsValid()) ? NodeTitlePin->GetTitleSize().X : 0.0f;
            return FMath::Max(75.0f, DesiredWidth);
        };
    auto GetNodeTitlePlaceholderHeight = [WeakNodeTitle]() -> FOptionalSize
        {
            TSharedPtr<SNodeTitle> NodeTitlePin = WeakNodeTitle.Pin();
            const float DesiredHeight = (NodeTitlePin.IsValid()) ? NodeTitlePin->GetTitleSize().Y : 0.0f;
            return FMath::Max(22.0f, DesiredHeight);
        };

    this->ContentScale.Bind(this, &SGraphNode::GetContentScale);
    this->GetOrAddSlot(ENodeZone::Center)
        .HAlign(HAlign_Fill)
        .VAlign(VAlign_Center)
        [
            SNew(SBorder)
                .BorderImage(FAppStyle::GetBrush("Graph.StateNode.Body"))
                .Padding(0)
                .BorderBackgroundColor(this, &SGraphNode_SpawnQuery::GetBorderBackgroundColor)
                .OnMouseButtonDown(this, &SGraphNode_SpawnQuery::OnMouseDown)
                [
                    SNew(SOverlay)

                        // Pins and node details
                        + SOverlay::Slot()
                        .HAlign(HAlign_Fill)
                        .VAlign(VAlign_Fill)
                        [
                            SNew(SVerticalBox)

                                // INPUT PIN AREA
                                + SVerticalBox::Slot()
                                .AutoHeight()
                                [
                                    SNew(SBox)
                                        .MinDesiredHeight(NodePadding.Top)
                                        [
                                            SAssignNew(LeftNodeBox, SVerticalBox)
                                        ]
                                ]

                            // STATE NAME AREA
                            + SVerticalBox::Slot()
                                .Padding(FMargin(NodePadding.Left, 0.0f, NodePadding.Right, 0.0f))
                                [
                                    SNew(SHorizontalBox)
                                    + SHorizontalBox::Slot()
                                        .FillWidth(1.0f)
                                        [
                                            SNew(SVerticalBox)
                                                + SVerticalBox::Slot()
                                                .AutoHeight()
                                                [
                                                    SNew(SBorder)
                                                        .BorderImage(FAppStyle::GetBrush("Graph.StateNode.Body"))
                                                        .BorderBackgroundColor(this, &SGraphNode_SpawnQuery::GetBackgroundColor)
                                                        .HAlign(HAlign_Fill)
                                                        .VAlign(VAlign_Center)
                                                        .Visibility(EVisibility::SelfHitTestInvisible)
                                                        [
                                                            SNew(SVerticalBox)
                                                                + SVerticalBox::Slot()
                                                                .AutoHeight()
                                                                [
                                                                    SNew(SHorizontalBox)
                                                                        + SHorizontalBox::Slot()
                                                                        .AutoWidth()
                                                                        [
                                                                            // POPUP ERROR MESSAGE
                                                                            SAssignNew(ErrorText, SErrorText)
                                                                                .BackgroundColor(this, &SGraphNode_SpawnQuery::GetErrorColor)
                                                                                .ToolTipText(this, &SGraphNode_SpawnQuery::GetErrorMsgToolTip)
                                                                        ]
                                                                        + SHorizontalBox::Slot()
                                                                        .Padding(FMargin(4.0f, 0.0f, 4.0f, 0.0f))
                                                                        [
                                                                            SNew(SLevelOfDetailBranchNode)
                                                                                .UseLowDetailSlot(this, &SGraphNode_SpawnQuery::UseLowDetailNodeTitles)
                                                                                .LowDetail()
                                                                                [
                                                                                    SNew(SBox)
                                                                                        .WidthOverride_Lambda(GetNodeTitlePlaceholderWidth)
                                                                                        .HeightOverride_Lambda(GetNodeTitlePlaceholderHeight)
                                                                                ]
                                                                                .HighDetail()
                                                                                [
                                                                                    SNew(SVerticalBox)
                                                                                        + SVerticalBox::Slot()
                                                                                        .AutoHeight()
                                                                                        [
                                                                                            SAssignNew(InlineEditableText, SInlineEditableTextBlock)
                                                                                                .Style(FAppStyle::Get(), "Graph.StateNode.NodeTitleInlineEditableText")
                                                                                                .Text(this, &SGraphNode_SpawnQuery::GetTitle)
                                                                                                .OnVerifyTextChanged(this, &SGraphNode_SpawnQuery::OnVerifyNameTextChanged)
                                                                                                .OnTextCommitted(this, &SGraphNode_SpawnQuery::OnNameTextCommited)
                                                                                                .IsReadOnly(this, &SGraphNode_SpawnQuery::IsNameReadOnly)
                                                                                                .IsSelected(this, &SGraphNode_SpawnQuery::IsSelectedExclusively)
                                                                                        ]
                                                                                    + SVerticalBox::Slot()
                                                                                        .AutoHeight()
                                                                                        [
                                                                                            NodeTitle.ToSharedRef()
                                                                                        ]
                                                                                ]
                                                                        ]
                                                                ]
                                                            + SVerticalBox::Slot()
                                                                .AutoHeight()
                                                                [
                                                                    // DESCRIPTION MESSAGE
                                                                    SAssignNew(DescriptionText, STextBlock)
                                                                        .Visibility(this, &SGraphNode_SpawnQuery::GetDescriptionVisibility)
                                                                        .Text(this, &SGraphNode_SpawnQuery::GetDescription)
                                                                ]
                                                        ]
                                                ]
                                            + SVerticalBox::Slot()
                                                .AutoHeight()
                                                .Padding(0, 0, 0, 0)
                                                [
                                                    TestBox.ToSharedRef()
                                                ]
                                        ]
                                ]

                            // OUTPUT PIN AREA
                            + SVerticalBox::Slot()
                                .AutoHeight()
                                [
                                    SNew(SBox)
                                        .MinDesiredHeight(NodePadding.Bottom)
                                        [
                                            SAssignNew(RightNodeBox, SVerticalBox)
                                        ]
                                ]

                            // Profiler overlay: option
                            /* + SVerticalBox::Slot()
                                .AutoHeight()
                                [
                                    SNew(SBorder)
                                        .BorderBackgroundColor(SpawnQueryColors::Action::Profiler)
                                        .Visibility(this, &SGraphNode_SpawnQuery::GetProfilerOptionVisibility)
                                        [
                                            SNew(STextBlock)
                                                .Text(this, &SGraphNode_SpawnQuery::GetProfilerDescOption)
                                        ]
                                ]*/
                        ]

                    // Drag marker overlay
                    + SOverlay::Slot()
                        .HAlign(HAlign_Fill)
                        .VAlign(VAlign_Top)
                        [
                            SNew(SBorder)
                                .BorderBackgroundColor(SpawnQueryColors::Action::DragMarker)
                                .ColorAndOpacity(SpawnQueryColors::Action::DragMarker)
                                .BorderImage(FAppStyle::GetBrush("Graph.StateNode.Body"))
                                .Visibility(this, &SGraphNode_SpawnQuery::GetDragOverMarkerVisibility)
                                [
                                    SNew(SBox)
                                        .HeightOverride(4)
                                ]
                        ]

                    // Profiler overlay: test
                    /* + SOverlay::Slot()
                        .HAlign(HAlign_Right)
                        .VAlign(VAlign_Fill)
                        .Padding(10, 5)
                        [
                            SNew(SBorder)
                                .BorderBackgroundColor(SpawnQueryColors::Action::Profiler)
                                .BorderImage(FAppStyle::GetBrush("Graph.StateNode.Body"))
                                .Visibility(this, &SGraphNode_SpawnQuery::GetProfilerTestVisibility)
                                [
                                    SNew(SHorizontalBox)
                                        + SHorizontalBox::Slot()
                                        .AutoWidth()
                                        [
                                            SNew(SBorder)
                                                .BorderImage(FAppStyle::GetBrush("Graph.StateNode.Body"))
                                                .BorderBackgroundColor(this, &SGraphNode_SpawnQuery::GetProfilerTestSlateColor)
                                                [
                                                    SNew(SBox)
                                                        .WidthOverride(10.0f)
                                                ]
                                        ]
                                    + SHorizontalBox::Slot()
                                        .Padding(2, 0, 0, 0)
                                        [
                                            SNew(SVerticalBox)
                                                + SVerticalBox::Slot()
                                                .VAlign(VAlign_Center)
                                                [
                                                    SNew(STextBlock)
                                                        .Text(this, &SGraphNode_SpawnQuery::GetProfilerDescAverage)
                                                ]
                                                + SVerticalBox::Slot()
                                                .VAlign(VAlign_Center)
                                                [
                                                    SNew(STextBlock)
                                                        .Text(this, &SGraphNode_SpawnQuery::GetProfilerDescWorst)
                                                ]
                                        ]
                                ]
                        ]*/
                ]
        ];

    // Create comment bubble
    TSharedPtr<SCommentBubble> CommentBubble;
    const FSlateColor CommentColor = GetDefault<UGraphEditorSettings>()->DefaultCommentNodeTitleColor;

    SAssignNew(CommentBubble, SCommentBubble)
        .GraphNode(GraphNode)
        .Text(this, &SGraphNode::GetNodeComment)
        .OnTextCommitted(this, &SGraphNode::OnCommentTextCommitted)
        .ColorAndOpacity(CommentColor)
        .AllowPinning(true)
        .EnableTitleBarBubble(true)
        .EnableBubbleCtrls(true)
        .GraphLOD(this, &SGraphNode::GetCurrentLOD)
        .IsGraphNodeHovered(this, &SGraphNode::IsHovered);

    GetOrAddSlot(ENodeZone::TopCenter)
        .SlotOffset2f(TAttribute<FVector2f>(CommentBubble.Get(), &SCommentBubble::GetOffset2f))
        .SlotSize2f(TAttribute<FVector2f>(CommentBubble.Get(), &SCommentBubble::GetSize2f))
        .AllowScaling(TAttribute<bool>(CommentBubble.Get(), &SCommentBubble::IsScalingAllowed))
        .VAlign(VAlign_Top)
        [
            CommentBubble.ToSharedRef()
        ];

    ErrorReporting = ErrorText;
    ErrorReporting->SetError(ErrorMsg);
    CreatePinWidgets();
}

void SGraphNode_SpawnQuery::CreatePinWidgets()
{
    USpawnQueryGraphNode* StateNode = CastChecked<USpawnQueryGraphNode>(GraphNode);

    UEdGraphPin* CurPin = StateNode->GetOutputPin();
    if (CurPin && !CurPin->bHidden)
    {
        TSharedPtr<SGraphPin> NewPin = SNew(SSpawnQueryPin, CurPin);

        AddPin(NewPin.ToSharedRef());
    }

    CurPin = StateNode->GetInputPin();
    if (CurPin && !CurPin->bHidden)
    {
        TSharedPtr<SGraphPin> NewPin = SNew(SSpawnQueryPin, CurPin);

        AddPin(NewPin.ToSharedRef());
    }
}


#undef LOCTEXT_NAMESPACE
