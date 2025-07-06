//Copyright 2020 Marchetti S. Alfredo I. All Rights Reserved.


#include "Task/Task_OnQuestFailed.h"

#include "Quest/QuestsFunctionLibrary.h"
#include "Quest/QuestManager.h"


UTask_OnQuestFailed::UTask_OnQuestFailed() : Super()
{
	//Task is activate condition only
	UseActivateConditionOnly = false;
}

bool UTask_OnQuestFailed::TaskShouldAnnounce_Implementation() const
{
	//Do not announce in UI
	return false;
}

bool UTask_OnQuestFailed::TaskShouldShowInUI_Implementation() const
{
	//Do not show in UI
	return false;
}

bool UTask_OnQuestFailed::ActivateCondition_Implementation(const FQuestTaskPayload& PayloadIn, const TArray<FString>& CustomPayloadDataIn, const UQuestManager* QuestManagerIn) const
{
	return true;
}

void UTask_OnQuestFailed::OnQuestTaskActivated_Implementation()
{
	BindToQuestEvent();
}

void UTask_OnQuestFailed::OnQuestTaskLoaded_Implementation()
{
	BindToQuestEvent();
}

void UTask_OnQuestFailed::BindToQuestEvent()
{
	if (QuestManagerReference) QuestManagerReference->QuestFailed.AddDynamic(this, &UTask_OnQuestFailed::OnQuestEventCallback);
}

void UTask_OnQuestFailed::OnQuestEventCallback(int QuestSlotIn, const FQuest& QuestIn)
{
	//Check if is the same quest
	TArray <FCompletedQuestTrack> LocalInventory = QuestManagerReference->GetCompletedQuestsTrack();
	
	if (LocalInventory.IsValidIndex(QuestSlotIn))
	{
		//Get Quest rep in completed track
		FQuestRep QuestRepLocal = LocalInventory[QuestSlotIn].QuestRep;

		//Get quest to check with from payload
		FQuestRep QuestRepToCheck = UQuestsFunctionLibrary::StringToQuestRep(InitialPayload.StringA);

		//If is the same complete task
		if (QuestRepLocal == QuestRepToCheck)
		{
			EndQuestTask(true);
		}
	}
}
