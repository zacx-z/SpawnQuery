#include "SpawnQueryEditor.h"
#include "SpawnQuery.h"
#include "SpawnQueryDebugger.h"
#include "SpawnQueryGraph.h"
#include "SpawnQueryEditorModule.h"
#include "SpawnQueryEditorStyle.h"
#include "SpawnQueryGraphNode_Root.h"
#include "SSpawnQueryEditorSelectedDebugContextWidget.h"

#define LOCTEXT_NAMESPACE "SpawnQueryEditor"

const FName FSpawnQueryEditor::QueryGraphTabId(TEXT("SpawnQueryEditor_QueryGraph"));
const FName FSpawnQueryEditor::PropertiesTabId(TEXT("SpawnQueryEditor_Properties"));

class FEnvQueryCommands : public TCommands<FEnvQueryCommands>
{
public:
    FEnvQueryCommands() : TCommands<FEnvQueryCommands>("SpawnQueryEditor", LOCTEXT("SpawnQuery", "SpawnQuery"), NAME_None, FSpawnQueryEditorStyle::GetStyleSetName())
    {
    }

    TSharedPtr<FUICommandInfo> Settings;
    TSharedPtr<FUICommandInfo> ContextTarget;

    /** Initialize commands */
    virtual void RegisterCommands() override
    {
        UI_COMMAND(Settings, "Settings", "Open SpawnQuery Settings", EUserInterfaceActionType::Button, FInputChord());
        UI_COMMAND(ContextTarget, "Context", "Choose a SpawnQueryContext to view", EUserInterfaceActionType::Button, FInputChord());
    }
};

void FSpawnQueryEditor::RegisterTabSpawners(const TSharedRef<class FTabManager>& InTabManager)
{
    WorkspaceMenuCategory = InTabManager->AddLocalWorkspaceMenuCategory(LOCTEXT("WorkspaceMenu_SpawnQueryEditor", "Spawn Query Editor"));
    auto WorkspaceMenuCategoryRef = WorkspaceMenuCategory.ToSharedRef();

    FAssetEditorToolkit::RegisterTabSpawners(InTabManager);

    InTabManager->RegisterTabSpawner(QueryGraphTabId, FOnSpawnTab::CreateSP(this, &FSpawnQueryEditor::SpawnTab_QueryGraph))
        .SetDisplayName(NSLOCTEXT("SpawnQueryEditor", "Graph", "Graph"))
        .SetGroup(WorkspaceMenuCategoryRef)
        .SetIcon(FSlateIcon(FAppStyle::GetAppStyleSetName(), "GraphEditor.EventGraph_16x"));
    
    InTabManager->RegisterTabSpawner(PropertiesTabId, FOnSpawnTab::CreateSP(this, &FSpawnQueryEditor::SpawnTab_Properties))
        .SetDisplayName(NSLOCTEXT("SpawnQueryEditor", "PropertiesTab", "Details"))
        .SetGroup(WorkspaceMenuCategoryRef)
        .SetIcon(FSlateIcon(FAppStyle::GetAppStyleSetName(), "LevelEditor.Tabs.Details"));
}

void FSpawnQueryEditor::UnregisterTabSpawners(const TSharedRef<class FTabManager>& InTabManager)
{
    FAssetEditorToolkit::UnregisterTabSpawners(InTabManager);

    InTabManager->UnregisterTabSpawner(QueryGraphTabId);
    InTabManager->UnregisterTabSpawner(PropertiesTabId);
}

void FSpawnQueryEditor::InitSpawnQueryEditor(const EToolkitMode::Type Mode, const TSharedPtr<class IToolkitHost>& InitToolkitHost, USpawnQuery* InScript)
{
    Query = InScript;
    check(Query != NULL);

    TSharedRef<FTabManager::FLayout> StandaloneDefaultLayout = FTabManager::NewLayout("Standalone_SpawnQuery_Layout")
        ->AddArea
        (
            FTabManager::NewPrimaryArea()->SetOrientation(Orient_Vertical)
            ->Split
            (
                FTabManager::NewSplitter()->SetOrientation(Orient_Horizontal)
                ->Split
                (
                    FTabManager::NewStack()
                    ->SetSizeCoefficient(0.7f)
                    ->AddTab(FSpawnQueryEditor::QueryGraphTabId, ETabState::OpenedTab)
                )
                ->Split
                (
                    FTabManager::NewStack()
                    ->SetSizeCoefficient(0.3f)
                    ->AddTab( PropertiesTabId, ETabState::OpenedTab )
                )
            )
        );

    const bool bCreateDefaultStandaloneMenu = true;
    const bool bCreateDefaultToolbar = true;
    FAssetEditorToolkit::InitAssetEditor(Mode, InitToolkitHost, FSpawnQueryEditorModule::SpawnQueryEditorAppIdentifier, StandaloneDefaultLayout, bCreateDefaultStandaloneMenu, bCreateDefaultToolbar, Query);

    Debugger = MakeShareable(new FSpawnQueryDebugger);
    Debugger->Setup(Query, SharedThis(this));

    FSpawnQueryEditorModule& SpawnQueryEditorModule = FModuleManager::LoadModuleChecked<FSpawnQueryEditorModule>("SpawnQueryEditor");

    BindCommands();
    ExtendToolbar();
    RegenerateMenusAndToolbars();

    // Update asset data based on saved graph to have correct data in editor
    TSharedPtr<SGraphEditor> QueryGraphEditor = UpdateGraphEdPtr.Pin();
    if (QueryGraphEditor.IsValid() && QueryGraphEditor->GetCurrentGraph() != NULL)
    {
        //let's find root node
        USpawnQueryGraph* SpawnQueryGraph = Cast<USpawnQueryGraph>(QueryGraphEditor->GetCurrentGraph());
        SpawnQueryGraph->UpdateAsset();
    }
}

FName FSpawnQueryEditor::GetToolkitFName() const
{
    return FName("Spawn Query");
}

FText FSpawnQueryEditor::GetBaseToolkitName() const
{
    return NSLOCTEXT("SpawnQueryEditor", "AppLabel", "SpawnQuery");
}

FString FSpawnQueryEditor::GetWorldCentricTabPrefix() const
{
    return NSLOCTEXT("SpawnQueryEditor", "WorldCentricTabPrefix", "SpawnQuery ").ToString();
}


FLinearColor FSpawnQueryEditor::GetWorldCentricTabColorScale() const
{
    return FLinearColor(0.0f, 0.0f, 0.2f, 0.5f);
}

TWeakObjectPtr<USpawnQueryContext> FSpawnQueryEditor::GetCurrentDebugContext()
{
    return Debugger->GetCurrentDebugContext();
}

void FSpawnQueryEditor::SetCurrentDebugContext(TWeakObjectPtr<USpawnQueryContext> InContext)
{
    Debugger->SetCurrentDebugContext(InContext);
}

void FSpawnQueryEditor::SaveAsset_Execute()
{
    if (Query)
    {
        USpawnQueryGraph* SpawnQueryGraph = Cast<USpawnQueryGraph>(Query->EdGraph);
        if (SpawnQueryGraph)
        {
            SpawnQueryGraph->UpdateAsset();
        }
    }
    ISpawnQueryEditor::SaveAsset_Execute();
}

void FSpawnQueryEditor::BindCommands()
{
    FEnvQueryCommands::Register();

    ToolkitCommands->MapAction(FEnvQueryCommands::Get().Settings,
        FExecuteAction::CreateRaw(this, &FSpawnQueryEditor::OnToolbar_OpenSettings)
        );
}

void FSpawnQueryEditor::ExtendToolbar()
{
    TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender);
    ToolbarExtender->AddToolBarExtension(
        "Asset",
        EExtensionHook::After,
        ToolkitCommands,
        FToolBarExtensionDelegate::CreateRaw(this, &FSpawnQueryEditor::FillToolbar)
        );

    AddToolbarExtender(ToolbarExtender);
}

void FSpawnQueryEditor::FillToolbar(FToolBarBuilder& ToolbarBuilder)
{
    ToolbarBuilder.BeginSection("Settings");
    {
        ToolbarBuilder.AddToolBarButton(FEnvQueryCommands::Get().Settings,
            NAME_None,
            TAttribute<FText>(),
            TAttribute<FText>(),
            FSlateIcon("EditorStyle", "FullBlueprintEditor.EditGlobalOptions"));
        ToolbarBuilder.AddSeparator();
        ToolbarBuilder.AddWidget(SNew(SSpawnQueryEditorSelectedDebugContextWidget, SharedThis(this)));
    }
    ToolbarBuilder.EndSection();
}

void FSpawnQueryEditor::OnToolbar_OpenSettings()
{
    if (DetailsView)
    {
        DetailsView->SetObject(Query);
    }
}

TSharedRef<SGraphEditor> FSpawnQueryEditor::CreateGraphEditorWidget(UEdGraph* InGraph)
{
    check(InGraph != NULL);

    // Create the appearance info
    FGraphAppearanceInfo AppearanceInfo;
    AppearanceInfo.CornerText = NSLOCTEXT("SpawnQueryEditor", "AppearanceCornerText", "SPAWN QUERY");

    SGraphEditor::FGraphEditorEvents InEvents;
    InEvents.OnSelectionChanged = SGraphEditor::FOnSelectionChanged::CreateSP(this, &FSpawnQueryEditor::OnSelectedNodesChanged);

    CreateCommandList();

    // Make title bar
    TSharedRef<SWidget> TitleBarWidget =
        SNew(SBorder)
        .BorderImage(FAppStyle::GetBrush(TEXT("Graph.TitleBackground")))
        .HAlign(HAlign_Fill)
        [
            SNew(SHorizontalBox)
                + SHorizontalBox::Slot()
                .HAlign(HAlign_Center)
                .FillWidth(1.f)
                [
                    SNew(STextBlock)
                        .Text(NSLOCTEXT("SpawnQueryEditor", "TheQueryGraphLabel", "Query Graph"))
                        .TextStyle(FAppStyle::Get(), TEXT("GraphBreadcrumbButtonText"))
                ]
        ];

    // Make full graph editor
    return SNew(SGraphEditor)
        .AdditionalCommands(GraphEditorCommands)
        .Appearance(AppearanceInfo)
        .TitleBar(TitleBarWidget)
        .GraphToEdit(InGraph)
        .GraphEvents(InEvents);
}

void FSpawnQueryEditor::CreateInternalWidgets()
{
    FPropertyEditorModule& PropertyEditorModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>( "PropertyEditor" );
    FDetailsViewArgs DetailsViewArgs;
    DetailsViewArgs.NameAreaSettings = FDetailsViewArgs::HideNameArea;
    DetailsView = PropertyEditorModule.CreateDetailView( DetailsViewArgs );
    DetailsView->SetObject( nullptr );
    //DetailsView->OnFinishedChangingProperties().AddSP(this, &FSpawnQueryEditor::OnFinishedChangingProperties);
}

void FSpawnQueryEditor::OnSelectedNodesChanged(const TSet<UObject*>& NewSelection)
{
    TArray<UObject*> Selection;

    if (NewSelection.Num())
    {
        for(TSet<UObject*>::TConstIterator SetIt(NewSelection);SetIt;++SetIt)
        {
            USpawnQueryGraphNode* GraphNode = Cast<USpawnQueryGraphNode>(*SetIt);
            if (GraphNode)
            {
                if (GraphNode->IsA(USpawnQueryGraphNode_Root::StaticClass()))
                {
                    Selection.Add(GraphNode);
                }
                else
                {
                    Selection.Add(GraphNode->NodeInstance);
                }
            }
        }
    }

    TSharedPtr<FTabManager> TabManagerPtr = GetTabManager();
    if (TabManagerPtr)
    {
        TabManagerPtr->TryInvokeTab(PropertiesTabId);
    }
    
    if (DetailsView)
    {
        if (Selection.Num() == 1)
        {
            DetailsView->SetObjects(Selection);
        }
        else
        {
            DetailsView->SetObject(nullptr);
        }
    }
}

TSharedRef<SDockTab> FSpawnQueryEditor::SpawnTab_QueryGraph(const FSpawnTabArgs& Args)
{
    check(Args.GetTabId().TabType == QueryGraphTabId);
    USpawnQueryGraph* MyGraph = Cast<USpawnQueryGraph>(Query->EdGraph);
    if (Query->EdGraph == NULL)
    {
        MyGraph = NewObject<USpawnQueryGraph>(Query, NAME_None, RF_Transactional);
        Query->EdGraph = MyGraph;

        // let's read data from BT script and generate nodes
        const UEdGraphSchema* Schema = Query->EdGraph->GetSchema();
        Schema->CreateDefaultNodesForGraph(*Query->EdGraph);

        MyGraph->OnCreated();
    }
    else
    {
        MyGraph->OnLoaded();
    }

    MyGraph->Initialize();

    TSharedRef<SGraphEditor> QueryGraphEditor = CreateGraphEditorWidget(Query->EdGraph);
    UpdateGraphEdPtr = QueryGraphEditor; // Keep pointer to editor

    return SNew(SDockTab)
        .Label(NSLOCTEXT("SpawnQueryEditor", "QueryGraph", "Update Graph"))
        .TabColorScale(GetTabColorScale())
        [
            QueryGraphEditor
        ];
}

TSharedRef<SDockTab> FSpawnQueryEditor::SpawnTab_Properties(const FSpawnTabArgs& Args)
{
    check(Args.GetTabId().TabType == PropertiesTabId);

    CreateInternalWidgets();

    TSharedRef<SDockTab> SpawnedTab = SNew(SDockTab)
        .Label(NSLOCTEXT("EnvironmentQueryEditor", "PropertiesTab", "Details"))
        [
            DetailsView.ToSharedRef()
        ];

    return SpawnedTab;
}

#undef LOCTEXT_NAMESPACE