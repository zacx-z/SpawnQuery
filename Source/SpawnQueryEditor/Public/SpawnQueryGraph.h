#pragma once

#include "AIGraph.h"
#include "SpawnQueryGraph.generated.h"

UCLASS()
class USpawnQueryGraph : public UAIGraph
{
	GENERATED_UCLASS_BODY()

	virtual void Initialize() override;
	virtual void OnLoaded() override;

	void SpawnMissingNodes();
};
