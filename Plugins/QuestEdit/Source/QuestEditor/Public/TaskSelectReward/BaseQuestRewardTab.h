//Copyright 2020 Marchetti S. Alfredo I. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"

#include "Quest/QuestTypes.h"

#include "BaseQuestRewardTab.generated.h"


class UQuestManager;

/**
 *  Base widget to show and select rewards.
 */
UCLASS(Abstract, BlueprintType, Blueprintable, Meta = (DisplayName = "Reward Tab"))
class QUESTEDITOR_API UBaseQuestRewardTab : public UUserWidget
{
	GENERATED_BODY()
	
public:

		virtual void NativeConstruct() override;
protected:
		//-----------------Variables
		/*Update timer for activation of rewards retriggering*/
		UPROPERTY()
			FTimerHandle UpdateTimer = FTimerHandle();

		/**
		*	Track all the reward tasks that activate in the current timer.
		*	This can be from NPC or location and must be analized in the quest manager to determine wich ones show in UI.
		*	I dont care about object reference here.
		*	Cleared when the tab is updated.
		*/
		UPROPERTY()
			TArray <FRewardTabSlotInfo> ActivatedTasks = TArray <FRewardTabSlotInfo>();

		/*Retrigger value of mode. For now there is only two so a bool is enough. Should convert to enum if more modes are needed*/
		UPROPERTY()
			bool Retrigger_IsNPCReward = false;	

		/*Location name in case is a location mode*/
		UPROPERTY()
			FString Retrigger_LocationName = FString();

		/*NPC def in case is a NPC mode*/
		UPROPERTY()
			FNPCQuestDefRep Retrigger_NPCDef = FNPCQuestDefRep();

public:
		//-----------------Functions
		/**
		*	Overrideable function to get the quest manager reference.
		*	By default considers the manager is in the player state. If you implemented this differently you will need to override it and provide a valid QuestManager reference.
		*/
		UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Reward Slot Tab")
			UQuestManager* GetQuestManagerReference();

protected:

		/*Function to bind to on reward activated and detect if needs to update the widget*/
		UFUNCTION()
			void OnRewardActivateBind(int QuestSlotIn, int TaskSlotIn, bool IsDialogTaskIn);

		/*Function called when a update is needed, after a rewardactivation timer succeds.*/
		UFUNCTION()
			void OnRewardActivateUpdate();

		/**
		*	Function called when a reward is selected.
		*	Handles the tab retrigger mostly.
		*/
		UFUNCTION()
			void OnRewardSelectedUpdate(const FQuestNode& QuestNodeIn, bool IsDialogTaskIn);

		/**
		*	For updating the tab when retriggered.
		*/
		UFUNCTION()
			void UpdateTabWithRetriggerData();

public:
		/**
		*	Main update event for the widget.
		*	All the logic needed for updating this widget should be placed here.
		*	All Rewards Tab Widgets must override this function.
		*/
		UFUNCTION(BlueprintImplementableEvent, Category = "Reward Slot Tab")
			void UpdateRewardTab(const TArray <FRewardTabSlotInfo>& SlotsData);
};
