#pragma once

#include "AIGraphNode.h"
#include "SpawnQueryGraphNode.generated.h"

class UEdGraphSchema;

UCLASS()
class SPAWNQUERYEDITOR_API USpawnQueryGraphNode : public UAIGraphNode
{
    GENERATED_UCLASS_BODY()

    /**
     * Nodes except ROOT support decorators.
     */
    UPROPERTY()
    TArray<TObjectPtr<USpawnQueryGraphNode>> Decorators;

    virtual class USpawnQueryGraph* GetSpawnQueryGraph();

    //~ Begin UEdGraphNode
    virtual void AllocateDefaultPins() override;
    virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
    virtual FText GetDescription() const override;
    virtual void GetNodeContextMenuActions(UToolMenu* Menu, UGraphNodeContextMenuContext* Context) const override;
    //~ End UEdGraphNode

    //~ Begin UAIGraphNode
    virtual void OnSubNodeAdded(UAIGraphNode* SubNode) override;
    virtual void OnSubNodeRemoved(UAIGraphNode* SubNode) override;
    virtual void RemoveAllSubNodes() override;
    virtual void InsertSubNodeAt(UAIGraphNode* SubNode, int32 DropIndex) override;
    //~ End UAIGraphNode

    virtual bool CanCreateUnderSpecifiedSchema(const UEdGraphSchema* DesiredSchema) const override;

    virtual FLinearColor GetBackgroundColor() const;

protected:

    bool bSupportDecorators = true;

private:

    void CreateAddDecoratorSubMenu(UToolMenu* Menu, UEdGraph* Graph) const;

public:

    //~ Begin debugger variables
    bool bDebuggerActiveState;
    //~ End debugger variables
};
