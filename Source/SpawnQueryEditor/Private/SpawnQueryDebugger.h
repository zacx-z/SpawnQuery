#pragma once

#include "SpawnQuery.h"
#include "SpawnQueryEditor.h"
#include "Tickable.h"

class USpawnQueryGraphNode_Root;
class USpawnQueryGraphNode;
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
    void Refresh();

    void OnBeginPIE(const bool IsSimulating);
    void OnEndPIE(const bool IsSimulating);
    void OnPausePIE(const bool IsSimulating);

    static bool IsPlaySessionPaused();
    static bool IsPlaySessionRunning();
    static bool IsPIESimulating();
    static void ForEachGameWorld(const TFunction<void(UWorld*)>& Func);
    static bool AreAllGameWorldPaused();

    TWeakObjectPtr<USpawnQueryContext> GetCurrentDebugContext()
    {
        return CurrentDebugContext;
    }
    void SetCurrentDebugContext(TWeakObjectPtr<USpawnQueryContext> InContext);

private:

    void CacheRootNode();
    void UpdateAssetFlags(const USpawnQueryContext& Context, USpawnQueryGraphNode* Node);

    TWeakObjectPtr<USpawnQueryContext> CurrentDebugContext;
    TWeakObjectPtr<USpawnQuery> SpawnQueryAsset;
    TWeakObjectPtr<USpawnQueryGraphNode_Root> RootNode;
};
