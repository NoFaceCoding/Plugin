//Copyright 2020 Marchetti S. Alfredo I. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"


#include "TaskSelectReward/BaseQuestTask_SelectReward.h"

#include "BaseQuestTask_SelectRewardOnNPC.generated.h"





/**
 * This is the basic object for any reward task on NPC reaching. StringA in payload is reserved for NPC. 
 */

UCLASS(Abstract, Blueprintable, Meta = (DisplayName = "NPCRewardTask"))
class QUESTEDITOR_API UBaseQuestTask_SelectRewardOnNPC : public UBaseQuestTask_SelectReward
{
	GENERATED_BODY()

public:

	UBaseQuestTask_SelectRewardOnNPC();


	//--------------------------------------------Variables


	//--------------------------------------------Functions

protected:

	/*Overrideable function that is called when the NPC is reached. Used in BindQuestEvents*/
	UFUNCTION(BlueprintNativeEvent, Category = "Base Quest Task Reward")
		void OnNPCReachForReward(AActor* NPCIN);

	//-------------------------------Select reward overrides

	/**
	*	Overrideable function that is called when the player selects a reward.
	*	You should use this function to give the reward to the player.
	*	If the reward give fails, you should return a false so the task can provide the player with the selection again.
	*/
		bool OnRewardSelected_Implementation(int QuestSlotIn, int TaskSlotIn, bool IsDialogIn) override;

	/**
	*	Called to bind to event dispatchers to handle all the logic.
	*/
		void BindQuestEvents_Implementation() override;


	//-------------------------------Interface


	virtual FString GetTaskShortDescription_Implementation(const FQuestTaskPayload& PayloadIn, const TArray <FString>& CustomPayloadDataIn) const override;

	virtual	bool TaskShowsOnlyOnceInSummary_Implementation(const FQuestTaskPayload& PayloadIn, FString& StringID) const override;

	virtual	bool TaskRewardGiver_Implementation(const FQuestTaskPayload& PayloadIn, FNPCQuestDefRep& RewardGiverOut) const override;

};
