//Copyright 2020 Marchetti S. Alfredo I. All Rights Reserved.


#include "TaskDialogs/Task_HasDialogCompleted.h"
#include "Quest/QuestsFunctionLibrary.h"
#include "Quest/QuestManager.h"

UTask_HasDialogCompleted::UTask_HasDialogCompleted() : Super()
{
	//Task is activate condition only
	UseActivateConditionOnly = true;
}

bool UTask_HasDialogCompleted::TaskShouldAnnounce_Implementation() const
{
	//Do not announce in UI
	return false;
}

bool UTask_HasDialogCompleted::TaskShouldShowInUI_Implementation() const
{
	//Do not show in UI
	return false;
}

bool UTask_HasDialogCompleted::GetTaskConditionOnlyStatus() const
{
	return true;
}

bool UTask_HasDialogCompleted::ActivateCondition_Implementation(const FQuestTaskPayload& PayloadIn, const TArray<FString>& CustomPayloadDataIn, const UQuestManager* QuestManagerIn) const
{
	if (!QuestManagerIn) return false;

	//Determine if the quest is completed. Quest is passed in the Payload string A
	return QuestManagerIn->IsQuestCompleted(true, UQuestsFunctionLibrary::StringToQuestRep(PayloadIn.StringA)); //This only checks dialogs
}
