#include "SpawnQueryGraphNode_Composite.h"

#include "SpawnQueryGraph.h"
#include "SpawnQuery/SpawnQueryNode_Composite.h"
#include "HAL/PlatformMath.h"
#include "Internationalization/Internationalization.h"
#include "Templates/Casts.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(SpawnQueryGraphNode_Composite)

class UToolMenu;

USpawnQueryGraphNode_Composite::USpawnQueryGraphNode_Composite(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
}

FText USpawnQueryGraphNode_Composite::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
    if (const USpawnQueryNode* MyNode = Cast<USpawnQueryNode>(NodeInstance))
    {
        return MyNode->GetDescriptionTitle();
    }
    return Super::GetNodeTitle(TitleType);
}

FText USpawnQueryGraphNode_Composite::GetDescription() const
{
    if (const USpawnQueryNode* MyNode = Cast<USpawnQueryNode>(NodeInstance))
    {
        return MyNode->GetDescriptionDetails();
    }

    return Super::GetDescription();
}

FText USpawnQueryGraphNode_Composite::GetTooltipText() const
{
    return Super::GetTooltipText();
}

void USpawnQueryGraphNode_Composite::GetNodeContextMenuActions(UToolMenu* Menu, UGraphNodeContextMenuContext* Context) const
{
    Super::GetNodeContextMenuActions(Menu, Context);
}
