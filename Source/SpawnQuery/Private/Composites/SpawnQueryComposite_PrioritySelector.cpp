#include "SpawnQuery/Composites/SpawnQueryComposite_PrioritySelector.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(SpawnQueryComposite_PrioritySelector)

USpawnQueryComposite_PrioritySelector::USpawnQueryComposite_PrioritySelector(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

bool USpawnQueryComposite_PrioritySelector::IsActive(USpawnQueryContext& context)
{
    for (auto Child : Children)
    {
        if (Child.ChildNode->IsActive(context))
        {
            return true;
        }
    }
    
    return false;
}

TObjectPtr<USpawnEntryBase> USpawnQueryComposite_PrioritySelector::Query(
    USpawnQueryContext& context)
{
    TObjectPtr<USpawnEntryBase> Entry = nullptr;

    for (auto Child : Children)
    {
        if (Child.ChildNode->IsActive(context))
        {
            Entry = Child.ChildNode->Query(context);
            break;
        }
    }

    if (Entry) return Entry;

    UE_LOG(LogBlueprint, Warning, TEXT("No active children available in USpawnQueryComposite_PrioritySelector::Query"));
    
    return Super::Query(context);
}
