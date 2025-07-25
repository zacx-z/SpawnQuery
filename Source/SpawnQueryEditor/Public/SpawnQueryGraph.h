#pragma once

#include "AIGraph.h"
#include "SpawnQueryGraphNode.h"
#include "SpawnQueryGraph.generated.h"

UCLASS()
class USpawnQueryGraph : public UAIGraph
{
	GENERATED_UCLASS_BODY()

	virtual void Initialize() override;
	virtual void OnLoaded() override;

	virtual void UpdateAsset(int32 UpdateFlags = 0) override;
	
	void SpawnMissingNodes();
	
private:
	
	void BuildQueryTree(USpawnQueryGraphNode* Node);
};
