#pragma once

#include "SGraphNodeAI.h"

class SVerticalBox;
class USpawnQueryGraphNode;

class SGraphNode_SpawnQuery : public SGraphNodeAI
{
public:
    SLATE_BEGIN_ARGS(SGraphNode_SpawnQuery) {}
    SLATE_END_ARGS()

    void Construct(const FArguments& InArgs, USpawnQueryGraphNode* InNode);

    //~ Begin SWidget interface
    virtual void Tick( const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime ) override;
    //~ End SWidget interface

    //~ Begin SGraphNode interface
    virtual void UpdateGraphNode() override;
    virtual void CreatePinWidgets() override;
    //~ End SGraphNode interface

    //~ Begin SGraphNodeAI interface
    /** adds decorator widget inside current node */
    virtual void AddSubNode(TSharedPtr<SGraphNode> SubNodeWidget) override;
    //~ End SGraphNodeAI interface

protected:
    TSharedPtr<SVerticalBox> DecoratorsBox;

    FSlateColor GetBorderBackgroundColor() const;
    FSlateColor GetBackgroundColor() const;
};
