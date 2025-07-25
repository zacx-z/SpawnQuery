#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Templates/SubclassOf.h"
#include "SpawnQuery/SpawnQueryNode.h"
#include "SpawnQueryNode_Sampler.generated.h"


UCLASS(EditInlineNew, Abstract, meta = (Category = "Samplers"), MinimalAPI)
class USpawnQueryNode_Sampler : public USpawnQueryNode
{
	GENERATED_UCLASS_BODY()

	SPAWNQUERY_API virtual void PostLoad() override;
};
