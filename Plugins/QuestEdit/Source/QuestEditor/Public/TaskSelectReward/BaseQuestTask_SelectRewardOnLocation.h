//Copyright 2020 Marchetti S. Alfredo I. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"


#include "TaskSelectReward/BaseQuestTask_SelectReward.h"

#include "BaseQuestTask_SelectRewardOnLocation.generated.h"


class ABaseQuestLocationActor;


/**
 * This is the basic object for any reward task on location reaching. StringA in payload is reserved for Location name. 
 */

UCLASS(Abstract, Blueprintable, Meta = (DisplayName = "LocationRewardTask"))
class QUESTEDITOR_API UBaseQuestTask_SelectRewardOnLocation : public UBaseQuestTask_SelectReward
{
	GENERATED_BODY()

public:

	UBaseQuestTask_SelectRewardOnLocation();
	//---------------------------------------------------------------------------------------------
	//---------------------------------LOCATION
	//---------------------------------------------------------------------------------------------

	//--------------------------------------------Variables
	/*The current character used for overlap*/
	UPROPERTY()
		ACharacter* CurrentCharacter = nullptr;

	/*Delegate handle for new pawn possesion*/
	FDelegateHandle OnNewPawnHandle = FDelegateHandle();

	//--------------------------------------------Functions

protected:

	/**
	*	Overrideable function to get the character that reaches the quest location
	*	By default is the pawn that is controlled by the player state owner of the quest manager.
	*/
	UFUNCTION(BlueprintNativeEvent, Category = "Base Quest Task Location")
		ACharacter* GetInteractingCharacter();

	/**Overrideable function with the condition that has to be met in order to commit this task succesfully.*/
	UFUNCTION(BlueprintNativeEvent, Category = "Base Quest Task Location")
		bool  OnQuestLocationReachedCondition(ABaseQuestLocationActor* QuestLocationActorIn);

	/*Overrideable function that is called when the Location is reached. Used in BindQuestEvents*/
	UFUNCTION(BlueprintNativeEvent, Category = "Base Quest Task Reward")
		void OnLocationReachForReward(ABaseQuestLocationActor* LocationIN);

	/**Overrideable function with the logic after overlapping, with potentially can be reaching a location.*/
	UFUNCTION()
		virtual void  OnOverlapEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/**Overrideable function with the logic after changing characters.*/
	virtual void  OnNewPawnEvent(APawn* NewPawn);

	/*Function that binds to the character overlap delegates.*/
	UFUNCTION()
		void  BindOverlapCharacterDelegates(ACharacter* NewCharacter);

	//-----------------------------Base Quest Task Overide

	/*Logic to bind and reach locations*/
	void  OnQuestTaskActivated_Implementation() override;

	/**Overrideable function with the task logic when loading the game. Will only execute for loaded Active tasks.*/
	void  OnQuestTaskLoaded_Implementation() override;

	//---------------------------------------------------------------------------------------------
	//-------------------------------REWARD FUNCTIONS
	//---------------------------------------------------------------------------------------------

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


	//---------------------------------------------------------------------------------------------
	//-------------------------------Interface
	//---------------------------------------------------------------------------------------------

	virtual FString GetTaskShortDescription_Implementation(const FQuestTaskPayload& PayloadIn, const TArray <FString>& CustomPayloadDataIn) const override;

	virtual	bool TaskShowsOnlyOnceInSummary_Implementation(const FQuestTaskPayload& PayloadIn, FString& StringID) const override;

	virtual	bool TaskRewardLocation_Implementation(const FQuestTaskPayload& PayloadIn, FString& LocationNameOut) const override;

};
