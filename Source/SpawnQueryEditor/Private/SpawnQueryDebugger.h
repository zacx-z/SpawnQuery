#pragma once

#include "SpawnQuery.h"
#include "SpawnQueryEditor.h"
#include "Tickable.h"

class FSpawnQueryEditor;

class FSpawnQueryDebugger : public FTickableGameObject
{
public:
    FSpawnQueryDebugger();
    ~FSpawnQueryDebugger();

    //~ Begin FTickableGameObject interface
    virtual void Tick(float DeltaTime) override;
    virtual TStatId GetStatId() const override { RETURN_QUICK_DECLARE_CYCLE_STAT(FSpawnQueryEditorTickHelper, STATGROUP_Tickables); }
    //~ End FTickableGameObject interface

    void Setup(USpawnQuery* InQueryAsset, TSharedRef<class FSpawnQueryEditor> InEditorOwner);

    static bool IsPlaySessionPaused();
    static bool IsPlaySessionRunning();
    static void ForEachGameWorld(const TFunction<void(UWorld*)>& Func);
    static bool AreAllGameWorldPaused();

    TWeakObjectPtr<USpawnQueryContext> GetCurrentDebugContext()
    {
        return CurrentDebugContext;
    }
    void SetCurrentDebugContext(TWeakObjectPtr<USpawnQueryContext> InContext)
    {
        CurrentDebugContext = InContext;
    }

private:
    TWeakObjectPtr<USpawnQueryContext> CurrentDebugContext;
};
