#include "SpawnQueryDebugger.h"

#include "SpawnQueryModule.h"
#include "UnrealEdGlobals.h"
#include "Editor/UnrealEdEngine.h"

FSpawnQueryDebugger::FSpawnQueryDebugger()
{
}

FSpawnQueryDebugger::~FSpawnQueryDebugger()
{
}

void FSpawnQueryDebugger::Tick(float DeltaTime)
{
}

void FSpawnQueryDebugger::Setup(USpawnQuery* InQueryAsset, TSharedRef<class FSpawnQueryEditor> InEditorOwner)
{
    FSpawnQueryModule& SpawnQueryModule = FModuleManager::LoadModuleChecked<FSpawnQueryModule>("SpawnQuery");
    CurrentDebugContext = SpawnQueryModule.GetDefaultContext();
}

bool FSpawnQueryDebugger::IsPlaySessionPaused()
{
    return AreAllGameWorldPaused();
}

bool FSpawnQueryDebugger::IsPlaySessionRunning()
{
    return !AreAllGameWorldPaused();
}

void FSpawnQueryDebugger::ForEachGameWorld(const TFunction<void(UWorld*)>& Func)
{
    for (const FWorldContext& PieContext : GUnrealEd->GetWorldContexts())
    {
        UWorld* PlayWorld = PieContext.World();
        if (PlayWorld && PlayWorld->IsGameWorld())
        {
            Func(PlayWorld);
        }
    }
}

bool FSpawnQueryDebugger::AreAllGameWorldPaused()
{
    bool bPaused = true;
    ForEachGameWorld([&](UWorld* World)
    {
        bPaused = bPaused && World->bDebugPauseExecution; 
    });
    return bPaused;
}
