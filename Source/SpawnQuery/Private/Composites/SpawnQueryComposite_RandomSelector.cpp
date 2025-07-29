#include "SpawnQuery/Composites/SpawnQueryComposite_RandomSelector.h"

#include "SpawnQuery/SpawnQueryContext.h"

TObjectPtr<USpawnEntryBase> USpawnQueryComposite_RandomSelector::Query(USpawnQueryContext& Context)
{
    int32 ChildrenNum = Children.Num();

    double TotalWeights = 0;

    for (int32 Index = 0; Index < ChildrenNum; ++Index)
    {
        if (Children[Index].ChildNode->IsActive(Context))
        {
            TotalWeights += 1; // weight defaults to 1 for now
        }
    }

    if (TotalWeights == 0)
    {
        return nullptr;
    }

    double WeightPosition = Context.GetRandomStream().FRandRange(0, TotalWeights);

    for (int32 Index = 0; Index < ChildrenNum; ++Index)
    {
        if (Children[Index].ChildNode->IsActive(Context))
        {
            WeightPosition -= 1; // weight defaults to 1 for now
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
