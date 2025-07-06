//Copyright 2020 Marchetti S. Alfredo I. All Rights Reserved.


#include "Task/Task_HasQuestCompleted.h"
#include "Quest/QuestsFunctionLibrary.h"
#include "Quest/QuestManager.h"

UTask_HasQuestCompleted::UTask_HasQuestCompleted() : Super()
{
	//Task is activate condition only
	UseActivateConditionOnly = true;
}

bool UTask_HasQuestCompleted::TaskShouldAnnounce_Implementation() const
{
	//Do not announce in UI
	return false;
}

bool UTask_HasQuestCompleted::TaskShouldShowInUI_Implementation() const
{
	//Do not show in UI
	return false;
}

bool UTask_HasQuestCompleted::GetTaskConditionOnlyStatus() const
{
	return true;
}

bool UTask_HasQuestCompleted::ActivateCondition_Implementation(const FQuestTaskPayload& PayloadIn, const TArray<FString>& CustomPayloadDataIn, const UQuestManager* QuestManagerIn) const
{
	if (!QuestManagerIn) return false;

	//Determine if the quest is completed. Quest is passed in the Payload string A
	return QuestManagerIn->IsQuestCompleted(false, UQuestsFunctionLibrary::StringToQuestRep(PayloadIn.StringA)); //This only checks quests
}
