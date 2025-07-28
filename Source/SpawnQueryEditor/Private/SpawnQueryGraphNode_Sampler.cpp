#include "SpawnQueryGraphNode_Sampler.h"

#include "SpawnQueryColors.h"
#include "SpawnQueryGraph.h"
#include "SpawnQuery/SpawnQueryNode_Sampler.h"
#include "HAL/PlatformMath.h"
#include "Internationalization/Internationalization.h"
#include "Templates/Casts.h"

class UToolMenu;

USpawnQueryGraphNode_Sampler::USpawnQueryGraphNode_Sampler(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
}

void USpawnQueryGraphNode_Sampler::AllocateDefaultPins()
{
    CreatePin(EGPD_Input, "MultipleNodes", TEXT("In"));
}

FText USpawnQueryGraphNode_Sampler::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
    const USpawnQueryNode* MyNode = Cast<USpawnQueryNode>(NodeInstance);
    if (MyNode != NULL)
    {
        return MyNode->GetDescriptionTitle();
    }
    return Super::GetNodeTitle(TitleType);
}

FText USpawnQueryGraphNode_Sampler::GetDescription() const
{
    return Super::GetDescription();
}

FText USpawnQueryGraphNode_Sampler::GetTooltipText() const
{
    return Super::GetTooltipText();
}

void USpawnQueryGraphNode_Sampler::GetNodeContextMenuActions(UToolMenu* Menu, UGraphNodeContextMenuContext* Context) const
{
    Super::GetNodeContextMenuActions(Menu, Context);
}
