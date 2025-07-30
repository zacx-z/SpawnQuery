#include "SpawnQuery/Composites/SpawnQueryComposite_RandomSelector.h"

#include "SpawnQuery/SpawnQueryContext.h"

TObjectPtr<USpawnEntryBase> USpawnQueryComposite_RandomSelector::Query(USpawnQueryContext& Context)
{
    int32 ChildrenNum = Children.Num();

    double TotalWeights = 0;

    for (int32 Index = 0; Index < ChildrenNum; ++Index)
    {
        TObjectPtr<USpawnQueryNode> ChildNode = Children[Index].ChildNode;
        if (ChildNode->IsSubtreeActive(Context))
        {
            TotalWeights += ChildNode->GetWeight(Context);
        }
    }

    if (TotalWeights == 0)
    {
        return nullptr;
    }

    double WeightPosition = Context.GetRandomStream().FRandRange(0, TotalWeights);

    for (int32 Index = 0; Index < ChildrenNum; ++Index)
    {
        TObjectPtr<USpawnQueryNode> ChildNode = Children[Index].ChildNode;
        if (ChildNode->IsSubtreeActive(Context))
        {
            WeightPosition -= ChildNode->GetWeight(Context);
            if (WeightPosition <= FLT_EPSILON)
            {
                return Children[Index].ChildNode->Query(Context);
            }
        }
    }

    // we should never arrive here
    check(0);

    return nullptr;
}
