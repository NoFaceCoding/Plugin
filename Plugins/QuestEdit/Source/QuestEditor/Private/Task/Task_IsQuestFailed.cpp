//Copyright 2020 Marchetti S. Alfredo I. All Rights Reserved.


#include "Task/Task_IsQuestFailed.h"
#include "Quest/QuestsFunctionLibrary.h"
#include "Quest/QuestManager.h"

UTask_IsQuestFailed::UTask_IsQuestFailed() : Super()
{
	//Task is activate condition only
	UseActivateConditionOnly = true;
}

bool UTask_IsQuestFailed::TaskShouldAnnounce_Implementation() const
{
	//Do not announce in UI
	return false;
}

bool UTask_IsQuestFailed::TaskShouldShowInUI_Implementation() const
{
	//Do not show in UI
	return false;
}

bool UTask_IsQuestFailed::GetTaskConditionOnlyStatus() const
{
	return true;
}

bool UTask_IsQuestFailed::ActivateCondition_Implementation(const FQuestTaskPayload& PayloadIn, const TArray<FString>& CustomPayloadDataIn, const UQuestManager* QuestManagerIn) const
{
	if (!QuestManagerIn) return false;

	//Determine if the quest is completed. Quest is passed in the Payload string A
	return QuestManagerIn->IsQuestFailed(false, UQuestsFunctionLibrary::StringToQuestRep(PayloadIn.StringA));  //Only check quests, does not check dialogs
}
