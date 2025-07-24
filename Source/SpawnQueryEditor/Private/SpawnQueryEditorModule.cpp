// Copyright Epic Games, Inc. All Rights Reserved.

#include "SpawnQueryEditorModule.h"
#include "SpawnQueryEditorStyle.h"
#include "SpawnQueryEditorCommands.h"
#include "LevelEditor.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "ToolMenus.h"

#include "AIGraphTypes.h"
#include "SpawnQuery.h"

#include "PropertyEditorModule.h"
#include "SpawnQueryEditor.h"

#include "EdGraphUtilities.h"

static const FName SpawnQuerySystemTabName("SpawnQuerySystem");

const FName FSpawnQueryEditorModule::SpawnQueryEditorAppIdentifier(TEXT("SpawnQueryEditorApp"));

#define LOCTEXT_NAMESPACE "FSpawnQueryEditorModule"

void FSpawnQueryEditorModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FSpawnQueryEditorStyle::Initialize();
	FSpawnQueryEditorStyle::ReloadTextures();

	FSpawnQueryEditorCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FSpawnQueryEditorCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FSpawnQueryEditorModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FSpawnQueryEditorModule::RegisterMenus));
	
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(SpawnQuerySystemTabName, FOnSpawnTab::CreateRaw(this, &FSpawnQueryEditorModule::OnSpawnPluginTab))
		.SetDisplayName(LOCTEXT("FSpawnQuerySystemTabTitle", "SpawnQuerySystem"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);
}

void FSpawnQueryEditorModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FSpawnQueryEditorStyle::Shutdown();

	FSpawnQueryEditorCommands::Unregister();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(SpawnQuerySystemTabName);
}

TSharedRef<SDockTab> FSpawnQueryEditorModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	FText WidgetText = FText::Format(
		LOCTEXT("WindowWidgetText", "Add code to {0} in {1} to override this window's contents"),
		FText::FromString(TEXT("FSpawnQueryEditorModule::OnSpawnPluginTab")),
		FText::FromString(TEXT("SpawnQueryEditorModule.cpp"))
		);

	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			// Put your tab content here!
			SNew(SBox)
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				SNew(STextBlock)
				.Text(WidgetText)
			]
		];
}

void FSpawnQueryEditorModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->TryInvokeTab(SpawnQuerySystemTabName);
}

void FSpawnQueryEditorModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FSpawnQueryEditorCommands::Get().OpenPluginWindow, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.PlayToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("PluginTools");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FSpawnQueryEditorCommands::Get().OpenPluginWindow));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}

TSharedRef<ISpawnQueryEditor> FSpawnQueryEditorModule::CreateSpawnQueryEditor(const EToolkitMode::Type Mode, const TSharedPtr< IToolkitHost >& InitToolkitHost, USpawnQuery* Query)
{
	if (!ClassCache.IsValid())
	{
		//ClassCache = MakeShareable(new FGraphNodeClassHelper(USpawnQueryNode::StaticClass()));
	}

	TSharedRef<FSpawnQueryEditor> NewSpawnQueryEditor(new FSpawnQueryEditor());
	NewSpawnQueryEditor->InitSpawnQueryEditor(Mode, InitToolkitHost, Query);
	return NewSpawnQueryEditor;
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FSpawnQueryEditorModule, SpawnQueryEditor)