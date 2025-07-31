#include "SpawnQuery/Composites/SpawnQueryComposite_RandomSelector.h"

#include "SpawnQuery/SpawnQueryContext.h"

namespace RandomizationHelpers
{
    int32 Randomize_Independent(TArray<FSpawnQueryCompositeChild>& Children, TArray<FRandomizationWeightState>& WeightCache, FRandomStream& RandomStream, USpawnQueryContext& Context)
    {
        int32 ChildrenNum = Children.Num();
        
        float TotalWeights = 0;

        for (int32 Index = 0; Index < ChildrenNum; ++Index)
        {
            TObjectPtr<USpawnQueryNode> ChildNode = Children[Index].ChildNode;

            if (Index >= WeightCache.Num()) WeightCache.Add(FRandomizationWeightState());

            if (ChildNode->IsSubtreeActive(Context))
            {
                WeightCache[Index].CachedActive = true;
                float Weight = ChildNode->GetWeight(Context);
                WeightCache[Index].CachedWeight = Weight;
                TotalWeights += Weight;
            }
            else
            {
                WeightCache[Index].CachedActive = false;
                WeightCache[Index].CachedWeight = 0;
            }
        }

        if (TotalWeights == 0)
        {
            return -1;
        }

        float WeightPosition = RandomStream.FRandRange(0, TotalWeights);

        for (int32 Index = 0; Index < ChildrenNum; ++Index)
        {
            WeightPosition -= WeightCache[Index].CachedWeight;
            if (WeightPosition <= FLT_EPSILON)
            {
                return Index;
            }
        }

        // we should never arrive here
        check(0)
        return -1;
    }

    int32 Randomize_ShuffledSequence(TArray<FSpawnQueryCompositeChild>& Children, TArray<FRandomizationWeightState>& WeightBase, FRandomStream& RandomStream, USpawnQueryContext& Context)
    {
        int32 ChildrenNum = Children.Num();
        
        float TotalWeights = 0;
        int32 BaseAddMultiplier = MAX_int32;

        for (int32 Index = 0; Index < ChildrenNum; ++Index)
        {
            TObjectPtr<USpawnQueryNode> ChildNode = Children[Index].ChildNode;

            if (Index >= WeightBase.Num()) WeightBase.Add(FRandomizationWeightState());

            float OriginalChildWeight = ChildNode->GetWeight(Context);
            WeightBase[Index].CachedWeight = OriginalChildWeight; // no matter whether the child node is active, we use the weight value to accumulate the base value

            WeightBase[Index].CachedActive = false;

            if (ChildNode->IsSubtreeActive(Context))
            {
                if (OriginalChildWeight <= 0) continue;
                WeightBase[Index].CachedActive = true;

                float ChildWeight = OriginalChildWeight + WeightBase[Index].Base;

                // only when weight >= 1 the child will be counted into the sequence (deck)
                if (ChildWeight >= 1 - FLT_EPSILON)
                {
                    TotalWeights += FMath::FloorToInt(ChildWeight + FLT_EPSILON);
                    BaseAddMultiplier = 0;
                }
                else if (BaseAddMultiplier > 0)
                {
                    int32 TargetBaseAddMultiplier = FMath::CeilToInt((1 - ChildWeight) / OriginalChildWeight);
                    if (TargetBaseAddMultiplier < BaseAddMultiplier)
                    {
                        BaseAddMultiplier = TargetBaseAddMultiplier;
                    }
                }
            }
        }

        if (TotalWeights <= 0)
        {
            if (BaseAddMultiplier == MAX_int32)
            {
                // all children have 0 weight
                return -1;
            }

            TotalWeights = 0;

            for (int32 Index = 0; Index < ChildrenNum; ++Index)
            {
                if (WeightBase[Index].CachedWeight > 0)
                {
                    float Addend = WeightBase[Index].CachedWeight * BaseAddMultiplier;
                    WeightBase[Index].Base += Addend;

                    if (WeightBase[Index].CachedActive)
                    {
                        TotalWeights += FMath::FloorToInt(WeightBase[Index].CachedWeight + WeightBase[Index].Base + FLT_EPSILON);
                    }
                }
            }
        }

        float WeightPosition = RandomStream.FRandRange(0, TotalWeights);

        for (int32 Index = 0; Index < ChildrenNum; ++Index)
        {
            if (WeightBase[Index].CachedActive)
            {
                WeightPosition -= FMath::FloorToInt(WeightBase[Index].CachedWeight + WeightBase[Index].Base + FLT_EPSILON * 2);
                if (WeightPosition <= FLT_EPSILON)
                {
                    WeightBase[Index].Base -= 1;
                    return Index;
                }
            }
        }

        // we should never arrive here
        check(0)
        return -1;
    }
}

TObjectPtr<USpawnEntryBase> USpawnQueryComposite_RandomSelector::Query(USpawnQueryContext& Context)
{
    TArray<FRandomizationWeightState>& WeightBase = Context.GetStateObject<
        USpawnQueryComposite_RandomSelector_State>(this)->WeightBase;
    int32 Picked =
        RandomizationPolicy == Independent
    ? RandomizationHelpers::Randomize_Independent(Children, WeightBase, Context.GetRandomStream(), Context)
    : RandomizationHelpers::Randomize_ShuffledSequence(Children, WeightBase, Context.GetRandomStream(), Context);
    

    if (Picked == -1)
    {
        return nullptr;
    }

    return Children[Picked].ChildNode->Query(Context);
}
