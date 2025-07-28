#pragma once

#include "Factories/Factory.h"
#include "SpawnQueryFactory.generated.h"

UCLASS(hidecategories = Object)
class USpawnQueryFactory : public UFactory
{
    GENERATED_UCLASS_BODY()

    // UFactory interface
    virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
    virtual bool CanCreateNew() const override;
    // End of UFactory interface
};
