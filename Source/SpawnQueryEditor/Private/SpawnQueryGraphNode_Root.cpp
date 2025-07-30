#include "SpawnQueryGraphNode_Root.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(SpawnQueryGraphNode_Root)

USpawnQueryGraphNode_Root::USpawnQueryGraphNode_Root(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    bIsReadOnly = true;
    bSupportDecorators = false;
}

void USpawnQueryGraphNode_Root::AllocateDefaultPins()
{
    UEdGraphPin* Outputs = CreatePin(EGPD_Output, TEXT("Transition"), TEXT("Out"));
}

FText USpawnQueryGraphNode_Root::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
    return NSLOCTEXT("SpawnQueryEditor", "Root", "ROOT");
}

void USpawnQueryGraphNode_Root::LogDebugMessage(const FString& Message)
{
    if (DebugMessages.Num() == 0)
    {
        bHasDebugError = false;
    }

    // store only 1 error message, discard everything after it
    if (!bHasDebugError)
    {
        DebugMessages.Add(Message);
    }
}

void USpawnQueryGraphNode_Root::LogDebugError(const FString& Message)
{
    if (DebugMessages.Num() == 0)
    {
        bHasDebugError = false;
    }

    // store only 1 error message, discard everything after it
    if (!bHasDebugError)
    {
        DebugMessages.Add(Message);
        bHasDebugError = true;
    }
}
