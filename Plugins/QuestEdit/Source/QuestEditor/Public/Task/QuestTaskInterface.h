//Copyright 2020 Marchetti S. Alfredo I. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Quest/QuestTypes.h"
#include "QuestTaskInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UQuestTaskInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class QUESTEDITOR_API IQuestTaskInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	/*Get task short description*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Quest Task")
		FString GetTaskShortDescription(const FQuestTaskPayload& PayloadIn, const TArray <FString>& CustomPayloadDataIn) const;

	/*Get task name for visual purposes*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Quest Task")
		FString GetTaskName() const;
	
	/*Get announce UI configuration*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Quest Task")
		bool TaskShouldAnnounce() const;

	/*Get if the task should be shown in UI or not*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Quest Task")
		bool TaskShouldShowInUI() const;

	/*Get task color for visual purposes*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Quest Task")
		FLinearColor GetTaskColor() const;

	/*Get task timer. Returns a running timer value if there is one active. Returns -1 if there is no timer in this task.*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Quest Task")
		float GetTaskTimer() const;

};
