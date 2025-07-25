#pragma once

#include "AIGraphEditor.h"
#include "ISpawnQueryEditor.h"

class FSpawnTabArgs;

class UEdGraph;
class USpawnQuery;

class FSpawnQueryEditor : public ISpawnQueryEditor, public FAIGraphEditor
{

public:

	virtual void RegisterTabSpawners(const TSharedRef<class FTabManager>& TabManager) override;
	virtual void UnregisterTabSpawners(const TSharedRef<class FTabManager>& TabManager) override;

	void InitSpawnQueryEditor(const EToolkitMode::Type Mode, const TSharedPtr< class IToolkitHost >& InitToolkitHost, USpawnQuery* Script);

	//~ Begin IToolkit Interface
	virtual FName GetToolkitFName() const override;
	virtual FText GetBaseToolkitName() const override;
	virtual FString GetWorldCentricTabPrefix() const override;
	virtual FLinearColor GetWorldCentricTabColorScale() const override;
	//~ End IToolkit Interface

private:
	/** Create widget for graph editing */
	TSharedRef<class SGraphEditor> CreateGraphEditorWidget(UEdGraph* InGraph);

	/** Spawns the tab with the update graph inside */
	TSharedRef<SDockTab> SpawnTab_QueryGraph(const FSpawnTabArgs& Args);

private:
	USpawnQuery* Query;

	static const FName QueryGraphTabId;
};
