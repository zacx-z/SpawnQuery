#include "SpawnQuery/SpawnQueryNode_Sampler.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(SpawnQueryNode_Sampler)

USpawnQueryNode_Sampler::USpawnQueryNode_Sampler(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

FText USpawnQueryNode_Sampler::GetDescriptionTitle() const
{
    return FText::FromString(Super::GetDescriptionTitle().ToString() + " Sampler");
}
