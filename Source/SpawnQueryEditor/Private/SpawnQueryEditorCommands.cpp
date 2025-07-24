// Copyright Epic Games, Inc. All Rights Reserved.

#include "SpawnQueryEditorCommands.h"

#define LOCTEXT_NAMESPACE "FSpawnQueryEditorModule"

void FSpawnQueryEditorCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "SpawnQuerySystem", "Bring up SpawnQuerySystem window", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
