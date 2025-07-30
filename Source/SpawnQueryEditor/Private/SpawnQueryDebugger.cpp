#include "SpawnQueryDebugger.h"

#include "SpawnQueryGraphNode.h"
#include "SpawnQueryGraphNode_Root.h"
#include "SpawnQueryModule.h"
#include "UnrealEdGlobals.h"
#include "Editor/UnrealEdEngine.h"
#include "SpawnQuery/SpawnQueryNode.h"

FSpawnQueryDebugger::FSpawnQueryDebugger()
    : SpawnQueryAsset(nullptr)
{
    // must be after PIE started so that World will be available to create the operating Actor
    FEditorDelegates::PostPIEStarted.AddRaw(this, &FSpawnQueryDebugger::OnBeginPIE);
    FEditorDelegates::EndPIE.AddRaw(this, &FSpawnQueryDebugger::OnEndPIE);
    FEditorDelegates::PausePIE.AddRaw(this, &FSpawnQueryDebugger::OnPausePIE);
}

FSpawnQueryDebugger::~FSpawnQueryDebugger()
{
    FEditorDelegates::PostPIEStarted.RemoveAll(this);
    FEditorDelegates::EndPIE.RemoveAll(this);
    FEditorDelegates::PausePIE.RemoveAll(this);
}

void FSpawnQueryDebugger::Tick(float DeltaTime)
{
    if (IsPlaySessionPaused())
    {
        USpawnQueryContext* ContextPtr = CurrentDebugContext.Get();
        if (ContextPtr)
        {
            UpdateAssetFlags(*ContextPtr, RootNode.Get());
        }
    }
}

void FSpawnQueryDebugger::Setup(USpawnQuery* InQueryAsset, TSharedRef<FSpawnQueryEditor> InEditorOwner)
{
    check(InQueryAsset)

    SpawnQueryAsset = InQueryAsset;

    FSpawnQueryModule& SpawnQueryModule = FModuleManager::LoadModuleChecked<FSpawnQueryModule>("SpawnQuery");
    CurrentDebugContext = SpawnQueryModule.GetDefaultContext();

    CacheRootNode();

    if (IsPIESimulating())
    {
        OnBeginPIE(GEditor->bIsSimulatingInEditor);

        Refresh();
    }
}

void FSpawnQueryDebugger::Refresh()
{
    CacheRootNode();

    if (IsPIESimulating())
    {
        if (USpawnQueryContext* ContextPtr = CurrentDebugContext.Get())
        {
            UpdateAssetFlags(*ContextPtr, RootNode.Get());
        }
    }
}

void FSpawnQueryDebugger::OnBeginPIE(const bool IsSimulating)
{
    if (USpawnQueryContext* ContextPtr = CurrentDebugContext.Get())
    {
        UpdateAssetFlags(*ContextPtr, RootNode.Get());
    }
}

void FSpawnQueryDebugger::OnEndPIE(const bool IsSimulating)
{
    Refresh();
}

void FSpawnQueryDebugger::OnPausePIE(const bool IsSimulating)
{
}

bool FSpawnQueryDebugger::IsPlaySessionPaused()
{
    return AreAllGameWorldPaused();
}

bool FSpawnQueryDebugger::IsPlaySessionRunning()
{
    return !AreAllGameWorldPaused();
}

bool FSpawnQueryDebugger::IsPIESimulating()
{
    return GEditor->bIsSimulatingInEditor || GEditor->PlayWorld;
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

void FSpawnQueryDebugger::CacheRootNode()
{
    if (RootNode.IsValid()) return;

    if(SpawnQueryAsset == nullptr || SpawnQueryAsset->EdGraph == nullptr)
    {
        return;
    }

    for (const auto& Node : SpawnQueryAsset->EdGraph->Nodes)
    {
        RootNode = Cast<USpawnQueryGraphNode_Root>(Node);
        if (RootNode.IsValid())
        {
            break;
        }
    }
}

void FSpawnQueryDebugger::UpdateAssetFlags(const USpawnQueryContext& Context, USpawnQueryGraphNode* Node)
{
    if (Node == nullptr) return;

    if (USpawnQueryNode* NodeInstance = Cast<USpawnQueryNode>(Node->NodeInstance))
    {
        Node->bDebuggerActiveState = NodeInstance->IsActive(Context);
        NodeInstance->Refresh();
    }
    else if (Node->IsA(USpawnQueryGraphNode_Root::StaticClass()) && SpawnQueryAsset != nullptr)
    {
        Node->bDebuggerActiveState = SpawnQueryAsset->IsActive(Context);
    }

    for (UEdGraphNode* SubNode : Node->SubNodes)
    {
        if (USpawnQueryGraphNode* LinkedNode = Cast<USpawnQueryGraphNode>(SubNode))
        {
            UpdateAssetFlags(Context, LinkedNode);
        }
    }

    for (UEdGraphPin* Pin : Node->Pins)
    {
        if (Pin->Direction != EGPD_Output)
        {
            continue;
        }

        for (UEdGraphPin* LinkedPin : Pin->LinkedTo)
        {
            if (USpawnQueryGraphNode* LinkedNode = Cast<USpawnQueryGraphNode>(LinkedPin->GetOwningNode()))
            {
                UpdateAssetFlags(Context, LinkedNode);
            }
        }
    }
}
