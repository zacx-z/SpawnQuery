// Copyright Epic Games, Inc. All Rights Reserved.

#include "SpawnQueryModule.h"

#include "SpawnQuery/SpawnQueryContext.h"

#define LOCTEXT_NAMESPACE "FSpawnQuerySystemModule"

void FSpawnQueryModule::StartupModule()
{
    // This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
    SpawnQueryContext = NewObject<USpawnQueryContext>(GetTransientPackage(), TEXT("Default Spawn Query Context"));
    SpawnQueryContext->AddToRoot();

    AllSpawnQueryContexts.Add(SpawnQueryContext);
}

void FSpawnQueryModule::ShutdownModule()
{
    // This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
    // we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FSpawnQueryModule, SpawnQuery)