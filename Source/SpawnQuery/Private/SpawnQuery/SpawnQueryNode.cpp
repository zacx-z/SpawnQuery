#include "SpawnQuery/SpawnQueryNode.h"
#include "UObject/UnrealType.h"
#include "SpawnQueryTypes.h"
#include "SpawnQuery/SpawnQueryNode_Decorator.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(SpawnQueryNode)

USpawnQueryNode::USpawnQueryNode(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

FText USpawnQueryNode::GetDescriptionTitle() const
{
    return USpawnQueryTypes::GetShortTypeName(this);
}

FText USpawnQueryNode::GetDescriptionDetails() const
{
    return FText::GetEmpty();
}

#if WITH_EDITOR
void USpawnQueryNode::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
}

double USpawnQueryNode::GetWeight(const USpawnQueryContext& Context)
{
    double Weight = 1.0;

    for (TObjectPtr<USpawnQueryNode_Decorator> Decorator : Decorators)
    {
        Weight = Decorator->MutateWeight(Weight, Context);
    }

    return Weight;
}

bool USpawnQueryNode::IsSubtreeActive(const USpawnQueryContext& Context)
{
    return IsActive(Context) && CheckDecoratorsActive(Context);
}

bool USpawnQueryNode::CheckDecoratorsActive(const USpawnQueryContext& Context)
{
    for (auto Decorator : Decorators)
    {
        if (!Decorator->IsActive(Context))
            return false;
    }

    return true;
}

#endif //WITH_EDITOR
