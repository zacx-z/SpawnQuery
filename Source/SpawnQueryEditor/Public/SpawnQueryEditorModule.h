// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"
#include "Toolkits/AssetEditorToolkit.h"

class ISpawnQueryEditor;
class USpawnQuery;

class FToolBarBuilder;
class FMenuBuilder;
struct FGraphNodeClassHelper;

class FSpawnQueryEditorModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	virtual TSharedRef<ISpawnQueryEditor> CreateSpawnQueryEditor(const EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost>& InitToolkitHost, USpawnQuery* Query);
	
	static const FName SpawnQueryEditorAppIdentifier;

	TSharedPtr<FGraphNodeClassHelper> GetClassCache() { return ClassCache; }

private:
    void OnPreBeginPIE(bool bIsSimulating);

private:
	TSharedPtr<FGraphNodeClassHelper> ClassCache;
};
