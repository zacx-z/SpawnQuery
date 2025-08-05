#pragma once

#include "AIGraphEditor.h"
#include "ISpawnQueryEditor.h"
#include "SpawnQueryDebugger.h"

class IDetailsView;
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

    TWeakObjectPtr<USpawnQueryContext> GetCurrentDebugContext();
    void SetCurrentDebugContext(TWeakObjectPtr<USpawnQueryContext> InContext);

protected:
    virtual void SaveAsset_Execute() override;
    void BindCommands();
    void ExtendToolbar();
    void FillToolbar(FToolBarBuilder& ToolbarBuilder);

    void OnToolbar_OpenSettings();

private:
    /** Create widget for graph editing */
    TSharedRef<class SGraphEditor> CreateGraphEditorWidget(UEdGraph* InGraph);

    /** Creates all internal widgets for the tabs to point at */
    void CreateInternalWidgets();

    /** Called when the selection changes in the GraphEditor */
    void OnSelectedNodesChanged(const TSet<UObject*>& NewSelection) override;
    virtual void PostUndo(bool bSuccess) override;

    void OnFinishedChangingProperties(const FPropertyChangedEvent& PropertyChangedEvent);

    /** Spawns the tab with the update graph inside */
    TSharedRef<SDockTab> SpawnTab_QueryGraph(const FSpawnTabArgs& Args);
    TSharedRef<SDockTab> SpawnTab_Properties(const FSpawnTabArgs& Args);

private:
    USpawnQuery* Query = nullptr;
    TSharedPtr<IDetailsView> DetailsView;
    TSharedPtr<class FSpawnQueryDebugger> Debugger;

    static const FName QueryGraphTabId;
    static const FName PropertiesTabId;
};
