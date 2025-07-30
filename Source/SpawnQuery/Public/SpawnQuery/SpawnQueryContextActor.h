#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnQueryContextActor.generated.h"

class UBlackboardComponent;

/**
 * The actor holding necessary components referenced by a context object
 */
UCLASS()
class SPAWNQUERY_API ASpawnQueryContextActor : public AActor
{
    GENERATED_BODY()

public:

    ASpawnQueryContextActor();

    UBlackboardComponent* GetBlackboardComponent() const { return BlackboardComponent; }

private:

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Blackboard", meta = (AllowPrivateAccess = "true"))
    UBlackboardComponent* BlackboardComponent;
};
