// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Framework/Commands/Commands.h"
#include "SpawnQueryEditorStyle.h"

class FSpawnQueryEditorCommands : public TCommands<FSpawnQueryEditorCommands>
{
public:

	FSpawnQueryEditorCommands()
		: TCommands<FSpawnQueryEditorCommands>(TEXT("SpawnQuerySystem"), NSLOCTEXT("Contexts", "SpawnQuerySystem", "SpawnQuerySystem Plugin"), NAME_None, FSpawnQueryEditorStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > OpenPluginWindow;
};