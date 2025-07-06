//Copyright 2020 Marchetti S. Alfredo I. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"

#include "Quest/QuestTypes.h"

#include "BaseQuestRewardSlot.generated.h"


class UQuestManager;

/**
 *	
 *  Base widget to show and select rewards.
 */
UCLASS(Abstract, BlueprintType, Blueprintable, Meta = (DisplayName = "Reward Slot"))
class QUESTEDITOR_API UBaseQuestRewardSlot : public UUserWidget
{
	GENERATED_BODY()
	


public:


		virtual void NativeConstruct() override;


		//-----------------Variables
		/*Quest slot of the task in the quest manager.*/

		UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (ExposeOnSpawn = true), Category = "Reward Slot Widget")
			int QuestSlot = 0;

		/*Task slot inside the quest. Task ID*/
		UPROPERTY (EditAnywhere, BlueprintReadOnly, Meta = (ExposeOnSpawn = true), Category = "Reward Slot Widget")
			int TaskSlot = 0;

		/*If the task is a dialog or quest task. */
		UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (ExposeOnSpawn = true), Category = "Reward Slot Widget")
			bool IsDialogTask = false;

		/*Quest task current info. Use to update widget.*/
		UPROPERTY(BlueprintReadOnly, Category = "Reward Slot Widget")
			FQuestTask QuestTask = FQuestTask();

		UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (ExposeOnSpawn = true), Category = "Reward Slot Widget")
			UQuestManager* QuestManagerReference;

		//-----------------Functions

		/**
		*	Main update event for the widget.
		*	Call to update all the widget content.
		*/
		UFUNCTION(BlueprintCallable, Category = "Reward Slot Widget")
			void UpdateFromTask(const FQuestTask& QuestTaskIn);

		/**
		*	Call to select the reward for this slot, for example, when clicking a button.
		*/
		UFUNCTION(BlueprintCallable, Category = "Reward Slot Widget")
			void SelectSlotReward();

		/**
		*	Main update event for the widget.
		*	All the logic needed for updating this widget should be placed here.
		*	All Reward Slot Widget must override this function.
		*	This function will be called by the reward tab.
		*	QuestTask variable contains current data.
		*/
		UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Reward Slot Widget")
			void UpdateRewardSlot();
};
