#include "SpawnQuery/Composites/SpawnQueryComposite_PrioritySelector.h"

#include "SpawnQuery/SpawnQueryContext.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(SpawnQueryComposite_PrioritySelector)

namespace PrioritySelectorHelper
{
    USpawnQueryNode* GetActiveChild(USpawnQueryContext& Context
        , USpawnQueryComposite_PrioritySelector_State* State
        , TArray<FSpawnQueryCompositeChild>& Children)
    {
        auto Priorities = State->ChildOrder;
        int32 ChildrenCount = Priorities.Num();
        check(Children.Num() == ChildrenCount);

        for (int Index = 0; Index < ChildrenCount; Index++)
        {
            int16 ChildIndex = Priorities[Index];
            USpawnQueryNode* Child = Children[ChildIndex].ChildNode;
            if (Child->IsSubtreeActive(Context))
            {
                // move the child to the back
                for (int K = Index + 1; K < ChildrenCount; K++)
                {
                    Priorities[K - 1] = Priorities[K];
                }

                Priorities[ChildrenCount - 1] = ChildIndex;

                return Child;
            }
        }

        return nullptr;
    }
}

TObjectPtr<USpawnEntryBase> USpawnQueryComposite_PrioritySelector::Query(
    USpawnQueryContext& Context)
{
    TObjectPtr<USpawnEntryBase> Entry = nullptr;

    int32 ChildrenNum = Children.Num();

    USpawnQueryNode* ActiveChild = nullptr;

    if (bDynamic)
    {
        BuildPriorityStates(Context);
        ActiveChild = PrioritySelectorHelper::GetActiveChild(Context
            , Context.GetStateObject<USpawnQueryComposite_PrioritySelector_State>(this)
            , Children);
    }
    else
    {
        if (!bReverseDirection)
        {
            for (int32 Index = 0; Index < ChildrenNum; ++Index)
            {
                FSpawnQueryCompositeChild Child = Children[Index];
                if (Child.ChildNode->IsSubtreeActive(Context))
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
                if (Child.ChildNode->IsSubtreeActive(Context))
                {
                    ActiveChild = Child.ChildNode;
                    break;
                }
            }
        }
    }

    if (ActiveChild)
    {
        Entry = ActiveChild->GetQueryResult(Context);
    }
    else
    {
        UE_LOG(LogBlueprint, Warning, TEXT("No active children available in USpawnQueryComposite_PrioritySelector::Query"));
    }

    if (Entry) return Entry;

    UE_LOG(LogBlueprint, Warning, TEXT("Active child returns no entry in USpawnQueryComposite_PrioritySelector::Query"));
    return nullptr;
}

void USpawnQueryComposite_PrioritySelector::BuildPriorityStates(USpawnQueryContext& Context)
{
    if (Children.Num() == 0) return;

    auto State = Context.GetStateObject<USpawnQueryComposite_PrioritySelector_State>(this);

    if (State->ChildOrder.Num() == 0)
    {
        int16 ChildrenCount = Children.Num();
        for (int16 Index = 0; Index < ChildrenCount; Index++)
        {
            int16 Priority = bReverseDirection ? ChildrenCount - Index - 1 : Index;
            State->ChildOrder.Add(Priority);
        }
    }
}
