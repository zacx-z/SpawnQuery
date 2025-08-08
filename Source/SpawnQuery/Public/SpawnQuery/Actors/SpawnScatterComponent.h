#pragma once
#include "CoreMinimal.h"
#include "SpawnQuery/Pools/SpawnEntryTableRow.h"
#include "SpawnScatterComponent.generated.h"

class USpawnQueryContext;
class USpawnQuery;

UCLASS(BlueprintType)
class USpawnScatterComponent : public USceneComponent
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable)
    SPAWNQUERY_API void SpawnActors(USpawnQueryContext* Context);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spawn Scatter")
    USpawnQuery* SpawnQuery;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spawn Scatter")
    int32 Amount = 1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spawn Scatter")
    float ScatterRange = 1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spawn Scatter")
    bool bRandomizeRotation = true;
};

USTRUCT(BlueprintType)
struct FSpawnEntryTableRow_Actor : public FSpawnEntryTableRowBase
{
    GENERATED_USTRUCT_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSoftClassPtr<AActor> ActorClass;
};
