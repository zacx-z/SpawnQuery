#include "SpawnQueryEditor.h"
#include "SpawnQuery.h"
#include "SpawnQueryGraph.h"
#include "SpawnQueryEditorModule.h"

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

	// Update BT asset data based on saved graph to have correct data in editor
	TSharedPtr<SGraphEditor> UpdateGraphEditor = UpdateGraphEdPtr.Pin();
	if (UpdateGraphEditor.IsValid() && UpdateGraphEditor->GetCurrentGraph() != NULL)
	{
		//let's find root node
		USpawnQueryGraph* SpawnQueryGraph = Cast<USpawnQueryGraph>(UpdateGraphEditor->GetCurrentGraph());
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
