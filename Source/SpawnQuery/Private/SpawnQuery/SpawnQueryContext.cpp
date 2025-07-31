#include "SpawnQuery/SpawnQueryContext.h"

#include "SpawnQuery.h"
#include "SpawnQueryTypes.h"
#include "SpawnQuery/SpawnQueryContextActor.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(SpawnQueryContext)

void USpawnQueryContext::PostLoad()
{
    UObject::PostLoad();

    RandomStream = FRandomStream(RandomSeed);
}

void USpawnQueryContext::PostInitProperties()
{
    UObject::PostInitProperties();

    RandomStream = FRandomStream(RandomSeed);
}

void USpawnQueryContext::Reset()
{
    RandomStream.Reset();
}

bool USpawnQueryContext::IsSpawnQueryActive(const USpawnQuery* SpawnQuery, bool bDefault) const
{
    const bool* FoundValue = QueryActiveStateMap.Find(SpawnQuery);
    return FoundValue ? *FoundValue : bDefault;
}

void USpawnQueryContext::SetSpawnQueryActiveState(const USpawnQuery* SpawnQuery, bool bActiveState)
{
    QueryActiveStateMap.Add(SpawnQuery, bActiveState);
}

void USpawnQueryContext::SetBlackboardAsset(UBlackboardData* InBlackboardAsset)
{
    BlackboardAsset = InBlackboardAsset;

    if (BlackboardPtr != nullptr)
    {
        BlackboardPtr->InitializeBlackboard(*InBlackboardAsset);
    }
}

void USpawnQueryContext::SetSeed(int32 Seed)
{
    RandomSeed = Seed;
    RandomStream = FRandomStream(Seed);
}

void USpawnQueryContext::PushCall(USpawnQuery* Query)
{
    QueryCallStack.Add(Query);
}

void USpawnQueryContext::PopCall(USpawnQuery* Query)
{
    int32 Num = QueryCallStack.Num();
    if (Num > 0 && QueryCallStack[Num - 1] == Query)
    {
        QueryCallStack.RemoveAt(Num - 1);
    }
}

bool USpawnQueryContext::HasQueryInCallStack(USpawnQuery* Query) const
{
    return QueryCallStack.Contains(Query);
}

FString USpawnQueryContext::GetCallStackInfo()
{
    FString CallStackInfo;
    for (int32 i = 0; i < QueryCallStack.Num(); ++i)
    {
        CallStackInfo += QueryCallStack[i]->GetName();
        CallStackInfo += TEXT("\n");
    }
    return CallStackInfo;
}

int32 USpawnQueryContext::GetWorldID() const
{
    UWorld* World = GetWorld();
    if (World == nullptr)
    {
        World = GetBlackboardRef().GetWorld();
    }

    return World->GetUniqueID();
}

UObject* USpawnQueryContext::GetStateObjectInternal(UObject* Owner, UClass* StateObjectClass)
{
    if (!Owner)
    {
        UE_LOG(LogTemp, Warning, TEXT("UContext::GetStateObjectInternal: Owner is null."));
        return nullptr;
    }

    UObject* FoundObject = StateObjectMap.FindRef(Owner);

    if (FoundObject)
    {
        // If an object is found, check if it's compatible with the expected class
        if (FoundObject->IsA(StateObjectClass))
        {
            return FoundObject;
        }
        else
        {
            UE_LOG(LogSpawnQuery, Error, TEXT("UContext::GetStateObjectInternal: Found state object for Owner %s is of type %s, but %s was expected. Returning null."),
                *Owner->GetName(), *FoundObject->GetClass()->GetName(), *StateObjectClass->GetName());
            return nullptr;
        }
    }
    else
    {
        // Object not found, create a new one
        if (UObject* NewStateObject = NewObject<UObject>(this, StateObjectClass))
        {
            StateObjectMap.Add(Owner, NewStateObject);
            return NewStateObject;
        }
    }
    return nullptr;
}

void USpawnQueryContext::CreateActor() const
{
    UWorld* World = GetWorld();

    if (World == nullptr)
    {
        World = GetRelevantWorld();
    }

    if (!World)
    {
        UE_LOG(LogSpawnQuery, Error, TEXT("USpawnQueryContext %s: No valid UWorld found to get or create Blackboard Component."), *GetName());
    }

    if (!((GIsEditor && World->IsPlayInEditor()) || (!GIsEditor)/* is playing */))
    {
        UE_LOG(LogSpawnQuery, Error, TEXT("USpawnQueryContext %s: Not in PIE or a cooked build. Cannot create Blackboard Component."), *GetName());
    }

    
    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    SpawnParams.ObjectFlags |= RF_Transient; // Mark as transient to avoid saving with map if not intended

    // Spawn the actor
    ASpawnQueryContextActor* NewBlackboardHolderActor = World->SpawnActor<ASpawnQueryContextActor>(FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);

    if (NewBlackboardHolderActor)
    {
        NewBlackboardHolderActor->SetActorLabel(GetName() + "_Actor");
        BlackboardPtr = NewBlackboardHolderActor->GetBlackboardComponent();

        if (BlackboardAsset != nullptr)
        {
            BlackboardPtr->InitializeBlackboard(*BlackboardAsset);
        }
    } else
    {
        UE_LOG(LogSpawnQuery, Error, TEXT("USpawnQueryContext %s: Failed to spawn SpawnQueryContextActor."), *GetName());
    }
}

UWorld* USpawnQueryContext::GetRelevantWorld() const
{
    UWorld* BestWorld = nullptr;

    for (auto WorldContext : GEngine->GetWorldContexts())
    {
        // If it's a game world, try and set BestWorld. If World() is null, this won't do anything
        if (WorldContext.WorldType == EWorldType::Game)
        {
            return WorldContext.World();
        }
#if WITH_EDITOR
        // This is a PIE world, PIE instance is set, and it matches this world
        else if (WorldContext.WorldType == EWorldType::PIE)
        {
            // We don't examine UE::GetPlayInEditorID() as this might called too early when the ID is not yet initialized
            BestWorld = WorldContext.World();
        }
#endif
    }

    return BestWorld;
}
