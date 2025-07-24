#include "SpawnQueryFactory.h"
#include "SpawnQuery.h"

#include "AssetTypeCategories.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(SpawnQueryFactory)

USpawnQueryFactory::USpawnQueryFactory(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SupportedClass = USpawnQuery::StaticClass();
	bEditAfterNew = true;
	bCreateNew = true;
}

UObject* USpawnQueryFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	check(Class->IsChildOf(USpawnQuery::StaticClass()));
	return NewObject<USpawnQuery>(InParent, Class, Name, Flags);
}

bool USpawnQueryFactory::CanCreateNew() const
{
	// Always true if this plugin has been loaded
	return true;
}
