#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraphNode.h"
#include "EdGraph/EdGraphNodeUtils.h"
#include "K2Node.h"
#include "Textures/SlateIcon.h"
#include "UObject/ObjectMacros.h"
#include "UObject/UObjectGlobals.h"
#include "K2Node_GetSpawnEntryRow.generated.h"

UCLASS()
class UK2Node_GetSpawnEntryRow : public UK2Node
{
    GENERATED_BODY()

    // UObject interface
    virtual void PostLoad() override;
    // End of UObject interface

    //~ Begin UEdGraphNode Interface.
    virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
    virtual FText GetTooltipText() const override;
    virtual void AllocateDefaultPins() override;
    virtual void PinDefaultValueChanged(UEdGraphPin* Pin) override;
    virtual void ExpandNode(class FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph) override;
    virtual FSlateIcon GetIconAndTint(FLinearColor& OutColor) const override;
    virtual void PostReconstructNode() override;
    //~ End UEdGraphNode Interface.

    //~ Begin UK2Node Interface
    virtual void GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const override;
    virtual FText GetMenuCategory() const override;

    virtual void EarlyValidation(class FCompilerResultsLog& MessageLog) const override;
    virtual void PreloadRequiredAssets() override;
    virtual bool IsNodeSafeToIgnore() const override { return true; }
    virtual void ReallocatePinsDuringReconstruction(TArray<UEdGraphPin*>& OldPins) override;
    virtual bool IsConnectionDisallowed(const UEdGraphPin* MyPin, const UEdGraphPin* OtherPin, FString& OutReason) const override;
    virtual void NotifyPinConnectionListChanged(UEdGraphPin* Pin) override;
    //~ End UK2Node Interface

private:
    void RefreshOutputPinType();
    UScriptStruct* GetDataTableRowStructType() const;

    UEdGraphPin* SpawnEntryPin;
    UEdGraphPin* ResultNamePin;
    UEdGraphPin* ResultPin;

    FNodeTextCache CachedNodeTitle;
};
