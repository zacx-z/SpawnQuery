// Copyright Epic Games, Inc. All Rights Reserved.

#include "SpawnQueryEditorModule.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "ToolMenus.h"

#include "AIGraphTypes.h"
#include "SpawnQueryGraphNode.h"
#include "SpawnQuery.h"
#include "SpawnQuery/SpawnQueryNode.h"

#include "PropertyEditorModule.h"
#include "SpawnQueryEditor.h"
#include "SGraphNode_SpawnQuery.h"

#include "EdGraphUtilities.h"
#include "SpawnQueryModule.h"

const FName FSpawnQueryEditorModule::SpawnQueryEditorAppIdentifier(TEXT("SpawnQueryEditorApp"));

class FGraphPanelNodeFactory_SpawnQuery : public FGraphPanelNodeFactory
{
    virtual TSharedPtr<class SGraphNode> CreateNode(UEdGraphNode* Node) const override
    {
        if (USpawnQueryGraphNode* SpawnQueryNode = Cast<USpawnQueryGraphNode>(Node))
        {
            return SNew(SGraphNode_SpawnQuery, SpawnQueryNode);
        }

        return NULL;
    }
};


TSharedPtr<FGraphPanelNodeFactory> GraphPanelNodeFactory_SpawnQuery;

#define LOCTEXT_NAMESPACE "FSpawnQueryEditorModule"

void FSpawnQueryEditorModule::StartupModule()
{
    // This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
    GraphPanelNodeFactory_SpawnQuery = MakeShareable(new FGraphPanelNodeFactory_SpawnQuery());
    FEdGraphUtilities::RegisterVisualNodeFactory(GraphPanelNodeFactory_SpawnQuery);

    FEditorDelegates::PreBeginPIE.AddRaw(this, &FSpawnQueryEditorModule::OnPreBeginPIE);
}

void FSpawnQueryEditorModule::ShutdownModule()
{
    // This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
    // we call this function before unloading the module.

    FEdGraphUtilities::UnregisterVisualNodeFactory(GraphPanelNodeFactory_SpawnQuery);

    FEditorDelegates::PreBeginPIE.RemoveAll(this);
}

TSharedRef<ISpawnQueryEditor> FSpawnQueryEditorModule::CreateSpawnQueryEditor(const EToolkitMode::Type Mode, const TSharedPtr< IToolkitHost >& InitToolkitHost, USpawnQuery* Query)
{
    if (!ClassCache.IsValid())
    {
        ClassCache = MakeShareable(new FGraphNodeClassHelper(USpawnQueryNode::StaticClass()));
    }

    TSharedRef<FSpawnQueryEditor> NewSpawnQueryEditor(new FSpawnQueryEditor());
    NewSpawnQueryEditor->InitSpawnQueryEditor(Mode, InitToolkitHost, Query);
    return NewSpawnQueryEditor;
}

void FSpawnQueryEditorModule::OnPreBeginPIE(bool bIsSimulating)
{
    FSpawnQueryModule* SpawnQueryModule = FModuleManager::GetModulePtr<FSpawnQueryModule>(TEXT("SpawnQuery"));
    if (SpawnQueryModule)
    {
        SpawnQueryModule->GetDefaultContext()->Reset();
    }
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FSpawnQueryEditorModule, SpawnQueryEditor)
