#include "SpawnQuery/SpawnQueryNode.h"
#include "UObject/UnrealType.h"
#include "DataProviders/AIDataProvider.h"
#include "DataProviders/AIDataProvider_QueryParams.h"
#include "SpawnQueryTypes.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(SpawnQueryNode)

USpawnQueryNode::USpawnQueryNode(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

FText USpawnQueryNode::GetDescriptionTitle() const
{
	return USpawnQueryTypes::GetShortTypeName(this);
}

FText USpawnQueryNode::GetDescriptionDetails() const
{
	return FText::GetEmpty();
}

#if WITH_EDITOR
void USpawnQueryNode::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
}
#endif //WITH_EDITOR
