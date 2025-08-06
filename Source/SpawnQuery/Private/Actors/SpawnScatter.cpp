#include "SpawnQuery/Actors/SpawnScatter.h"

#include "SpawnQuery.h"
#include "SpawnQueryModule.h"
#include "SpawnQuery/Pools/SpawnEntryTableRow.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"

void ASpawnScatter::BeginPlay()
{
    Super::BeginPlay();

    for (int Index = 0; Index < Amount; Index++)
    {
        USpawnEntryBase* Entry = SpawnQuery->QueryEntry(GetContext());
        if (Entry)
        {
            if (auto RowHandle = Cast<USpawnEntryRowHandle>(Entry))
            {
                if (auto Row = RowHandle->GetTableRow<FSpawnEntryTableRow_Actor>())
                {
                    // Generate a random location within the ScatterRange radius
                    float Angle = FMath::FRand() * 2.0f * PI;
                    float Radius = FMath::Sqrt(FMath::FRand()) * ScatterRange; // Using Sqrt to ensure uniform distribution
                    FVector RandomLocation = GetActorLocation() + FVector(Radius * FMath::Cos(Angle), Radius * FMath::Sin(Angle), 0.0f);

                    // Generate a rotation for the Z-axis
                    FRotator Rotation = FRotator();
                    if (bRandomizeRotation)
                        Rotation = FRotator(0.0f, FMath::FRand() * 360.0f, 0.0f);

                    // Spawn the actor at the generated location and rotation
                    FActorSpawnParameters SpawnParams;
                    TSoftClassPtr<AActor> ActorClassAsset = Row->ActorClass;
                    if (UClass* ActorClass = ActorClassAsset.LoadSynchronous())
                    {
                        GetWorld()->SpawnActor<AActor>(ActorClass, RandomLocation, Rotation, SpawnParams);
                    }
                }
            }
        }
    }
}

USpawnQueryContext* ASpawnScatter::GetContext_Implementation()
{
    if (CachedGlobalContext == nullptr)
    {
        FSpawnQueryModule& SpawnQueryEditorModule = FModuleManager::LoadModuleChecked<FSpawnQueryModule>("SpawnQuery");
        CachedGlobalContext = SpawnQueryEditorModule.GetDefaultContext();
    }

    return CachedGlobalContext.Get();
}
