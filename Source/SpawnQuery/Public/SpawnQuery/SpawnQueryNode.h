#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "UObject/Object.h"
//#include "SpawnQuery/SpawnQueryTypes.h"
#include "SpawnQueryNode.generated.h"

struct FPropertyChangedEvent;

UCLASS(Abstract, MinimalAPI)
class USpawnQueryNode : public UObject
{
	GENERATED_UCLASS_BODY()

	SPAWNQUERY_API virtual FText GetDescriptionTitle() const;
	SPAWNQUERY_API virtual FText GetDescriptionDetails() const;

	/**
	 * To be extended by any Node who offloads its work to another thread.
	 * Returns false by default, unless overridden.
	 * If overridden, will return whether or not this Node is currently being processed asynchronously.
	 */
	virtual FORCEINLINE bool IsCurrentlyRunningAsync() const { return false; }

#if WITH_EDITOR
	SPAWNQUERY_API virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif //WITH_EDITOR
};
