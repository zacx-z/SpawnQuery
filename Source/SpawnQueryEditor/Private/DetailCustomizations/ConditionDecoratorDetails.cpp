#include "ConditionDecoratorDetails.h"

#include "DetailCategoryBuilder.h"
#include "DetailLayoutBuilder.h"
#include "DetailWidgetRow.h"
#include "IPropertyUtilities.h"
#include "SpawnQueryDebugger.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Enum.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Float.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Int.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_NativeEnum.h"
#include "SpawnQuery/Decorators/SpawnQueryDecorator_Condition.h"

#define LOCTEXT_NAMESPACE "ConditionDecoratorDetails"

TSharedRef<IDetailCustomization> FConditionDecoratorDetails::MakeInstance()
{
    return MakeShareable(new FConditionDecoratorDetails);
}

bool FConditionDecoratorDetails::IsEditingEnabled() const
{
    return (!FSpawnQueryDebugger::IsPIESimulating()) && PropUtils->IsPropertyEditingEnabled();
}

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void FConditionDecoratorDetails::CustomizeDetails(IDetailLayoutBuilder& DetailLayout)
{
    CacheDecoratorRef(DetailLayout);

    PropUtils = &DetailLayout.GetPropertyUtilities().Get();

    TAttribute<bool> PropertyEditCheck = TAttribute<bool>::Create(TAttribute<bool>::FGetter::CreateSP(this, &FConditionDecoratorDetails::IsEditingEnabled));

    IDetailCategoryBuilder& BBCategory = DetailLayout.EditCategory("Blackboard");
    IDetailPropertyRow& KeySelectorRow = BBCategory.AddProperty(DetailLayout.GetProperty(GET_MEMBER_NAME_CHECKED(USpawnQueryDecorator_Condition, BlackboardKey)));
    IDetailPropertyRow& KeyTypeRow = BBCategory.AddProperty(
        DetailLayout.GetProperty(GET_MEMBER_NAME_CHECKED(USpawnQueryDecorator_Condition, BlackboardKeyType)));
    TSharedPtr<IPropertyHandle> KeyTypeProperty = KeyTypeRow.GetPropertyHandle();
    if (KeyTypeProperty.IsValid())
    {
        KeyTypeProperty->SetOnPropertyValueChanged(FSimpleDelegate::CreateSP(this, &FConditionDecoratorDetails::OnKeyTypeChanged));
        OnKeyTypeChanged();
    }

#if WITH_EDITORONLY_DATA

    IDetailPropertyRow& BasicOpRow = BBCategory.AddProperty(DetailLayout.GetProperty(GET_MEMBER_NAME_CHECKED(USpawnQueryDecorator_Condition, BasicOperation)));
    BasicOpRow.Visibility(TAttribute<EVisibility>::Create(TAttribute<EVisibility>::FGetter::CreateLambda(
        [&]()
        {
            return CachedOperationType == EBlackboardKeyOperation::Basic ? EVisibility::Visible : EVisibility::Collapsed;
        })));
    BasicOpRow.IsEnabled(PropertyEditCheck);
    
    IDetailPropertyRow& ArithmeticOpRow = BBCategory.AddProperty(DetailLayout.GetProperty(GET_MEMBER_NAME_CHECKED(USpawnQueryDecorator_Condition, ArithmeticOperation)));
    ArithmeticOpRow.Visibility(TAttribute<EVisibility>::Create(TAttribute<EVisibility>::FGetter::CreateLambda(
        [&]()
        {
            return CachedOperationType == EBlackboardKeyOperation::Arithmetic ? EVisibility::Visible : EVisibility::Collapsed;
        })));
    ArithmeticOpRow.IsEnabled(PropertyEditCheck);

    IDetailPropertyRow& TextOpRow = BBCategory.AddProperty(DetailLayout.GetProperty(GET_MEMBER_NAME_CHECKED(USpawnQueryDecorator_Condition, TextOperation)));
    TextOpRow.Visibility(TAttribute<EVisibility>::Create(TAttribute<EVisibility>::FGetter::CreateLambda(
        [&]()
        {
            return CachedOperationType == EBlackboardKeyOperation::Text ? EVisibility::Visible : EVisibility::Collapsed;
        })));
    TextOpRow.IsEnabled(PropertyEditCheck);

#endif // WITH_EDITORONLY_DATA

    IDetailPropertyRow& IntValueRow = BBCategory.AddProperty(DetailLayout.GetProperty(GET_MEMBER_NAME_CHECKED(USpawnQueryDecorator_Condition, IntValue)));
    IntValueRow.Visibility(TAttribute<EVisibility>::Create(TAttribute<EVisibility>::FGetter::CreateLambda(
        [&]()
        {
            return (CachedKeyType == UBlackboardKeyType_Int::StaticClass()) ? EVisibility::Visible : EVisibility::Collapsed;
        })));
    IntValueRow.IsEnabled(PropertyEditCheck);

    IDetailPropertyRow& FloatValueRow = BBCategory.AddProperty(DetailLayout.GetProperty(GET_MEMBER_NAME_CHECKED(USpawnQueryDecorator_Condition, FloatValue)));
    FloatValueRow.Visibility(TAttribute<EVisibility>::Create(TAttribute<EVisibility>::FGetter::CreateLambda(
    [&]()
        {
            return (CachedKeyType == UBlackboardKeyType_Float::StaticClass()) ? EVisibility::Visible : EVisibility::Collapsed;
        })));
    FloatValueRow.IsEnabled(PropertyEditCheck);

    StringValueProperty = DetailLayout.GetProperty(GET_MEMBER_NAME_CHECKED(USpawnQueryDecorator_Condition, StringValue));
    IDetailPropertyRow& StringValueRow = BBCategory.AddProperty(StringValueProperty);
    StringValueRow.Visibility(TAttribute<EVisibility>::Create(TAttribute<EVisibility>::FGetter::CreateLambda(
    [&]()
        {
            return (CachedOperationType == EBlackboardKeyOperation::Text) ? EVisibility::Visible : EVisibility::Collapsed;
        })));
    StringValueRow.IsEnabled(PropertyEditCheck);

    // TODO: need a control to specify a concrete enum type
    IDetailPropertyRow& EnumValueRow = BBCategory.AddProperty(StringValueProperty);
    EnumValueRow.Visibility(TAttribute<EVisibility>::Create(TAttribute<EVisibility>::FGetter::CreateLambda(
        [&]()
        {
            return (CachedKeyType == UBlackboardKeyType_Enum::StaticClass() || CachedKeyType == UBlackboardKeyType_NativeEnum::StaticClass())
            ? EVisibility::Visible : EVisibility::Collapsed;
        })));
    EnumValueRow.IsEnabled(PropertyEditCheck);
    EnumValueRow.CustomWidget()
        .NameContent()
        [
            StringValueProperty->CreatePropertyNameWidget()
        ]
        .ValueContent()
        [
            SNew(SComboButton)
            .OnGetMenuContent(this, &FConditionDecoratorDetails::OnGetEnumValueContent)
            .ContentPadding(FMargin( 2.0f, 2.0f ))
            .ButtonContent()
            [
                SNew(STextBlock) 
                .Text(this, &FConditionDecoratorDetails::GetCurrentEnumValueDesc)
                .Font(IDetailLayoutBuilder::GetDetailFont())
            ]
        ];
}

void FConditionDecoratorDetails::PreChange(const UUserDefinedEnum* Changed,
    FEnumEditorUtils::EEnumEditorChangeInfo ChangedType)
{
}

void FConditionDecoratorDetails::PostChange(const UUserDefinedEnum* Changed,
    FEnumEditorUtils::EEnumEditorChangeInfo ChangedType)
{
    if (Changed != nullptr && CachedCustomObjectType == Changed)
    {
        RefreshEnumPropertyValues();
    }
}

void FConditionDecoratorDetails::CacheDecoratorRef(IDetailLayoutBuilder& DetailLayout)
{
    TArray<TWeakObjectPtr<UObject>> MyOuters;
    DetailLayout.GetObjectsBeingCustomized(MyOuters);

    CachedConditionDecorator.Reset();
    for (int32 i = 0; i < MyOuters.Num(); i++)
    {
        USpawnQueryDecorator_Condition* Decorator = Cast<USpawnQueryDecorator_Condition>(MyOuters[i].Get());
        if (Decorator)
        {
            CachedConditionDecorator = Decorator;
            break;
        }
    }
}

void FConditionDecoratorDetails::OnKeyTypeChanged()
{
    if (CachedConditionDecorator.IsValid())
    {
        CachedKeyType = CachedConditionDecorator->BlackboardKeyType;
        if (CachedKeyType != nullptr)
        {
            CachedOperationType = CachedKeyType->GetDefaultObject<UBlackboardKeyType>()->GetTestOperation();
        }
    }

    
    // special handling of enum type: cache all names for combo box (display names)
    UEnum* SelectedEnumType = nullptr;
    if (CachedKeyType == UBlackboardKeyType_Enum::StaticClass())
    {
        SelectedEnumType = CachedKeyType->GetDefaultObject<UBlackboardKeyType_Enum>()->EnumType;
    }
    else if (CachedKeyType == UBlackboardKeyType_NativeEnum::StaticClass())
    {
        SelectedEnumType = CachedKeyType->GetDefaultObject<UBlackboardKeyType_NativeEnum>()->EnumType;
    }

    if (SelectedEnumType)
    {
        CachedCustomObjectType = SelectedEnumType;
        RefreshEnumPropertyValues();
    }
}

void FConditionDecoratorDetails::RefreshEnumPropertyValues()
{
    EnumPropValues.Reset();

    if (CachedCustomObjectType)
    {
        UE_LOG(LogTemp, Warning, TEXT("a %p"), CachedCustomObjectType);
        for (int32 i = 0; i < CachedCustomObjectType->NumEnums() - 1; i++)
        {
            FString DisplayedName = CachedCustomObjectType->GetDisplayNameTextByIndex(i).ToString();
            EnumPropValues.Add(DisplayedName);
        }
    }
}

TSharedRef<SWidget> FConditionDecoratorDetails::OnGetEnumValueContent() const
{
    FMenuBuilder MenuBuilder(true, NULL);

    for (int32 i = 0; i < EnumPropValues.Num(); i++)
    {
        FUIAction ItemAction(FExecuteAction::CreateSP(const_cast<FConditionDecoratorDetails*>(this), &FConditionDecoratorDetails::OnEnumValueComboChange, i));
        MenuBuilder.AddMenuEntry(FText::FromString( EnumPropValues[i]), TAttribute<FText>(), FSlateIcon(), ItemAction);
    }

    return MenuBuilder.MakeWidget();
}

FText FConditionDecoratorDetails::GetCurrentEnumValueDesc() const
{
    FPropertyAccess::Result Result = FPropertyAccess::Fail;
    int32 EnumIndex = INDEX_NONE;

    if (CachedCustomObjectType)
    {
        // Always use string value to recompute matching index since enumeration
        // can be modified while Editing (i.e. UserDefinedEnumeration)
        FString CurrentStringValue;
        Result = StringValueProperty->GetValue(CurrentStringValue);
        EnumIndex = CachedCustomObjectType->GetIndexByNameString(CurrentStringValue);
    }

    return (Result == FPropertyAccess::Success && EnumPropValues.IsValidIndex(EnumIndex))
        ? FText::FromString(EnumPropValues[EnumIndex])
        : FText::GetEmpty();
}

void FConditionDecoratorDetails::OnEnumValueComboChange(int Index)
{
    if (CachedCustomObjectType)
    {
        const FString NewStringValue = CachedCustomObjectType->GetNameStringByIndex(Index);
        StringValueProperty->SetValue(NewStringValue);
    }
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION

#undef LOCTEXT_NAMESPACE 