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
                    TSoftClassPtr<AActor> ActorClassAsset = Row->ActorClass;
                    if (ActorClassAsset.IsValid())
                    {
                        GetWorld()->SpawnActor<AActor>(ActorClassAsset.Get(), RandomLocation, Rotation);
                    }
                    else
                    {
                        ActorClassAsset.LoadAsync(FLoadSoftObjectPathAsyncDelegate::CreateLambda([&](const FSoftObjectPath& Path, UObject* Class)
                        {
                            GetWorld()->SpawnActor<AActor>(static_cast<UClass*>(Class), RandomLocation, Rotation);
                        }));
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
