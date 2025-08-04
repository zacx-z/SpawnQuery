#include "SpawnQuery/Decorators/SpawnQueryDecorator_Condition.h"

#include "BehaviorTree/Blackboard/BlackboardKeyType_Enum.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_NativeEnum.h"
#include "BehaviorTree/Decorators/BTDecorator_Blackboard.h"
#include "SpawnQuery/SpawnQueryContext.h"

#if WITH_EDITOR

void USpawnQueryDecorator_Condition::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    Super::PostEditChangeProperty(PropertyChangedEvent);

    if (PropertyChangedEvent.Property == nullptr)
    {
        return;
    }

    const FName ChangedPropName = PropertyChangedEvent.Property->GetFName();

    const FName SelectedKeyName = GET_MEMBER_NAME_CHECKED(FBlackboardKeySelector, SelectedKeyName);
    const FName IntValueName = GET_MEMBER_NAME_CHECKED(USpawnQueryDecorator_Condition, IntValue);
    const FName StringValueName = GET_MEMBER_NAME_CHECKED(USpawnQueryDecorator_Condition, StringValue);

    const bool bIsEnumKey = BlackboardKeyType == UBlackboardKeyType_Enum::StaticClass()
        || BlackboardKeyType == UBlackboardKeyType_NativeEnum::StaticClass();

    if (bIsEnumKey && (ChangedPropName == SelectedKeyName || ChangedPropName == IntValueName || ChangedPropName ==
        StringValueName))
    {
        if (ChangedPropName == SelectedKeyName)
        {
            // Set both properties to match the first valid value or invalidate them
            if (EnumType != nullptr && EnumType->NumEnums())
            {
                IntValue = IntCastChecked<int32>(EnumType->GetValueByIndex(0));
                StringValue = EnumType->GetNameStringByIndex(0);
            }
            else
            {
                IntValue = INDEX_NONE;
                StringValue.Reset();
            }
        }
        else if (ChangedPropName == IntValueName)
        {
            StringValue = (EnumType != nullptr) ? EnumType->GetNameStringByValue(IntValue) : TEXT("");
        }
        else if (ChangedPropName == StringValueName)
        {
            IntValue = (EnumType != nullptr)
                           ? IntCastChecked<int32>(EnumType->GetValueByNameString(StringValue))
                           : INDEX_NONE;
        }
    }

    const bool bKeyTypeChanged = (ChangedPropName == GET_MEMBER_NAME_CHECKED(FBlackboardKeySelector, SelectedKeyType));

    const UBlackboardKeyType* KeyCDO = BlackboardKeyType
                                           ? BlackboardKeyType->GetDefaultObject<UBlackboardKeyType>()
                                           : nullptr;
    if (bKeyTypeChanged || ChangedPropName == GET_MEMBER_NAME_CHECKED(USpawnQueryDecorator_Condition, BasicOperation))
    {
        if (KeyCDO && KeyCDO->GetTestOperation() == EBlackboardKeyOperation::Basic)
        {
            OperationType = BasicOperation.GetIntValue();
        }
    }
    if (bKeyTypeChanged || ChangedPropName == GET_MEMBER_NAME_CHECKED(USpawnQueryDecorator_Condition, ArithmeticOperation))
    {
        if (KeyCDO && KeyCDO->GetTestOperation() == EBlackboardKeyOperation::Arithmetic)
        {
            OperationType = ArithmeticOperation.GetIntValue();
        }
    }
    if (bKeyTypeChanged || ChangedPropName == GET_MEMBER_NAME_CHECKED(USpawnQueryDecorator_Condition, TextOperation))
    {
        if (KeyCDO && KeyCDO->GetTestOperation() == EBlackboardKeyOperation::Text)
        {
            OperationType = TextOperation.GetIntValue();
        }
    }

    BuildDescription();
}

#endif

FText USpawnQueryDecorator_Condition::GetDescriptionDetails() const
{
    return CachedDescription;
}

bool USpawnQueryDecorator_Condition::IsActive(const USpawnQueryContext& Context)
{
    const UBlackboardComponent& BlackboardComp = Context.GetBlackboardRef();

    bool bResult = false;
    if (BlackboardKey.IsValid() && BlackboardComp.HasValidAsset())
    {
        FBlackboard::FKey KeyID = BlackboardComp.GetKeyID(BlackboardKey);
        if (!BlackboardComp.IsValidKey(KeyID))
        {
            return false;
        }
        UBlackboardKeyType* KeyCDO = BlackboardComp.GetKeyType(KeyID)->GetDefaultObject<UBlackboardKeyType>();
        const uint8* KeyMemory = BlackboardComp.GetKeyRawData(KeyID);

        // KeyMemory can be NULL if the blackboard has its data setup wrong, so we must conditionally handle that case.
        if (ensure(KeyCDO != NULL) && (KeyMemory != NULL))
        {
            const EBlackboardKeyOperation::Type Op = KeyCDO->GetTestOperation();
            switch (Op)
            {
            case EBlackboardKeyOperation::Basic:
                bResult = KeyCDO->WrappedTestBasicOperation(BlackboardComp, KeyMemory, (EBasicKeyOperation::Type)OperationType);
                break;

            case EBlackboardKeyOperation::Arithmetic:
                bResult = KeyCDO->WrappedTestArithmeticOperation(BlackboardComp, KeyMemory, (EArithmeticKeyOperation::Type)OperationType, IntValue, FloatValue);
                break;

            case EBlackboardKeyOperation::Text:
                bResult = KeyCDO->WrappedTestTextOperation(BlackboardComp, KeyMemory, (ETextKeyOperation::Type)OperationType, StringValue);
                break;

            default:
                break;
            }
        }
    }

    return bResult;
}

#if WITH_EDITOR

static UEnum* BasicOpEnum = NULL;
static UEnum* ArithmeticOpEnum = NULL;
static UEnum* TextOpEnum = NULL;

static void CacheOperationEnums()
{
    if (BasicOpEnum == NULL)
    {
        BasicOpEnum = StaticEnum<EBasicKeyOperation::Type>();
        check(BasicOpEnum);
    }

    if (ArithmeticOpEnum == NULL)
    {
        ArithmeticOpEnum = StaticEnum<EArithmeticKeyOperation::Type>();
        check(ArithmeticOpEnum);
    }

    if (TextOpEnum == NULL)
    {
        TextOpEnum = StaticEnum<ETextKeyOperation::Type>();
        check(TextOpEnum);
    }
}

void USpawnQueryDecorator_Condition::BuildDescription()
{
    FString BlackboardDesc = "Invalid";

    if (BlackboardKey.IsValid() && BlackboardKeyType)
    {
        // safety feature to not crash when changing couple of properties on a bunch
        // while "post edit property" triggers for every each of them
        RefreshEnumBasedDecorator();

        const FString KeyName = BlackboardKey.ToString();
        CacheOperationEnums();

        UBlackboardKeyType* KeyType = BlackboardKeyType->GetDefaultObject<UBlackboardKeyType>();

        const EBlackboardKeyOperation::Type Op = KeyType->GetTestOperation();
        switch (Op)
        {
        case EBlackboardKeyOperation::Basic:
            BlackboardDesc = FString::Printf(TEXT("%s is %s"), *KeyName, *BasicOpEnum->GetDisplayNameTextByValue(OperationType).ToString());
            break;

        case EBlackboardKeyOperation::Arithmetic:
            BlackboardDesc = FString::Printf(TEXT("%s %s %s"), *KeyName, *ArithmeticOpEnum->GetDisplayNameTextByValue(OperationType).ToString(),
                *KeyType->DescribeArithmeticParam(IntValue, FloatValue));
            break;

        case EBlackboardKeyOperation::Text:
            BlackboardDesc = FString::Printf(TEXT("%s %s [%s]"), *KeyName, *TextOpEnum->GetDisplayNameTextByValue(OperationType).ToString(), *StringValue);
            break;

        default: break;
        }
    }

    CachedDescription = FText::FromString(BlackboardDesc);
}

#endif


void USpawnQueryDecorator_Condition::RefreshEnumBasedDecorator()
{
    if (BlackboardKeyType != UBlackboardKeyType_Enum::StaticClass() &&
        BlackboardKeyType != UBlackboardKeyType_NativeEnum::StaticClass())
    {
        return;
    }

    if (EnumType == nullptr)
    {
        return;
    }

    // Enum implementation relies on 'StringValue' to store enum name but also
    // synchronizes 'IntValue' as the enumeration value for runtime evaluation
    if (!StringValue.IsEmpty())
    {
        // Pass string as FName to use fast search path whenever possible 
        const FName Name = *StringValue;
        IntValue = IntCastChecked<int32>(EnumType->GetValueByName(Name));
    }
    else if (IntValue >= 0 && IntValue < EnumType->NumEnums())
    {
        // Previous implementation was using only 'IntValue' to store the enumeration
        // index so we need to extract associated name and update `IntValue` to store the value.
        StringValue = EnumType->GetNameByIndex(IntValue).ToString();
        IntValue = IntCastChecked<int32>(EnumType->GetValueByIndex(IntValue));
    }
}
