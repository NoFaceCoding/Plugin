//Copyright 2020 Marchetti S. Alfredo I. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityWidget.h"
#include "GameplayTagContainer.h"
#include "Quest/QuestTypes.h"


#include "BasePayloadSelectorWidget.generated.h"

//----------------------------------------------DISPATCHERS DEFINITION
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPayloadSelectedFloat, float, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPayloadSelectedInt, int, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPayloadSelectedString, FString, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPayloadSelectedObjectClass, TSoftClassPtr<UObject>, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPayloadSelectedVector, FVector, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPayloadSelectedTag, FGameplayTagContainer, NewValue);

/**
 *		Base payload selector widget
 */
UCLASS(Abstract, Blueprintable)
class QUESTEDITORDEV_API UBasePayloadSelectorWidget : public UEditorUtilityWidget
{
	GENERATED_BODY()
	
public:

	UBasePayloadSelectorWidget();

	//------------------------------------------DISPATCHERS------------------------------------------------------
	
	/*Called when Payload StringB value changes*/
	UPROPERTY(BlueprintAssignable, Category = "Payload Update")
		FPayloadSelectedString StringBChanged;

	/*Called when Payload StringA value changes*/
	UPROPERTY(BlueprintAssignable, Category = "Payload Update")
		FPayloadSelectedString StringAChanged;

	/*Called when Payload TimesTocheck value changes*/
	UPROPERTY(BlueprintAssignable, Category = "Payload Update")
		FPayloadSelectedInt TimesTocheckChanged;

	/*Called when Payload Amount value changes*/
	UPROPERTY(BlueprintAssignable, Category = "Payload Update")
		FPayloadSelectedFloat AmountChanged;

	/*Called when Payload Required Tags value changes*/
	UPROPERTY(BlueprintAssignable, Category = "Payload Update")
		FPayloadSelectedTag RequiredTagsChanged;

	/*Called when Payload Ignored Tags value changes*/
	UPROPERTY(BlueprintAssignable, Category = "Payload Update")
		FPayloadSelectedTag IgnoredTagsChanged;

	/*Called when Payload Object Class value changes*/
	UPROPERTY(BlueprintAssignable, Category = "Payload Update")
		FPayloadSelectedObjectClass ObjectClassChanged;

	/*Called when Payload Vector value changes*/
	UPROPERTY(BlueprintAssignable, Category = "Payload Update")
		FPayloadSelectedVector VectorChanged;


	//------------------------------------------FUNCTIONS-----------------------------------------------------
	/**
	*	Handles all logic for selectors after spawn.
	*	Dispatches all events for payload change.
	*	Must be called after selector widgets are spawned.
	*/
	UFUNCTION(BlueprintCallable, Category = "Selector")
		void HandleWidgetSelectorsStart(TArray <UWidget*> SelectorWidgets);

	//---------------------------------*Callbacsk*/
	UFUNCTION(BlueprintCallable, Category = "Selector")
		void SelectedFloatCallback(float NewValueIn, int SlotIn);

	UFUNCTION(BlueprintCallable, Category = "Selector")
		void SelectedStringCallback(FString NewValueIn, int SlotIn);

	UFUNCTION(BlueprintCallable, Category = "Selector")
		void SelectedObjectClassCallback(TSoftClassPtr<UObject> NewValueIn, int SlotIn);

	UFUNCTION(BlueprintCallable, Category = "Selector")
		void SelectedVectorCallback(FVector NewValueIn, int SlotIn);

	UFUNCTION(BlueprintCallable, Category = "Selector")
		void SelectedTagCallback(FGameplayTagContainer NewValueIn, int SlotIn);


};
