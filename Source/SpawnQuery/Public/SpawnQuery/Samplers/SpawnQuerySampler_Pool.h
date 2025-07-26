#pragma once
#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "SpawnQuery/SpawnQueryNode_Sampler.h"
#include "SpawnQuerySampler_Pool.generated.h"

UCLASS(MinimalAPI)
class USpawnQuerySampler_Pool : public USpawnQueryNode_Sampler
{
	GENERATED_UCLASS_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (RequiredAssetDataTags = "RowStructure=/Script/SpawnQuery.SpawnEntryTableRow"))
	TObjectPtr<UDataTable> PoolTable;

public:
	virtual bool IsActive(USpawnQueryContext& context) override;
	virtual TSharedPtr<ISpawnEntryBase> Query(USpawnQueryContext& context) override;
};
