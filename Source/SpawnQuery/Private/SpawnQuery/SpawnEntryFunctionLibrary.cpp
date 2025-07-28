#include "SpawnQuery/SpawnEntryFunctionLibrary.h"

#include "Blueprint/BlueprintExceptionInfo.h"

bool USpawnEntryFunctionLibrary::GetSpawnEntryRow(USpawnEntryRowHandle* Entry, FSpawnEntryTableRowBase& OutRow)
{
    // We should never arrive here. Just a stub.
    check(0);
    return false;
}

DEFINE_FUNCTION(USpawnEntryFunctionLibrary::execGetSpawnEntryRow)
{
    P_GET_OBJECT(USpawnEntryRowHandle, Entry);

    Stack.StepCompiledIn<FStructProperty>(nullptr);
    void* OutRowPtr = Stack.MostRecentPropertyAddress;

    P_FINISH;

    bool bSuccess = false;
    FStructProperty* StructProp = CastField<FStructProperty>(Stack.MostRecentProperty);

    if (!Entry)
    {
        FBlueprintExceptionInfo ExceptionInfo(
            EBlueprintExceptionType::AccessViolation,
            NSLOCTEXT("GetSpawnEntryRow", "MissingEntryRowHandleInput", "Failed to resolve the row handle input. Be sure the SpawnEntryRowHandle is valid.")
        );
        FBlueprintCoreDelegates::ThrowScriptException(P_THIS, Stack, ExceptionInfo);
    }
    else if (OutRowPtr)
    {
        const UScriptStruct* OutputType = StructProp->Struct;
        const UScriptStruct* EntryRowType = Entry->GetRowStruct();

        if (OutputType == EntryRowType)
        {
            P_NATIVE_BEGIN;
            bSuccess = Generic_GetSpawnEntryRow(Entry, OutRowPtr);
            P_NATIVE_END;
        } else
        {
            FBlueprintExceptionInfo ExceptionInfo(
                EBlueprintExceptionType::AccessViolation,
                FText::Format(NSLOCTEXT("GetSpawnEntryRow", "IncompatibleProperty", "Incompatible output parameter; the data table row's type ({0}) is not the same as the return type ({1})."), FText::AsCultureInvariant(GetPathNameSafe(EntryRowType)), FText::AsCultureInvariant(OutputType->GetPathName()))
                );
            FBlueprintCoreDelegates::ThrowScriptException(P_THIS, Stack, ExceptionInfo);
        }
    }
    else
    {
        FBlueprintExceptionInfo ExceptionInfo(
            EBlueprintExceptionType::AccessViolation,
            NSLOCTEXT("GetSpawnEntryRow", "MissingOutputProperty", "Failed to resolve the output parameter for GetSpawnEntryRow.")
        );
        FBlueprintCoreDelegates::ThrowScriptException(P_THIS, Stack, ExceptionInfo);
    }

    *static_cast<bool*>(RESULT_PARAM) = bSuccess;
}

bool USpawnEntryFunctionLibrary::Generic_GetSpawnEntryRow(const USpawnEntryRowHandle* Entry, void* OutRowPtr)
{
    const UScriptStruct* StructType = Entry->GetRowStruct();

    void* RowPtr = Entry->GetTableRow();
    if (StructType)
    {
        StructType->CopyScriptStruct(OutRowPtr, RowPtr);
        return true;
    }

    return false;
}
