//Copyright 2020 Marchetti S. Alfredo I. All Rights Reserved.


#include "Task/Task_OnQuestAccepted.h"

#include "Quest/QuestsFunctionLibrary.h"
#include "Quest/QuestManager.h"


UTask_OnQuestAccepted::UTask_OnQuestAccepted() : Super()
{
	//Task is activate condition only
	UseActivateConditionOnly = false;
}

bool UTask_OnQuestAccepted::TaskShouldAnnounce_Implementation() const
{
	//Do not announce in UI
	return false;
}

bool UTask_OnQuestAccepted::TaskShouldShowInUI_Implementation() const
{
	//Do not show in UI
	return false;
}

bool UTask_OnQuestAccepted::ActivateCondition_Implementation(const FQuestTaskPayload& PayloadIn, const TArray<FString>& CustomPayloadDataIn, const UQuestManager* QuestManagerIn) const
{
	return true;
}

void UTask_OnQuestAccepted::OnQuestTaskActivated_Implementation()
{
	BindToQuestEvent();
}

void UTask_OnQuestAccepted::OnQuestTaskLoaded_Implementation()
{
	BindToQuestEvent();
}

void UTask_OnQuestAccepted::BindToQuestEvent()
{
	if (QuestManagerReference) QuestManagerReference->QuestAccepted.AddDynamic(this, &UTask_OnQuestAccepted::OnQuestEventCallback);
}

void UTask_OnQuestAccepted::OnQuestEventCallback(int QuestSlotIn, const FQuest& QuestIn)
{
	//Check if is the same quest
	TArray <FQuestRep> LocalInventory = QuestManagerReference->GetQuestsRepInventory();
	
	if (LocalInventory.IsValidIndex(QuestSlotIn))
	{
		//Get Quest rep in completed track
		FQuestRep QuestRepLocal = LocalInventory[QuestSlotIn];

		//Get quest to check with from payload
		FQuestRep QuestRepToCheck = UQuestsFunctionLibrary::StringToQuestRep(InitialPayload.StringA);

		//If is the same complete task
		if (QuestRepLocal == QuestRepToCheck)
		{
			EndQuestTask(true);
		}
	}
}
