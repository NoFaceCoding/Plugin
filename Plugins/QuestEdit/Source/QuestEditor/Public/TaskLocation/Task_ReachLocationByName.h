//Copyright 2020 Marchetti S. Alfredo I. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"


#include "TaskLocation/BaseQuestTask_Location.h"


#include "Task_ReachLocationByName.generated.h"




/**
 * This is the basic object for reach location by name tasks. 
 */

UCLASS(BlueprintType, Meta = (DisplayName = "ReachLocationByNameTask"))
class QUESTEDITOR_API UTask_ReachLocationByName : public UBaseQuestTask_Location
{
	GENERATED_BODY()

public:

	UTask_ReachLocationByName();


	//--------------------------------------------Functions

protected:


	/**Overrideable function with the condition that has to be met in order to commit this task succesfully.*/
		bool  OnQuestLocationReachedCondition_Implementation(ABaseQuestLocationActor* QuestLocationActorIn) override;


	//--------------------------------------------------------------------------------------------
	//------------------------------------------Quest Task Interface
	//--------------------------------------------------------------------------------------------
	/*Get announce UI configuration*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Quest Task")
		bool TaskShouldAnnounce() const;
	virtual bool TaskShouldAnnounce_Implementation() const override;

	/*Get if the task should be shown in UI or not*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Quest Task")
		bool TaskShouldShowInUI() const;
	virtual bool TaskShouldShowInUI_Implementation() const override;

	/*Get task short description*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Quest Task")
		FString GetTaskShortDescription(const FQuestTaskPayload& PayloadIn, const TArray <FString>& CustomPayloadDataIn) const;
	virtual FString GetTaskShortDescription_Implementation(const FQuestTaskPayload& PayloadIn, const TArray <FString>& CustomPayloadDataIn) const override;

	/*Get task name for visual purposes*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Quest Task")
		FString GetTaskName() const;
	virtual FString GetTaskName_Implementation() const override;
};
