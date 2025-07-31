#include "SSpawnQueryEditorSelectedDebugContextWidget.h"

#include "IDocumentation.h"
#include "SLevelOfDetailBranchNode.h"
#include "SpawnQueryEditor.h"
#include "SpawnQueryModule.h"
#include "SpawnQuery/SpawnQueryContext.h"

#define LOCTEXT_NAMESPACE "SpawnQueryEditor"

void SSpawnQueryEditorSelectedDebugContextWidget::Construct(const FArguments& InArgs,
                                                            TSharedPtr<FSpawnQueryEditor> InEditor)
{
    SpawnQueryEditor = InEditor;

    FSpawnQueryModule& SpawnQueryModule = FModuleManager::LoadModuleChecked<FSpawnQueryModule>("SpawnQuery");

    DebugContextComboBox = SNew(SComboBox<TWeakObjectPtr<USpawnQueryContext>>)
        .ToolTip(IDocumentation::Get()->CreateToolTip(
            LOCTEXT("SpawnQueryDebugContextTooltip", "Select a context to debug"),
            nullptr,
            TEXT("Shared/Editors/SpawnQueryEditor/SpawnQueryDebugger"),
    TEXT("DebugSpawnQueryContext")))
        .OptionsSource(&SpawnQueryModule.GetSpawnQueryContexts())
        .InitiallySelectedItem(InEditor->GetCurrentDebugContext())
        .OnComboBoxOpening_Lambda([&]()
        {
            SpawnQueryModule.GetSpawnQueryContexts(); // prune null pointers
        })
        .OnGenerateWidget(this, &SSpawnQueryEditorSelectedDebugContextWidget::CreateDebugContextWidget)
        .OnSelectionChanged(this, &SSpawnQueryEditorSelectedDebugContextWidget::OnDebugContextChanged)
        .ContentPadding(FMargin(0.f, 4.f))
        [
            SNew(STextBlock)
            .Text(this, &SSpawnQueryEditorSelectedDebugContextWidget::GetSelectedDebugContextTextLabel)
        ];

    ChildSlot
    [
        SNew(SLevelOfDetailBranchNode)
        .UseLowDetailSlot(FMultiBoxSettings::UseSmallToolBarIcons)
        .OnGetActiveDetailSlotContent_Lambda([&](bool bChangedToHighDetail)
        {
            return SNew(SHorizontalBox)
            + SHorizontalBox::Slot()
            .VAlign(VAlign_Bottom)
            .Padding(4.0f, 0.0f)
            .AutoWidth()
            [
                DebugContextComboBox.ToSharedRef()
            ];
        })
    ];
}

void SSpawnQueryEditorSelectedDebugContextWidget::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime,
    const float InDeltaTime)
{
    SCompoundWidget::Tick(AllottedGeometry, InCurrentTime, InDeltaTime);
}

FText SSpawnQueryEditorSelectedDebugContextWidget::GetSelectedDebugContextTextLabel() const
{
    auto Editor = SpawnQueryEditor.Pin();
    if (Editor.IsValid())
    {
        auto ContextPtr = Editor->GetCurrentDebugContext();
        if (ContextPtr.IsValid())
        {
            return FText::FromString(*ContextPtr.Pin()->GetName());
        }
    }

    return FText::FromString(TEXT("No SpawnQuery context selected"));
}

TSharedRef<SWidget> SSpawnQueryEditorSelectedDebugContextWidget::CreateDebugContextWidget(
    TWeakObjectPtr<USpawnQueryContext> InItem)
{
    FString ItemString;

    if (InItem.IsValid())
    {
        ItemString = InItem->GetName();
    }

    return SNew(STextBlock)
        .Text(FText::FromString(*ItemString));
}

void SSpawnQueryEditorSelectedDebugContextWidget::OnDebugContextChanged(
    TWeakObjectPtr<USpawnQueryContext> NewSelection, ESelectInfo::Type SelectInfo)
{
    TSharedPtr<FSpawnQueryEditor> Editor = SpawnQueryEditor.Pin();

    if (Editor.IsValid())
    {
        if (NewSelection != Editor->GetCurrentDebugContext() && NewSelection.IsValid())
        {
            Editor->SetCurrentDebugContext(NewSelection);
        }
    }
}
