#include "K2Node_GetSpawnEntryRow.h"

#include "BlueprintActionDatabaseRegistrar.h"
#include "BlueprintNodeSpawner.h"
#include "DataTableEditorUtils.h"
#include "EditorCategoryUtils.h"
#include "K2Node_CallFunction.h"
#include "K2Node_IfThenElse.h"
#include "KismetCompiler.h"
#include "Kismet/DataTableFunctionLibrary.h"
#include "SpawnQuery/SpawnEntryFunctionLibrary.h"
#include "SpawnQuery/Pools/SpawnEntryTableRow.h"

#define LOCTEXT_NAMESPACE "UK2Node_GetSpawnEntryRow"

namespace GetSpawnEntryRowHelper
{
    const FName SpawnEntryPinName = "SpawnEntryRowHandle";
    const FName RowNotFoundPinName = "RowNotFound";
    const FName RowNamePinName = "RowName";
}

void UK2Node_GetSpawnEntryRow::PostLoad()
{
    Super::PostLoad();

    SpawnEntryPin = FindPinChecked(GetSpawnEntryRowHelper::SpawnEntryPinName);
    ResultPin = FindPin(UEdGraphSchema_K2::PN_ReturnValue);
}

FText UK2Node_GetSpawnEntryRow::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
    if (TitleType == ENodeTitleType::MenuTitle)
    {
        return LOCTEXT("ListViewTitle", "Get Spawn Entry Row");
    }
    else if (SpawnEntryPin)
    {
        if (SpawnEntryPin->LinkedTo.Num())
        {
            return NSLOCTEXT("K2Node", "SpawnEntry_Title_Unknown", "Get Spawn Entry Row");
        }
        else
        {
            USpawnEntryRowHandle* Entry = Cast<USpawnEntryRowHandle>(SpawnEntryPin->DefaultObject);
            if (Entry == nullptr)
            {
                return NSLOCTEXT("K2Node", "SpawnEntry_Title_None", "Get Spawn Entry Row NONE");
            }
            else if (CachedNodeTitle.IsOutOfDate(this))
            {
                FFormatNamedArguments Args;
                Args.Add(TEXT("SpawnEntryTableName"), FText::FromString(Entry->GetRowStruct()->GetName()));

                FText LocFormat = NSLOCTEXT("K2Node", "SpawnEntry", "Get Spawn Entry Row {SpawnEntryTableName}");
                // FText::Format() is slow, so we cache this to save on performance
                CachedNodeTitle.SetCachedText(FText::Format(LocFormat, Args), this);
            }
        }
    }
    else
    {
        return NSLOCTEXT("K2Node", "SpawnEntry_Title_None", "Get Spawn Entry Row NONE");
    }

    return CachedNodeTitle;
}

FText UK2Node_GetSpawnEntryRow::GetTooltipText() const
{
    return LOCTEXT("NodeTooltip", "Attempts to get a SpawnEntryTableRow from USpawnEntryBase class");
}

void UK2Node_GetSpawnEntryRow::AllocateDefaultPins()
{
    const UEdGraphSchema_K2* K2Schema = GetDefault<UEdGraphSchema_K2>();
    
    // Add execution pins
    CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Exec, UEdGraphSchema_K2::PN_Execute);
    UEdGraphPin* RowFoundPin = CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Exec, UEdGraphSchema_K2::PN_Then);
    RowFoundPin->PinFriendlyName = LOCTEXT("GetSpawnEntryRow Row Found Exec pin", "Row Found");
    CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Exec, GetSpawnEntryRowHelper::RowNotFoundPinName);

    // Add SpawnEntry pin
    SpawnEntryPin = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Object, USpawnEntryRowHandle::StaticClass(), GetSpawnEntryRowHelper::SpawnEntryPinName);
    SpawnEntryPin->bDefaultValueIsIgnored = true;

    // Result pin
    ResultPin = CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Wildcard, UEdGraphSchema_K2::PN_ReturnValue);
    ResultPin->PinFriendlyName = LOCTEXT("GetSpawnEntryRow Output Row", "Out Row");

    Super::AllocateDefaultPins();
}

void UK2Node_GetSpawnEntryRow::PinDefaultValueChanged(UEdGraphPin* ChangedPin)
{
    if (ChangedPin && ChangedPin->PinName == GetSpawnEntryRowHelper::SpawnEntryPinName)
    {
        RefreshOutputPinType();
    }
}

void UK2Node_GetSpawnEntryRow::ExpandNode(class FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph)
{
    Super::ExpandNode(CompilerContext, SourceGraph);

    UEdGraphPin* OriginalEntryPin = SpawnEntryPin;
    USpawnEntryRowHandle* Entry = SpawnEntryPin ? Cast<USpawnEntryRowHandle>(OriginalEntryPin->DefaultObject) : nullptr;

    if (OriginalEntryPin == nullptr)
    {
        CompilerContext.MessageLog.Error(*LOCTEXT("GetSpawnEntryRowText_Error", "Test Error").ToString(), this);
        // we break exec links so this is the only error we get
        BreakAllNodeLinks();
        return;
    }

    if (OriginalEntryPin == nullptr || (OriginalEntryPin->LinkedTo.Num() == 0 && Entry == nullptr))
    {
        CompilerContext.MessageLog.Error(*LOCTEXT("GetSpawnEntryRowNoHandle_Error", "GetSpawnEntryRow must have a SpawnEntryRowHandle specified.").ToString(), this);
        // we break exec links so this is the only error we get
        BreakAllNodeLinks();
        return;
    }

    // FUNCTION NODE
    const FName FunctionName = GET_FUNCTION_NAME_CHECKED(USpawnEntryFunctionLibrary, GetSpawnEntryRow);
    UK2Node_CallFunction* GetSpawnEntryRowFunction = CompilerContext.SpawnIntermediateNode<UK2Node_CallFunction>(this, SourceGraph);
    GetSpawnEntryRowFunction->FunctionReference.SetExternalMember(FunctionName, USpawnEntryFunctionLibrary::StaticClass());
    GetSpawnEntryRowFunction->AllocateDefaultPins();
    CompilerContext.MovePinLinksToIntermediate(*GetExecPin(), *(GetSpawnEntryRowFunction->GetExecPin()));

    // Connect the entry input to the function's input pin
    UEdGraphPin* EntryInPin = GetSpawnEntryRowFunction->FindPinChecked(TEXT("Entry"));

    if (OriginalEntryPin->LinkedTo.Num())
    {
        CompilerContext.MovePinLinksToIntermediate(*OriginalEntryPin, *EntryInPin);
    }
    else
    {
        EntryInPin->DefaultObject = OriginalEntryPin->DefaultObject;
    }

    // Connect the output from the function's output pin
    UEdGraphPin* OriginalOutRowPin = FindPinChecked(UEdGraphSchema_K2::PN_ReturnValue);
    UEdGraphPin* FunctionOutRowPin = GetSpawnEntryRowFunction->FindPinChecked(TEXT("OutRow"));

    FunctionOutRowPin->PinType = OriginalOutRowPin->PinType;
    FunctionOutRowPin->PinType.PinSubCategoryObject = OriginalOutRowPin->PinType.PinSubCategoryObject;
    CompilerContext.MovePinLinksToIntermediate(*OriginalOutRowPin, *FunctionOutRowPin);

    // Connect the out execution pins
    UEdGraphPin* FunctionReturnPin = GetSpawnEntryRowFunction->FindPinChecked(UEdGraphSchema_K2::PN_ReturnValue);
    UEdGraphPin* FunctionThenPin = GetSpawnEntryRowFunction->GetThenPin();
    //BRANCH NODE
    UK2Node_IfThenElse* BranchNode = CompilerContext.SpawnIntermediateNode<UK2Node_IfThenElse>(this, SourceGraph);
    BranchNode->AllocateDefaultPins();
    // Hook up inputs to branch
    FunctionThenPin->MakeLinkTo(BranchNode->GetExecPin());
    FunctionReturnPin->MakeLinkTo(BranchNode->GetConditionPin());
    // Hook up outputs
    CompilerContext.MovePinLinksToIntermediate(*GetThenPin(), *(BranchNode->GetThenPin()));
    CompilerContext.MovePinLinksToIntermediate(*FindPinChecked(GetSpawnEntryRowHelper::RowNotFoundPinName), *(BranchNode->GetElsePin()));

    BreakAllNodeLinks();
}

FSlateIcon UK2Node_GetSpawnEntryRow::GetIconAndTint(FLinearColor& OutColor) const
{
    OutColor = GetNodeTitleColor();
    static FSlateIcon Icon(FAppStyle::GetAppStyleSetName(), "Kismet.AllClasses.FunctionIcon");
    return Icon;
}

void UK2Node_GetSpawnEntryRow::PostReconstructNode()
{
    Super::PostReconstructNode();

    RefreshOutputPinType();
}

void UK2Node_GetSpawnEntryRow::GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const
{
    UClass* ActionKey = GetClass();
    if (ActionRegistrar.IsOpenForRegistration(ActionKey))
    {
        UBlueprintNodeSpawner* NodeSpawner = UBlueprintNodeSpawner::Create(GetClass());
        check(NodeSpawner != nullptr);

        ActionRegistrar.AddBlueprintAction(ActionKey, NodeSpawner);
    }
}

FText UK2Node_GetSpawnEntryRow::GetMenuCategory() const
{
    return FEditorCategoryUtils::GetCommonCategory(FCommonEditorCategory::Utilities);
}

void UK2Node_GetSpawnEntryRow::EarlyValidation(class FCompilerResultsLog& MessageLog) const
{
    Super::EarlyValidation(MessageLog);
    
    if (!SpawnEntryPin)
    {
        MessageLog.Error(*LOCTEXT("MissingPins", "Missing pins in @@").ToString(), this);
        return;
    }

    if (SpawnEntryPin->LinkedTo.Num() == 0)
    {
        const USpawnEntryRowHandle* RowHandle = Cast<USpawnEntryRowHandle>(SpawnEntryPin->DefaultObject);
        if (!RowHandle)
        {
            MessageLog.Error(*LOCTEXT("NoRowHandle", "No RowHandle in @@").ToString(), this);
            return;
        }
    }
}

void UK2Node_GetSpawnEntryRow::PreloadRequiredAssets()
{
    if (SpawnEntryPin)
    {
        if (USpawnEntryRowHandle* RowHandle = Cast<USpawnEntryRowHandle>(SpawnEntryPin->DefaultObject))
        {
            // make sure to properly load the data-table object so that we can 
            // farm the "RowStruct" property from it (below, in GetDataTableRowStructType)
            PreloadObject(RowHandle);
        }
    }
    return Super::PreloadRequiredAssets();
}

void UK2Node_GetSpawnEntryRow::ReallocatePinsDuringReconstruction(TArray<UEdGraphPin*>& OldPins)
{
    Super::ReallocatePinsDuringReconstruction(OldPins); // will call `AllocateDefaultPins()`

    if (SpawnEntryPin)
    {
        if (USpawnEntryRowHandle* RowHandle = Cast<USpawnEntryRowHandle>(SpawnEntryPin->DefaultObject))
        {
            // make sure to properly load the data-table object so that we can 
            // farm the "RowStruct" property from it (below, in GetDataTableRowStructType)
            PreloadObject(RowHandle);
        }
    }
}

bool UK2Node_GetSpawnEntryRow::IsConnectionDisallowed(const UEdGraphPin* MyPin, const UEdGraphPin* OtherPin,
    FString& OutReason) const
{
    if (MyPin == ResultPin && MyPin->PinType.PinCategory == UEdGraphSchema_K2::PC_Wildcard)
    {
        bool bDisallowed = true;
        if (OtherPin->PinType.PinCategory == UEdGraphSchema_K2::PC_Struct)
        {
            if (UScriptStruct* ConnectionType = Cast<UScriptStruct>(OtherPin->PinType.PinSubCategoryObject.Get()))
            {
                bDisallowed = !FDataTableEditorUtils::IsValidTableStruct(ConnectionType);
            }
        }
        else if (OtherPin->PinType.PinCategory == UEdGraphSchema_K2::PC_Wildcard)
        {
            bDisallowed = false;
        }

        if (bDisallowed)
        {
            OutReason = TEXT("Must be a struct that can be used in a DataTable");
        }
        return bDisallowed;
    }
    return false;
}

void UK2Node_GetSpawnEntryRow::NotifyPinConnectionListChanged(UEdGraphPin* Pin)
{
    Super::NotifyPinConnectionListChanged(Pin);

    if (Pin == ResultPin)
    {
        // this connection would only change the output type if the table pin is undefined
        const bool bIsTypeAuthority = SpawnEntryPin->LinkedTo.Num() == 0 && SpawnEntryPin->DefaultObject == nullptr;
        if (bIsTypeAuthority)
        {
            RefreshOutputPinType();
        }
    }
    else if (Pin == SpawnEntryPin)
    {
        const bool bConnectionAdded = Pin->LinkedTo.Num() > 0;
        if (bConnectionAdded)
        {
            // if the output connection was previously, incompatible, it now becomes the authority on this node's output type
            RefreshOutputPinType();
        }
    }
}

void UK2Node_GetSpawnEntryRow::RefreshOutputPinType()
{
    if (ResultPin == nullptr) return;

    UScriptStruct* OldRowStruct = static_cast<UScriptStruct*>(ResultPin->PinType.PinSubCategoryObject.Get());
    UScriptStruct* NewRowStruct = GetDataTableRowStructType();

    if (NewRowStruct != OldRowStruct)
    {
        if (ResultPin->SubPins.Num())
        {
            GetSchema()->RecombinePin(ResultPin);
        }

        ResultPin->PinType.PinSubCategoryObject = NewRowStruct;
        ResultPin->PinType.PinCategory = (NewRowStruct == nullptr) ? UEdGraphSchema_K2::PC_Wildcard : UEdGraphSchema_K2::PC_Struct;

        CachedNodeTitle.Clear();
    }
}

UScriptStruct* UK2Node_GetSpawnEntryRow::GetDataTableRowStructType() const
{
    UScriptStruct* RowStructType = nullptr;

    if(SpawnEntryPin && SpawnEntryPin->DefaultObject != nullptr && SpawnEntryPin->LinkedTo.Num() == 0)
    {
        if (const USpawnEntryRowHandle* RowHandle = Cast<const USpawnEntryRowHandle>(SpawnEntryPin->DefaultObject))
        {
            RowStructType = RowHandle->GetRowStruct();
        }
    }

    
    if (RowStructType == nullptr)
    {
        if (ResultPin && ResultPin->LinkedTo.Num() > 0)
        {
            RowStructType = Cast<UScriptStruct>(ResultPin->LinkedTo[0]->PinType.PinSubCategoryObject.Get());

            if (RowStructType == nullptr && ResultPin->PinType.PinCategory != UEdGraphSchema_K2::PC_Wildcard)
            {
                RowStructType = GetFallbackStruct();
            }

            // find the deepest descendant in the class hierarchy
            for (int32 LinkIndex = 1; LinkIndex < ResultPin->LinkedTo.Num(); ++LinkIndex)
            {
                UEdGraphPin* Link = ResultPin->LinkedTo[LinkIndex];
                UScriptStruct* LinkType = Cast<UScriptStruct>(Link->PinType.PinSubCategoryObject.Get());

                if (RowStructType && RowStructType->IsChildOf(LinkType))
                {
                    RowStructType = LinkType;
                }
            }
        }
    }

    return RowStructType;
}

#undef LOCTEXT_NAMESPACE
