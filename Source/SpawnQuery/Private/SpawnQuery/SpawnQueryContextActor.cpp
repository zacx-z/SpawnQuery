#include "SpawnQuery/SpawnQueryContextActor.h"

#include "BehaviorTree/BlackboardComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(SpawnQueryContextActor)

ASpawnQueryContextActor::ASpawnQueryContextActor() : BlackboardComponent(CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent")))
{
    PrimaryActorTick.bCanEverTick = false;
}
