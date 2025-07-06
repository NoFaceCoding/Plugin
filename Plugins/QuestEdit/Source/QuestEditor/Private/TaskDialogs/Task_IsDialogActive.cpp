//Copyright 2020 Marchetti S. Alfredo I. All Rights Reserved.


#include "TaskDialogs/Task_IsDialogActive.h"
#include "Quest/QuestsFunctionLibrary.h"
#include "Quest/QuestManager.h"

UTask_IsDialogActive::UTask_IsDialogActive() : Super()
{
	//Task is activate condition only
	UseActivateConditionOnly = true;
}

bool UTask_IsDialogActive::TaskShouldAnnounce_Implementation() const
{
	//Do not announce in UI
	return false;
}

bool UTask_IsDialogActive::TaskShouldShowInUI_Implementation() const
{
	//Do not show in UI
	return false;
}

bool UTask_IsDialogActive::GetTaskConditionOnlyStatus() const
{
	return true;
}

bool UTask_IsDialogActive::ActivateCondition_Implementation(const FQuestTaskPayload& PayloadIn, const TArray<FString>& CustomPayloadDataIn, const UQuestManager* QuestManagerIn) const
{
	if (!QuestManagerIn) return false;

	//Determine if the quest is completed. Quest is passed in the Payload string A
	return QuestManagerIn->IsQuestInProgress(true, UQuestsFunctionLibrary::StringToQuestRep(PayloadIn.StringA)); //Only checks dialogs
}
