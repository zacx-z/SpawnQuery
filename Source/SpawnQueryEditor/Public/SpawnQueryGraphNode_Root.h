#pragma once

#include "SpawnQueryGraphNode.h"
#include "SpawnQueryGraphNode_Root.generated.h"

UCLASS(MinimalAPI)
class USpawnQueryGraphNode_Root : public USpawnQueryGraphNode
{
    GENERATED_UCLASS_BODY()

    UPROPERTY(EditAnywhere, Category = Debug)
    TArray<FString> DebugMessages;

    UPROPERTY()
    bool bHasDebugError;

    void LogDebugMessage(const FString& Message);
    void LogDebugError(const FString& Message);

    SPAWNQUERYEDITOR_API virtual void AllocateDefaultPins() override;
    SPAWNQUERYEDITOR_API virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
    SPAWNQUERYEDITOR_API virtual bool HasErrors() const override { return false; }
};
