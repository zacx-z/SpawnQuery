#include "AssetDefinition_SpawnQuery.h"
#include "SpawnQuery.h"
#include "SpawnQueryEditorModule.h"
#include "Modules/ModuleManager.h"

#define LOCTEXT_NAMESPACE "AssetTypeActions"

FText UAssetDefinition_SpawnQuery::GetAssetDisplayName() const
{
    return LOCTEXT("AssetTypeActions_SpawnQuery", "Spawn Query");
}

FLinearColor UAssetDefinition_SpawnQuery::GetAssetColor() const
{
    return FColor(201, 29, 85);
}

TSoftClassPtr<> UAssetDefinition_SpawnQuery::GetAssetClass() const
{
    return USpawnQuery::StaticClass();
}

TConstArrayView<FAssetCategoryPath> UAssetDefinition_SpawnQuery::GetAssetCategories() const
{
    static const FAssetCategoryPath Categories[] = { EAssetCategoryPaths::Misc };
    return Categories;
}

EAssetCommandResult UAssetDefinition_SpawnQuery::OpenAssets(const FAssetOpenArgs& OpenArgs) const
{
    FSpawnQueryEditorModule& SpawnQueryEditorModule = FModuleManager::LoadModuleChecked<FSpawnQueryEditorModule>("SpawnQueryEditor");
    for (USpawnQuery* Query : OpenArgs.LoadObjects<USpawnQuery>())
    {
        SpawnQueryEditorModule.CreateSpawnQueryEditor(EToolkitMode::Standalone, OpenArgs.ToolkitHost, Query);
    }
    return EAssetCommandResult::Handled;
}
#undef LOCTEXT_NAMESPACE