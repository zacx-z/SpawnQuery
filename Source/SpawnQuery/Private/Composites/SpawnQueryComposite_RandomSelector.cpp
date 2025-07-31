#include "SpawnQuery/Composites/SpawnQueryComposite_RandomSelector.h"

#include "SpawnQuery/SpawnQueryContext.h"
#include "SpawnQuery/RandomizationHelpers.h"

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
