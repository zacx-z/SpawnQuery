// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"
#include "SpawnQuery/SpawnQueryContext.h"

class USpawnQueryContext;

class FSpawnQueryModule : public IModuleInterface
{
public:

    /** IModuleInterface implementation */
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;

    USpawnQueryContext* GetDefaultContext() const {
        return SpawnQueryContext;
    }

private:
    USpawnQueryContext* SpawnQueryContext = nullptr;
};
