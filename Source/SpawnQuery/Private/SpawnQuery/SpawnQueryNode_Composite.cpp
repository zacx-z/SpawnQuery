#include "SpawnQuery/SpawnQueryNode_Composite.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(SpawnQueryNode_Composite)

USpawnQueryNode_Composite::USpawnQueryNode_Composite(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

bool USpawnQueryNode_Composite::IsActive(USpawnQueryContext& Context)
{
    for (FSpawnQueryCompositeChild Child : Children)
    {
        if (Child.ChildNode->IsActive(Context))
        {
            return true;
        }
    }
    
    return false;
}
