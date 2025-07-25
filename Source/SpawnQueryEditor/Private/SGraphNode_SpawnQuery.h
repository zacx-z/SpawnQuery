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

	// SGraphNode interface
	virtual void UpdateGraphNode() override;
	virtual void CreatePinWidgets() override;
	// End of SGraphNode interface

	/** adds decorator widget inside current node */
	virtual void AddSubNode(TSharedPtr<SGraphNode> SubNodeWidget) override;

	EVisibility GetWeightMarkerVisibility() const;
	TOptional<float> GetWeightProgressBarPercent() const;
	FSlateColor GetWeightProgressBarColor() const;

protected:
	TSharedPtr<SVerticalBox> TestBox;

	FSlateColor GetBorderBackgroundColor() const;
	FSlateColor GetBackgroundColor() const;
};
