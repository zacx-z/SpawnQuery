#include "SpawnQuery/Composites/SpawnQueryComposite_PrioritySelector.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(SpawnQueryComposite_PrioritySelector)

TObjectPtr<USpawnEntryBase> USpawnQueryComposite_PrioritySelector::Query(
    USpawnQueryContext& context)
{
    TObjectPtr<USpawnEntryBase> Entry = nullptr;

    int32 ChildrenNum = Children.Num();

    USpawnQueryNode* ActiveChild = nullptr;
    if (!bReverseDirection)
    {
        for (int32 Index = 0; Index < ChildrenNum; ++Index)
        {
            FSpawnQueryCompositeChild Child = Children[Index];
            if (Child.ChildNode->IsActive(context))
            {
                ActiveChild = Child.ChildNode;
                break;
            }
        }
    } else
    {
        for (int32 Index = ChildrenNum - 1; Index >= 0; --Index)
        {
            FSpawnQueryCompositeChild Child = Children[Index];
            if (Child.ChildNode->IsActive(context))
            {
                ActiveChild = Child.ChildNode;
                break;
            }
        }
    }

    if (ActiveChild)
    {
        Entry = ActiveChild->Query(context);
    }
    else
    {
        UE_LOG(LogBlueprint, Warning, TEXT("No active children available in USpawnQueryComposite_PrioritySelector::Query"));
    }

    if (Entry) return Entry;

    UE_LOG(LogBlueprint, Warning, TEXT("Active child returns no entry in USpawnQueryComposite_PrioritySelector::Query"));
    return Super::Query(context);
}
