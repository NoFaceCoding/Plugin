//Copyright 2020 Marchetti S. Alfredo I. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "Task/QuestTaskInterface.h"
#include "TaskSelectReward/QuestTaskRewardInterface.h"

#include "Task/BaseQuestTask.h"

#include "BaseQuestTask_SelectReward.generated.h"





/**
 * This is the basic object for any reward task for quests. 
 */

UCLASS(Abstract, Blueprintable, Meta = (DisplayName = "BaseSelectRewardTask"))
class QUESTEDITOR_API UBaseQuestTask_SelectReward : public UBaseQuestTask, public IQuestTaskInterface, public IQuestTaskRewardInterface
{
	GENERATED_BODY()

public:

	UBaseQuestTask_SelectReward(); 

protected:

	//--------------------------------------------Variables
	/**
	*	This is the quest node on creation. 
	*	This has to be calculated on activate and never modified. 
	*	Used to check if this is the correct task since quest slot is insufficient when multirewards
	*/
	UPROPERTY()
		FQuestNode QuestNode;

	//--------------------------------------------Functions
	
	//-------------------------------Select reward overrides

	/**
	*	Condition to check before calling the UI for showind rewards.
	*	Provides extra control.
	*	This is called before "OnRewardSelected". 
	*/
	UFUNCTION(BlueprintNativeEvent, Category = "Base Quest Task Reward")
		bool OnRewardSelectedCondition();

	/**
	*	Overrideable function that is called when the player selects a reward.
	*	You should use this function to give the reward to the player.
	*	If the reward give fails, you should return a false so the task can provide the player with the selection again.
	*/
	UFUNCTION(BlueprintNativeEvent, Category = "Base Quest Task Reward")
		bool OnRewardSelected(int QuestSlotIn, int TaskSlotIn, bool IsDialogIn);

	/**
	*	Called to bind to event dispatchers to handle all the logic.
	*/
	UFUNCTION(BlueprintNativeEvent, Category = "Base Quest Task Reward")
		void BindQuestEvents();


	//-----------------Function handlers

	/*This handles the give reward the override and the task end if the reward given is succesful*/
	UFUNCTION()
		void HandleRewardSelection(const FQuestNode& QuestNodeIn, bool IsDialogTaskIn);


	//-----------------------------Base Quest Task Overide

	/*Logic to bind and reach locations*/
	void  OnQuestTaskActivated_Implementation() override;

	/**Overrideable function with the task logic when loading the game. Will only execute for loaded Active tasks.*/
	void  OnQuestTaskLoaded_Implementation() override;

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

	/**
	*	Determines if the task should show only once in summary even if there are many of it active at the same time.
	*	Used in choose reward.
	*	The string ID helps identify the task type. Should be unique per each task. For Example: ItemReward.
	*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Quest Task")
		bool TaskShowsOnlyOnceInSummary(const FQuestTaskPayload& PayloadIn, FString& StringID) const;
	virtual	bool TaskShowsOnlyOnceInSummary_Implementation(const FQuestTaskPayload& PayloadIn, FString& StringID) const override;

	/**
	*	Determines if the task has a reward giver and if found, returns the definition.
	*	By default the reward tasks use stringA of payload for the NPCDef.
	*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Quest Task Rewards Interface")
		bool TaskRewardGiver(const FQuestTaskPayload& PayloadIn, FNPCQuestDefRep& RewardGiverOut) const;
	virtual	bool TaskRewardGiver_Implementation(const FQuestTaskPayload& PayloadIn, FNPCQuestDefRep& RewardGiverOut) const override;
};
