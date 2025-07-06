//Copyright 2020 Marchetti S. Alfredo I. All Rights Reserved.


#include "Task/Task_HasQuestCompletedWithEnding.h"

#include "Quest/QuestsFunctionLibrary.h"
#include "Quest/QuestManager.h"

UTask_HasQuestCompletedWithEnding::UTask_HasQuestCompletedWithEnding() : Super()
{
	//Task is activate condition only
	UseActivateConditionOnly = true;
}

bool UTask_HasQuestCompletedWithEnding::TaskShouldAnnounce_Implementation() const
{
	//Do not announce in UI
	return false;
}

bool UTask_HasQuestCompletedWithEnding::TaskShouldShowInUI_Implementation() const
{
	//Do not show in UI
	return false;
}

bool UTask_HasQuestCompletedWithEnding::ActivateCondition_Implementation(const FQuestTaskPayload& PayloadIn, const TArray<FString>& CustomPayloadDataIn, const UQuestManager* QuestManagerIn) const
{
	if (!QuestManagerIn) return false;

	//Determine if the quest is completed. Quest is passed in the Payload string A
	return QuestManagerIn->IsQuestCompletedWithEnding(false, UQuestsFunctionLibrary::StringToQuestNode(PayloadIn.StringA));	//Only works for quests, does not check dialogs
}