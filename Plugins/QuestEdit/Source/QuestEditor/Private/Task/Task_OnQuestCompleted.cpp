//Copyright 2020 Marchetti S. Alfredo I. All Rights Reserved.


#include "Task/Task_OnQuestCompleted.h"

#include "Quest/QuestsFunctionLibrary.h"
#include "Quest/QuestManager.h"


UTask_OnQuestCompleted::UTask_OnQuestCompleted() : Super()
{
	//Task is activate condition only
	UseActivateConditionOnly = false;
}

bool UTask_OnQuestCompleted::TaskShouldAnnounce_Implementation() const
{
	//Do not announce in UI
	return false;
}

bool UTask_OnQuestCompleted::TaskShouldShowInUI_Implementation() const
{
	//Do not show in UI
	return false;
}

bool UTask_OnQuestCompleted::ActivateCondition_Implementation(const FQuestTaskPayload& PayloadIn, const TArray<FString>& CustomPayloadDataIn, const UQuestManager* QuestManagerIn) const
{
	return true;
}

void UTask_OnQuestCompleted::OnQuestTaskActivated_Implementation()
{
	BindToQuestEvent();
}

void UTask_OnQuestCompleted::OnQuestTaskLoaded_Implementation()
{
	BindToQuestEvent();
}

void UTask_OnQuestCompleted::BindToQuestEvent()
{
	if (QuestManagerReference) QuestManagerReference->QuestCompleted.AddDynamic(this, &UTask_OnQuestCompleted::OnQuestEventCallback);
}

void UTask_OnQuestCompleted::OnQuestEventCallback(const FQuest& QuestIn, int CompletedSlotIn)
{
	//Check if is the same quest
	TArray <FCompletedQuestTrack> LocalInventory = QuestManagerReference->GetCompletedQuestsTrack();
	
	if (LocalInventory.IsValidIndex(CompletedSlotIn))
	{
		//Get Quest rep in completed track
		FQuestRep QuestRepLocal = LocalInventory[CompletedSlotIn].QuestRep;

		//Get quest to check with from payload
		FQuestRep QuestRepToCheck = UQuestsFunctionLibrary::StringToQuestRep(InitialPayload.StringA);

		//If is the same completed task
		if (QuestRepLocal == QuestRepToCheck)
		{
			EndQuestTask(true);
		}
	}
}
