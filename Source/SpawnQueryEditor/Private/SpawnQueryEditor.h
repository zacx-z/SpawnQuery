#pragma once

#include "AIGraphEditor.h"
#include "ISpawnQueryEditor.h"

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

protected:
    virtual void SaveAsset_Execute() override;
    void BindCommands();
    void ExtendToolbar();

    void OnToolbar_OpenSettings();

private:
    /** Create widget for graph editing */
    TSharedRef<class SGraphEditor> CreateGraphEditorWidget(UEdGraph* InGraph);

    /** Creates all internal widgets for the tabs to point at */
    void CreateInternalWidgets();

    /** Called when the selection changes in the GraphEditor */
    void OnSelectedNodesChanged(const TSet<UObject*>& NewSelection) override;

    /** Spawns the tab with the update graph inside */
    TSharedRef<SDockTab> SpawnTab_QueryGraph(const FSpawnTabArgs& Args);
    TSharedRef<SDockTab> SpawnTab_Properties(const FSpawnTabArgs& Args);

private:
    USpawnQuery* Query = nullptr;
    TSharedPtr<IDetailsView> DetailsView;

    static const FName QueryGraphTabId;
    static const FName PropertiesTabId;
};
