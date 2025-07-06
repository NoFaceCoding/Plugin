//Copyright 2020 Marchetti S. Alfredo I. All Rights Reserved.


#include "TaskDialogs/Task_ResetNPCDialogs.h"

#include "Quest/QuestsFunctionLibrary.h"
#include "Quest/QuestManager.h"

UTask_ResetNPCDialogs::UTask_ResetNPCDialogs() : Super()
{
	//Task is activate condition only
	UseActivateConditionOnly = false;
}

bool UTask_ResetNPCDialogs::TaskShouldAnnounce_Implementation() const
{
	//Do not announce in UI
	return false;
}

bool UTask_ResetNPCDialogs::TaskShouldShowInUI_Implementation() const
{
	//Do not show in UI
	return false;
}

bool UTask_ResetNPCDialogs::ActivateCondition_Implementation(const FQuestTaskPayload& PayloadIn, const TArray<FString>& CustomPayloadDataIn, const UQuestManager* QuestManagerIn) const
{
	//Check that nodes are valid for the quest slot. I am doing this in the  HandleTaskEnd_ResetTrackToNode because it is easier.
	return true;
}

void UTask_ResetNPCDialogs::OnQuestTaskActivated_Implementation()
{
	//Get NPC def
	FNPCQuestDefRep NPC = UQuestsFunctionLibrary::StringToNPCQuestDefRep(InitialPayload.StringA);

	//Reset NPC
	if (QuestManagerReference) QuestManagerReference->ResetNPCDialogsFromTask(QuestSlot, IsDialogTask, NPC);

	//End Task
	EndQuestTask();
}