#pragma once

#include "AIGraphEditor.h"
#include "ISpawnQueryEditor.h"

class FSpawnTabArgs;

class UEdGraph;
class USpawnQuery;

class FSpawnQueryEditor : public ISpawnQueryEditor, public FAIGraphEditor
{

public:

	void InitSpawnQueryEditor(const EToolkitMode::Type Mode, const TSharedPtr< class IToolkitHost >& InitToolkitHost, USpawnQuery* Script);

	//~ Begin IToolkit Interface
	virtual FName GetToolkitFName() const override;
	virtual FText GetBaseToolkitName() const override;
	virtual FString GetWorldCentricTabPrefix() const override;
	virtual FLinearColor GetWorldCentricTabColorScale() const override;
	//~ End IToolkit Interface
private:
	USpawnQuery* Query;
};
