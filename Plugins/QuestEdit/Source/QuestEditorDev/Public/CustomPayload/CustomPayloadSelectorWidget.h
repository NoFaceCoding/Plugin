//Copyright 2020 Marchetti S. Alfredo I. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityWidget.h"
#include "Quest/QuestTypes.h"


#include "CustomPayloadSelectorWidget.generated.h"

 //------------------------------------------DISPATCHERS DEFINITIONS-----------------------------------------------------
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FStructDataCommited, FString, NewValue, int, Slot);


/**
 *		Base widget that contains logic for all custom payload selectors
 */

UCLASS(Abstract, Meta = (DisplayName = "CustomPayloadSelector"))
class QUESTEDITORDEV_API UCustomPayloadSelectorWidget : public UEditorUtilityWidget
{
	GENERATED_BODY()

public:
	//---------------------------DISPATCHERS

	/*Called when a selector commits float value*/
	UPROPERTY(BlueprintAssignable, Category = "Selector")
	FStructDataCommited OnStructDataCommited;
	
	//------------------------------------------VARIABLES------------------------------------------------------

	/**
	*	Indicates the index on the payload.
	*/
	UPROPERTY(meta = (ExposeOnSpawn = "true"), BlueprintReadOnly, Category = "Selector")
		int SelectorSlot = 0;

	/**
	*	Main update event for the selector.
	*	All the logic needed for updating this widget should be placed here.
	*	All selectors must override this function.
	*/
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Selector")
		void UpdateFromData(const FString& CustomData);

	/**
	*	Selector value commit function.
	*	Must be called when the selected variable changes.
	*/
	UFUNCTION(BlueprintCallable, Category = "Selector", CustomThunk, meta = (CustomStructureParam = "NewSelectorValue"))
		static void SelectorCommitValue(const TFieldPath<FProperty> NewSelectorValue);
	DECLARE_FUNCTION(execSelectorCommitValue)
	{
		//This is a function copied from quest funct library, no point in calling that one
		//Grab struct				
		Stack.MostRecentProperty = nullptr;
		Stack.StepCompiledIn<FStructProperty>(NULL);
		void* StructAddr = Stack.MostRecentPropertyAddress;
		FStructProperty* StructProperty = CastField<FStructProperty>(Stack.MostRecentProperty);

		//Get scripstruct
		UScriptStruct* MyScriptStruct = StructProperty->Struct;

		//Convert to string
		FString StringToStore;
		MyScriptStruct->ExportText(StringToStore, StructAddr, nullptr, nullptr, (PPF_ExportsNotFullyQualified | PPF_Copy | PPF_Delimited | PPF_IncludeTransient), nullptr);
		
		UObject* Outer = Stack.Object;
		if (!Outer) return;

		UCustomPayloadSelectorWidget* OuterWidget = Cast<UCustomPayloadSelectorWidget>(Outer);
		if (OuterWidget)
		{
				OuterWidget->OnStructDataCommited.Broadcast(StringToStore, OuterWidget->SelectorSlot);
		}

		//Commit dispatcher
		P_FINISH;
		P_NATIVE_BEGIN;		
		P_NATIVE_END;
	}

};
