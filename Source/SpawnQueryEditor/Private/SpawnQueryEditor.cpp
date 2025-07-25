#include "SpawnQueryEditor.h"
#include "SpawnQuery.h"
#include "SpawnQueryGraph.h"
#include "SpawnQueryEditorModule.h"

#define LOCTEXT_NAMESPACE "SpawnQueryEditor"

const FName FSpawnQueryEditor::QueryGraphTabId(TEXT("SpawnQueryEditor_QueryGraph"));

void FSpawnQueryEditor::RegisterTabSpawners(const TSharedRef<class FTabManager>& InTabManager)
{
	WorkspaceMenuCategory = InTabManager->AddLocalWorkspaceMenuCategory(LOCTEXT("WorkspaceMenu_SpawnQueryEditor", "Spawn Query Editor"));
	auto WorkspaceMenuCategoryRef = WorkspaceMenuCategory.ToSharedRef();

	FAssetEditorToolkit::RegisterTabSpawners(InTabManager);

	InTabManager->RegisterTabSpawner(QueryGraphTabId, FOnSpawnTab::CreateSP(this, &FSpawnQueryEditor::SpawnTab_QueryGraph))
		.SetDisplayName(NSLOCTEXT("SpawnQueryEditor", "Graph", "Graph"))
		.SetGroup(WorkspaceMenuCategoryRef)
		.SetIcon(FSlateIcon(FAppStyle::GetAppStyleSetName(), "GraphEditor.EventGraph_16x"));
}

void FSpawnQueryEditor::UnregisterTabSpawners(const TSharedRef<class FTabManager>& InTabManager)
{
	FAssetEditorToolkit::UnregisterTabSpawners(InTabManager);

	InTabManager->UnregisterTabSpawner(QueryGraphTabId);
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
				)
			)
		);

	const bool bCreateDefaultStandaloneMenu = true;
	const bool bCreateDefaultToolbar = true;
	FAssetEditorToolkit::InitAssetEditor(Mode, InitToolkitHost, FSpawnQueryEditorModule::SpawnQueryEditorAppIdentifier, StandaloneDefaultLayout, bCreateDefaultStandaloneMenu, bCreateDefaultToolbar, Query);

	FSpawnQueryEditorModule& SpawnQueryEditorModule = FModuleManager::LoadModuleChecked<FSpawnQueryEditorModule>("SpawnQueryEditor");

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

#undef LOCTEXT_NAMESPACE