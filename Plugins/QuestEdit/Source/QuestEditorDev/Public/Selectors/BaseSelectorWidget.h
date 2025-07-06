//Copyright 2020 Marchetti S. Alfredo I. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityWidget.h"
#include "GameplayTagContainer.h"
#include "Engine/DataTable.h"
#include "Quest/QuestTypes.h"

#include "BaseSelectorWidget.generated.h"

/**
 *	Base widget that contains logic for all payload selectors
 */

 //------------------------------------------DISPATCHERS DEFINITIONS-----------------------------------------------------
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSelectedFloat, float, NewValue, int, Slot);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSelectedString, FString, NewValue, int, Slot);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSelectedObjectClass, TSoftClassPtr<UObject>, NewValue, int, Slot);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSelectedVector, FVector, NewValue, int, Slot);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSelectedTag, FGameplayTagContainer, NewValue, int, Slot);




UCLASS(Abstract)
class QUESTEDITORDEV_API UBaseSelectorWidget : public UEditorUtilityWidget
{
	GENERATED_BODY()
	
public:
	//------------------------------------------DISPATCHERS------------------------------------------------------

	/*Called when a selector commits float value*/
	UPROPERTY(BlueprintAssignable, Category = "Selector")
		FSelectedFloat SelectedFloat;

	/*Called when a selector commits string value*/
	UPROPERTY(BlueprintAssignable, Category = "Selector")
		FSelectedString SelectedString;

	/*Called when a selector commits object class value*/
	UPROPERTY(BlueprintAssignable, Category = "Selector")
		FSelectedObjectClass SelectedObjectClass;

	/*Called when a selector commits vector value*/
	UPROPERTY(BlueprintAssignable, Category = "Selector")
		FSelectedVector SelectedVector;

	/*Called when a selector commits tag value*/
	UPROPERTY(BlueprintAssignable, Category = "Selector")
		FSelectedTag SelectedTag;

	//------------------------------------------VARIABLES------------------------------------------------------

	/**
	*	Indicates the variable on the payload.
	*	Slot 0 is the first variable of type that appears on the payload. The followings increasing in number.
	*	Some variables in the payload are duplicated, for example there are two string variables.
	*	Use to update from payload.
	*	Its also used for int as it was float. "AmountInvolved" is slot 0 and "TimesToCheck" is slot 1 even when is a int value.
	*/
	UPROPERTY(meta = (ExposeOnSpawn="true"), BlueprintReadOnly, Category = "Selector")
		int SelectorSlot = 0;

	/**
	*	Array of datatables of the selector. Used when the selector is choosing from diferent tables.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Selector")
		TArray <UDataTable*> Datatables;

	//------------------------------------------Functions------------------------------------------------------

	/**
	*	Gets information of the current datatable being used by the selector.
	*	Not needed for most selectors.
	*/
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Selector")
		void GetDTVariables(FName& DTNameOUT, TSoftObjectPtr <UDataTable>& DTPathOUT);

	/**
	*	Finds a DT from the datatable array that matches the object name
	*	If finds none, returns first of array.
	*/
	UFUNCTION(BlueprintCallable, Category = "Selector")
		UDataTable* FindDTFromName(const FString& DTObjectNameIn);

	/**
	*	Finds a DT from the datatable array that matches the object path name
	*	If finds none, returns first of array.
	*/
	UFUNCTION(BlueprintCallable, Category = "Selector")
		UDataTable* FindDTFromPath(const TSoftObjectPtr <UDataTable>& DTObjectPathIn);

	/**
	*	Main update event for the selector.
	*	All the logic needed for updating this widget should be placed here.
	*	All selectors must override this function.
	*/
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Selector")
		void UpdateFromTask(const FQuestTask& QuestTaskIn);

};
