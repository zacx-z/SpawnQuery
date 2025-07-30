#include "SpawnQueryGraphNode_Decorator.h"
#include "SpawnQueryColors.h"

#include "SpawnQuery/SpawnQueryNode_Decorator.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(SpawnQueryGraphNode_Decorator)

FText USpawnQueryGraphNode_Decorator::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
    const USpawnQueryNode_Decorator* Decorator = Cast<USpawnQueryNode_Decorator>(NodeInstance);
    if (Decorator != nullptr)
    {
        return Decorator->GetDescriptionTitle();
    }
    else if (!ClassData.GetClassName().IsEmpty())
    {
        FString StoredClassName = ClassData.GetClassName();
        StoredClassName.RemoveFromEnd(TEXT("_C"));

        return FText::Format(NSLOCTEXT("AIGraph", "NodeClassError", "Class {0} not found, make sure it's saved!"), FText::FromString(StoredClassName));
    }

    return Super::GetNodeTitle(TitleType);
}

FText USpawnQueryGraphNode_Decorator::GetDescription() const
{
    if (const USpawnQueryNode_Decorator* MyNode = Cast<USpawnQueryNode_Decorator>(NodeInstance))
    {
        return MyNode->GetDescriptionDetails();
    }

    return Super::GetDescription();
}

void USpawnQueryGraphNode_Decorator::AllocateDefaultPins()
{
    // No pins for decorators
}

FLinearColor USpawnQueryGraphNode_Decorator::GetBackgroundColor() const
{
    return SpawnQueryColors::NodeBody::Decorator;
}
