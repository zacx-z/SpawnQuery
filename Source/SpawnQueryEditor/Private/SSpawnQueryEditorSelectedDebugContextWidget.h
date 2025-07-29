#pragma once

class USpawnQueryContext;
class FSpawnQueryEditor;

class SSpawnQueryEditorSelectedDebugContextWidget : public SCompoundWidget
{
public:
    SLATE_BEGIN_ARGS(SSpawnQueryEditorSelectedDebugContextWidget){}
    SLATE_END_ARGS()

    void Construct(const FArguments& InArgs, TSharedPtr<FSpawnQueryEditor> InEditor);

    //~ Begin SWidget interface
    virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;
    //~ End SWidget interface

private:
    FText GetSelectedDebugContextTextLabel() const;
    TSharedRef<SWidget> CreateDebugContextWidget(TWeakObjectPtr<USpawnQueryContext> InItem);
    void OnDebugContextChanged(TWeakObjectPtr<USpawnQueryContext> NewSelection, ESelectInfo::Type SelectInfo);

    TWeakPtr<FSpawnQueryEditor> SpawnQueryEditor;
    TSharedPtr<SComboBox<TWeakObjectPtr<USpawnQueryContext>>> DebugContextComboBox;
};
