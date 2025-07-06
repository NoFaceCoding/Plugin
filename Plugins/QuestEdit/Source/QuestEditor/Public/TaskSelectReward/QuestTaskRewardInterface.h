//Copyright 2020 Marchetti S. Alfredo I. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "Quest/QuestTypes.h"

#include "QuestTaskRewardInterface.generated.h"


class UBaseQuestRewardSlot;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UQuestTaskRewardInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 *	Interface for quest tasks that handle reward selection.
 */
class QUESTEDITOR_API IQuestTaskRewardInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	
	/**
	*	Determines if the task should show only once in summary even if there are many of it active at the same time.
	*	Used in choose reward.
	*	The string ID helps identify the task type. Should be unique per each task type. For Example: ItemReward.
	*	All tasks with same ID will be shown only once in the summary is there are more than one active at the same time.
	*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Quest Task Rewards Interface")
		bool TaskShowsOnlyOnceInSummary(const FQuestTaskPayload& PayloadIn, FString& StringID) const;

	/**
	*	Determines if the task has a reward giver and if found, returns the definition.
	*	By default the reward tasks use stringA of payload for the NPCDef.
	*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Quest Task Rewards Interface")
		bool TaskRewardGiver(const FQuestTaskPayload& PayloadIn, FNPCQuestDefRep& RewardGiverOut) const;

	/**
	*	Determines if the task has a location reward giver and if found, returns the location name.
	*	By default the reward tasks use stringA of payload for the location name.
	*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Quest Task Rewards Interface")
		bool TaskRewardLocation(const FQuestTaskPayload& PayloadIn, FString& LocationNameOut) const;

	/**
	*	Determines the task reward slot used in UI.
	*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Quest Task Rewards Interface")
		TSubclassOf<UBaseQuestRewardSlot> TaskRewardSlot() const;

	/**
	*	Add all additional tags that you want to have for this reward task. 
	*	Useful to distinguish between different reward tasks, for example, when notifying the player that a reward has been given.
	*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Quest Task Rewards Interface")
		FGameplayTagContainer TaskRewardTags() const;
};
