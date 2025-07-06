//Copyright 2020 Marchetti S. Alfredo I. All Rights Reserved.


#include "TaskSelectReward/BaseQuestTask_SelectReward.h"

#include "Quest/QuestManager.h"


//-----------reward task

UBaseQuestTask_SelectReward::UBaseQuestTask_SelectReward() : Super()
{
	UseActivateConditionOnly = false;
}

bool UBaseQuestTask_SelectReward::OnRewardSelectedCondition_Implementation()
{
	//Condition before give reward
	return true;
}

bool UBaseQuestTask_SelectReward::OnRewardSelected_Implementation(int QuestSlotIn, int TaskSlotIn, bool IsDialogIn)
{
	//Give reward here
	return false;
}

void UBaseQuestTask_SelectReward::BindQuestEvents_Implementation()
{
	//This should be overriden in the task itself. This is a simple implementation

	//Bind to reward selected.
	QuestManagerReference->OnPlayerSelectedReward.AddDynamic(this, &UBaseQuestTask_SelectReward::HandleRewardSelection);

}

//--------------base task

void UBaseQuestTask_SelectReward::HandleRewardSelection(const FQuestNode& QuestNodeIn, bool IsDialogTaskIn)
{
	//This function handles the override of the give reward and if succeds, it will call end task.

	//Is this the same reward slot? If not i dont do anything
	//The check is done with node instead of slots for more precision and to avoid the adjust on slots during multiple rewards active from different quests. there are some cases that dont work correctly with slots only
	if (QuestNode == QuestNodeIn && IsDialogTask == IsDialogTaskIn)
	{
		//Check condition Pre give reward. Just adds extra customization if you want to use that.
		if (OnRewardSelectedCondition())
		{
			//Attempt to give reward.
			bool RewardGiven = OnRewardSelected(QuestSlot, TaskSlot, IsDialogTask);

			//If reward succeded, end task.
			if (RewardGiven)
			{
				//RewardHandledLogic(dispatchers)
				if (QuestManagerReference) QuestManagerReference->PlayerHandledRewardSuccesfully(QuestNodeIn, IsDialogTask);

				//End task succesfully.
				EndQuestTask(true, 1);
			}				
		}
	}
}

void UBaseQuestTask_SelectReward::OnQuestTaskActivated_Implementation()
{
	//Calculate the node using slots. This is more solid than slots.	
	if (IsDialogTask)
	{
		QuestNode.Quest = QuestManagerReference->GetDialogsRepInventory()[QuestSlot];
	}
	else
	{
		QuestNode.Quest = QuestManagerReference->GetQuestsRepInventory()[QuestSlot];
	}

	QuestNode.Node.TaskID = TaskSlot;
	QuestNode.Node.TaskType = EQuestTaskType::Task;

	//Attemp reward UI and then bind in case something goes bad
	//If the local NPC is valid, use that NPC for task activation.

	QuestManagerReference->RewardTaskActivated(QuestSlot, TaskSlot, IsDialogTask);

	//Binds
	BindQuestEvents();
}

void UBaseQuestTask_SelectReward::OnQuestTaskLoaded_Implementation()
{
	//Calculate the node using slots. This is more solid than slots.	
	if (IsDialogTask)
	{
		QuestNode.Quest = QuestManagerReference->GetDialogsRepInventory()[QuestSlot];
	}
	else
	{
		QuestNode.Quest = QuestManagerReference->GetQuestsRepInventory()[QuestSlot];
	}
	QuestNode.Node.TaskID = TaskSlot;
	QuestNode.Node.TaskType = EQuestTaskType::Task;

	//Binds directly, do not attempt to give reward
	BindQuestEvents();
}

//------------------interface

bool UBaseQuestTask_SelectReward::TaskShouldAnnounce_Implementation() const
{
	return false;
}

bool UBaseQuestTask_SelectReward::TaskShouldShowInUI_Implementation() const
{
	return true;
}

FString UBaseQuestTask_SelectReward::GetTaskShortDescription_Implementation(const FQuestTaskPayload& PayloadIn, const TArray <FString>& CustomPayloadDataIn) const
{
	return "Choose a reward";
}

FString UBaseQuestTask_SelectReward::GetTaskName_Implementation() const
{
	return "Reward received";
}

bool UBaseQuestTask_SelectReward::TaskShowsOnlyOnceInSummary_Implementation(const FQuestTaskPayload& PayloadIn, FString& StringID) const
{
	StringID = "Reward";
	return true;
}

bool UBaseQuestTask_SelectReward::TaskRewardGiver_Implementation(const FQuestTaskPayload& PayloadIn, FNPCQuestDefRep& RewardGiverOut) const
{
	return false;
}
