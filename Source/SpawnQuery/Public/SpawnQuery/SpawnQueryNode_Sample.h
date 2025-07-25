#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Templates/SubclassOf.h"
#include "SpawnQuery/SpawnQueryNode.h"
#include "SpawnQueryNode_Sample.generated.h"


UCLASS(EditInlineNew, Abstract, meta = (Category = "Generators"), MinimalAPI)
class USpawnQueryNode_Sample : public USpawnQueryNode
{
	GENERATED_UCLASS_BODY()

	SPAWNQUERY_API virtual void PostLoad() override;
};
