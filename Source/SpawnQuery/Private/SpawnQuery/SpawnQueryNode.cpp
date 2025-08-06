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
    return SpawnQueryTypes::GetShortTypeName(this);
}

FText USpawnQueryNode::GetDescriptionDetails() const
{
    return FText::GetEmpty();
}

float USpawnQueryNode::GetWeight(const USpawnQueryContext& Context)
{
    float Weight = 1.0f;

    for (TObjectPtr<USpawnQueryNode_Decorator> Decorator : Decorators)
    {
        Weight = Decorator->MutateWeight(Weight, Context);
    }

    return Weight;
}

FString USpawnQueryNode::GetErrorMessage() const
{
    return FString();
}

bool USpawnQueryNode::IsSubtreeActive(const USpawnQueryContext& Context)
{
    return IsActive(Context) && CheckDecoratorsActive(Context);
}

TObjectPtr<USpawnEntryBase> USpawnQueryNode::GetQueryResult(USpawnQueryContext& Context)
{
    TObjectPtr<USpawnEntryBase> Entry = Query(Context);
    for (USpawnQueryNode_Decorator* Decorator : Decorators)
    {
        Entry = Decorator->Rewrite(Entry, Context);
    }
    return Entry;
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
