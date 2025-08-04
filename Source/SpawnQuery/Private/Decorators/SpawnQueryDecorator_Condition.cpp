#include "SpawnQuery/Decorators/SpawnQueryDecorator_Condition.h"

#include "SpawnQuery/SpawnQueryContext.h"

void USpawnQueryDecorator_Condition::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    Super::PostEditChangeProperty(PropertyChangedEvent);
}

FText USpawnQueryDecorator_Condition::GetDescriptionDetails() const
{
    return Super::GetDescriptionDetails();
}

bool USpawnQueryDecorator_Condition::IsActive(const USpawnQueryContext& Context)
{
    const UBlackboardComponent& BlackboardComp = Context.GetBlackboardRef();

    bool bResult = false;
    if (BlackboardKey.IsValid())
    {
        FBlackboard::FKey KeyID = BlackboardComp.GetKeyID(BlackboardKey);
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
