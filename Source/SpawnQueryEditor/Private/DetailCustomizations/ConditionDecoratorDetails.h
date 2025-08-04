#pragma once
#include "IDetailCustomization.h"
#include "Kismet2/EnumEditorUtils.h"
#include "SpawnQuery/Decorators/SpawnQueryDecorator_Condition.h"

class USpawnQueryDecorator_Condition;

class FConditionDecoratorDetails : public IDetailCustomization, public FEnumEditorUtils::INotifyOnEnumChanged
{
public:

    /** Makes a new instance of this detail layout class for a specific detail view requesting it */
    static TSharedRef<IDetailCustomization> MakeInstance();

    bool IsEditingEnabled() const;

protected:
    virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override;

    virtual void PreChange(const UUserDefinedEnum* Changed, FEnumEditorUtils::EEnumEditorChangeInfo ChangedType) override;
    virtual void PostChange(const UUserDefinedEnum* Changed, FEnumEditorUtils::EEnumEditorChangeInfo ChangedType) override;

private:
    void CacheDecoratorRef(IDetailLayoutBuilder& DetailLayout);
    void OnKeyTypeChanged();
    void RefreshEnumPropertyValues();
    TSharedRef<SWidget> OnGetEnumValueContent() const;
    FText GetCurrentEnumValueDesc() const;
    void OnEnumValueComboChange(int Index);

    /** property utils */
    IPropertyUtilities* PropUtils = nullptr;

    TWeakObjectPtr<USpawnQueryDecorator_Condition> CachedConditionDecorator;

    TSubclassOf<UBlackboardKeyType> CachedKeyType = nullptr;
    UEnum* CachedCustomObjectType = nullptr;
    EBlackboardKeyOperation::Type CachedOperationType = EBlackboardKeyOperation::Basic;
    TArray<FString> EnumPropValues;
    TSharedPtr<IPropertyHandle> StringValueProperty;
};
