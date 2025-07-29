#include "SpawnQueryTypes.h"


#define LOCTEXT_NAMESPACE "SpawnQueryGenerator"

//----------------------------------------------------------------------//
// USpawnQueryTypes
//----------------------------------------------------------------------//
FText USpawnQueryTypes::GetShortTypeName(const UObject* Ob)
{
    if (Ob == nullptr)
    {
        return LOCTEXT("Unknown", "unknown");
    }

    const UClass* ObClass = Ob->IsA(UClass::StaticClass()) ? (const UClass*)Ob : Ob->GetClass();
    check(ObClass != nullptr);
    if (ObClass->HasAnyClassFlags(CLASS_CompiledFromBlueprint))
    {
        return FText::FromString(ObClass->GetName().LeftChop(2));
    }

    FString TypeDesc = ObClass->GetName();
    const int32 ShortNameIdx = TypeDesc.Find(TEXT("_"), ESearchCase::CaseSensitive);
    if (ShortNameIdx != INDEX_NONE)
    {
        TypeDesc.MidInline(ShortNameIdx + 1, MAX_int32, EAllowShrinking::No);
    }

    for (int32 i = 1; i < TypeDesc.Len(); ++i)
    {
        if (FChar::IsUpper(TypeDesc[i]))
        {
            TypeDesc.InsertAt(i, TEXT(" "));
            ++i;
        }
    }

    return FText::FromString(TypeDesc);
}

#undef LOCTEXT_NAMESPACE
