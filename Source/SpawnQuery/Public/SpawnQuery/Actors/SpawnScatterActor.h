#pragma once
#include "CoreMinimal.h"
#include "SpawnScatterComponent.h"
#include "SpawnScatterActor.generated.h"

class USpawnQuery;
class USpawnQueryContext;

UCLASS(Blueprintable, BlueprintType, DisplayName="Spawn Scatter")
class ASpawnScatterActor : public AActor
{
    GENERATED_UCLASS_BODY()

public:
    virtual void BeginPlay() override;

    UFUNCTION(BlueprintNativeEvent)
    USpawnQueryContext* GetContext();

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Spawn Scatter")
    TObjectPtr<USpawnScatterComponent> SpawnScatterComponent;

private:
    UPROPERTY()
    TWeakObjectPtr<USpawnQueryContext> CachedGlobalContext;
};
