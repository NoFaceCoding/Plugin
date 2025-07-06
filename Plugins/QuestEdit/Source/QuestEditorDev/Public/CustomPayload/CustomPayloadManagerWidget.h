//Copyright 2020 Marchetti S. Alfredo I. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityWidget.h"
#include "CustomPayloadManagerWidget.generated.h"


//----------------------------------------------DISPATCHERS DEFINITION
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FCustomPayloadDataChanged, FString, NewValue, int, Slot);

/**
 *	Manager for all custom payload widgets
 */

UCLASS()
class QUESTEDITORDEV_API UCustomPayloadManagerWidget : public UEditorUtilityWidget
{
	GENERATED_BODY()

	//------------------------------------------DISPATCHERS------------------------------------------------------

	/*Called when Payload StringB value changes*/
	UPROPERTY(BlueprintAssignable, Category = "Payload Update")
		FCustomPayloadDataChanged CustomPayloadDataChanged;
	
	

public:
	//------------------------------------------FUNCTIONS-----------------------------------------------------
	/**
	*	Handles all logic for selectors after spawn.
	*	Dispatches all events for custom payload change.
	*	Must be called after selector widgets are spawned.
	*/

	UFUNCTION(BlueprintCallable, Category = "Selector")
		void HandleWidgetSelectorsStart(TArray <UWidget*> SelectorWidgets);
	
	/*Callback for data change*/
	UFUNCTION(BlueprintCallable, Category = "Selector")
		void SelectedDataCallback(FString NewValueIn, int SlotIn);
	

};
