#include "SpawnScatterComponentVisualizer.h"

#include "SpawnQuery/Actors/SpawnScatterComponent.h"

void FSpawnScatterComponentVisualizer::DrawVisualization(const UActorComponent* Component, const FSceneView* View,
                                                         FPrimitiveDrawInterface* PDI)
{
    const USpawnScatterComponent* Comp = Cast<USpawnScatterComponent>(Component);
    if (Comp == nullptr) return;

    const FVector Location = Comp->GetComponentLocation();
    const float Radius = Comp->ScatterRange;

    DrawWireSphere(PDI, Location, FLinearColor::Green, Radius, 20, 0);
}
