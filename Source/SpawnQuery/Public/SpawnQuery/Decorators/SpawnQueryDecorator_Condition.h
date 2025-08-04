#pragma once
#include "BehaviorTree/Blackboard/BlackboardKeyEnums.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType.h"
#include "SpawnQuery/SpawnQueryNode_Decorator.h"
#include "SpawnQueryDecorator_Condition.generated.h"

class FConditionDecoratorDetails;

UCLASS(MinimalAPI)
class USpawnQueryDecorator_Condition : public USpawnQueryNode_Decorator
{
    GENERATED_BODY()
    
#if WITH_EDITOR
protected:
    SPAWNQUERY_API virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

public:
    //~ begin USpawnQueryNode
    SPAWNQUERY_API virtual FText GetDescriptionDetails() const override;
    SPAWNQUERY_API virtual bool IsActive(const USpawnQueryContext& Context) override;
    //~ end USpawnQueryNode

protected:

    /** blackboard key name */
    UPROPERTY(EditAnywhere, Category=Blackboard)
    FName BlackboardKey;

    /** blackboard key type */
    UPROPERTY(EditAnywhere, Category=Blackboard)
    TSubclassOf<UBlackboardKeyType> BlackboardKeyType;

    UPROPERTY(EditAnywhere, Category=Blackboard)
    TObjectPtr<UEnum> EnumType;

    /** value for arithmetic operations */
    UPROPERTY(Category=Blackboard, EditAnywhere, meta=(DisplayName="Key Value"))
    int32 IntValue;

    /** value for arithmetic operations */
    UPROPERTY(Category=Blackboard, EditAnywhere, meta=(DisplayName="Key Value"))
    float FloatValue;

    /** value for string operations */
    UPROPERTY(Category=Blackboard, EditAnywhere, meta=(DisplayName="Key Value"))
    FString StringValue;

    /** cached description */
    UPROPERTY()
    FText CachedDescription;

    /** operation type */
    UPROPERTY()
    uint8 OperationType;

#if WITH_EDITORONLY_DATA

    UPROPERTY(Category=Blackboard, EditAnywhere, meta=(DisplayName="Key Query"))
    TEnumAsByte<EBasicKeyOperation::Type> BasicOperation;

    UPROPERTY(Category=Blackboard, EditAnywhere, meta=(DisplayName="Key Query"))
    TEnumAsByte<EArithmeticKeyOperation::Type> ArithmeticOperation;

    UPROPERTY(Category=Blackboard, EditAnywhere, meta=(DisplayName="Key Query"))
    TEnumAsByte<ETextKeyOperation::Type> TextOperation;

#endif

    friend FConditionDecoratorDetails;

private:
#if WITH_EDITOR
    void BuildDescription();
#endif
    void RefreshEnumBasedDecorator();
};
