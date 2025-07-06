//Copyright 2020 Marchetti S. Alfredo I. All Rights Reserved.


#include "Task/Task_IsQuestActive.h"
#include "Quest/QuestsFunctionLibrary.h"
#include "Quest/QuestManager.h"

UTask_IsQuestActive::UTask_IsQuestActive() : Super()
{
	//Task is activate condition only
	UseActivateConditionOnly = true;
}

bool UTask_IsQuestActive::TaskShouldAnnounce_Implementation() const
{
	//Do not announce in UI
	return false;
}

bool UTask_IsQuestActive::TaskShouldShowInUI_Implementation() const
{
	//Do not show in UI
	return false;
}

bool UTask_IsQuestActive::GetTaskConditionOnlyStatus() const
{
	return true;
}

bool UTask_IsQuestActive::ActivateCondition_Implementation(const FQuestTaskPayload& PayloadIn, const TArray<FString>& CustomPayloadDataIn, const UQuestManager* QuestManagerIn) const
{
	if (!QuestManagerIn) return false;

	//Determine if the quest is completed. Quest is passed in the Payload string A
	return QuestManagerIn->IsQuestInProgress(false, UQuestsFunctionLibrary::StringToQuestRep(PayloadIn.StringA)); //Only checks quests, does not check dialogs
}
