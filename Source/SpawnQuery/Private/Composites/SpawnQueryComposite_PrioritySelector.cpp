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

TSharedPtr<ISpawnEntryBase> USpawnQueryComposite_PrioritySelector::Query(
	USpawnQueryContext& context)
{
	TSharedPtr<ISpawnEntryBase> Entry = nullptr;

	for (auto Child : Children)
	{
		if (Child.ChildNode->IsActive(context))
		{
			Entry = Child.ChildNode->Query(context);
			break;
		}
	}

	if (Entry) return Entry;

	return Super::Query(context);
}
