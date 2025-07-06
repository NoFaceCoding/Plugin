//Copyright 2020 Marchetti S. Alfredo I. All Rights Reserved.


#include "TaskDialogs/Task_HasDialogCompletedWithEnding.h"

#include "Quest/QuestsFunctionLibrary.h"
#include "Quest/QuestManager.h"

UTask_HasDialogCompletedWithEnding::UTask_HasDialogCompletedWithEnding() : Super()
{
	//Task is activate condition only
	UseActivateConditionOnly = true;
}

bool UTask_HasDialogCompletedWithEnding::TaskShouldAnnounce_Implementation() const
{
	//Do not announce in UI
	return false;
}

bool UTask_HasDialogCompletedWithEnding::TaskShouldShowInUI_Implementation() const
{
	//Do not show in UI
	return false;
}

bool UTask_HasDialogCompletedWithEnding::ActivateCondition_Implementation(const FQuestTaskPayload& PayloadIn, const TArray<FString>& CustomPayloadDataIn, const UQuestManager* QuestManagerIn) const
{
	if (!QuestManagerIn) return false;

	//Determine if the quest is completed. Quest is passed in the Payload string A
	return QuestManagerIn->IsQuestCompletedWithEnding(true, UQuestsFunctionLibrary::StringToQuestNode(PayloadIn.StringA));	//Only works for dialogs
}