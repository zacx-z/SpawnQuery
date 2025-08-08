#include "SpawnQuery/Actors/SpawnScatterActor.h"

#include "SpawnQuery.h"
#include "SpawnQueryModule.h"
#include "SpawnQuery/Pools/SpawnEntryTableRow.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "SpawnQuery/Actors/SpawnScatterComponent.h"

ASpawnScatterActor::ASpawnScatterActor(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    SpawnScatterComponent = CreateDefaultSubobject<USpawnScatterComponent>("Scatter");
    SpawnScatterComponent->Mobility = EComponentMobility::Static;

    RootComponent = SpawnScatterComponent;
}

void ASpawnScatterActor::BeginPlay()
{
    Super::BeginPlay();

    SpawnScatterComponent->SpawnActors(GetContext());
}

USpawnQueryContext* ASpawnScatterActor::GetContext_Implementation()
{
    if (CachedGlobalContext == nullptr)
    {
        FSpawnQueryModule& SpawnQueryEditorModule = FModuleManager::LoadModuleChecked<FSpawnQueryModule>("SpawnQuery");
        CachedGlobalContext = SpawnQueryEditorModule.GetDefaultContext();
    }

    return CachedGlobalContext.Get();
}
