#include "ComponentVisualizerEditor.h"

#include "UnrealEdGlobals.h"
#include "Editor/UnrealEdEngine.h"
#include "SpawnQuery/Actors/SpawnScatterComponent.h"
#include "Visualizer/SpawnScatterComponentVisualizer.h"

#define LOCTEXT_NAMESPACE "FComponentVisualizerEditorModule"

void FComponentVisualizerEditorModule::StartupModule()
{
    if (GUnrealEd)
    {
        TSharedPtr<FSpawnScatterComponentVisualizer> SpawnScatterVisualizer = MakeShareable(new FSpawnScatterComponentVisualizer);
        GUnrealEd->RegisterComponentVisualizer(USpawnScatterComponent::StaticClass()->GetFName(), SpawnScatterVisualizer);
    }
}

void FComponentVisualizerEditorModule::ShutdownModule()
{
    if (GUnrealEd)
    {
        GUnrealEd->UnregisterComponentVisualizer(USpawnScatterComponent::StaticClass()->GetFName());
    }
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FComponentVisualizerEditorModule, ComponentVisualizerEditor)