#pragma once
#include "CoreMinimal.h"
#include "SpawnQuery/Pools/SpawnEntryTableRow.h"
#include "SpawnScatter.generated.h"

class USpawnQuery;
class USpawnQueryContext;

UCLASS(Blueprintable, BlueprintType)
class ASpawnScatter : public AActor
{
    GENERATED_BODY()

public:
    virtual void BeginPlay() override;

    UFUNCTION(BlueprintNativeEvent)
    USpawnQueryContext* GetContext();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spawn Scatter")
    USpawnQuery* SpawnQuery;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spawn Scatter")
    int32 Amount = 1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spawn Scatter")
    float ScatterRange;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spawn Scatter")
    bool bRandomizeRotation;

private:

    TWeakObjectPtr<USpawnQueryContext> CachedGlobalContext;
};

USTRUCT(BlueprintType)
struct FSpawnEntryTableRow_Actor : public FSpawnEntryTableRowBase
{
    GENERATED_USTRUCT_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSoftClassPtr<AActor> ActorClass;
};
