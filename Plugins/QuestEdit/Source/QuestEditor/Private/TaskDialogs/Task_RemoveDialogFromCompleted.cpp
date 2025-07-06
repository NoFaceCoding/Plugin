//Copyright 2020 Marchetti S. Alfredo I. All Rights Reserved.


#include "TaskDialogs/Task_RemoveDialogFromCompleted.h"

#include "Quest/QuestsFunctionLibrary.h"
#include "Quest/QuestManager.h"

UTask_RemoveDialogFromCompleted::UTask_RemoveDialogFromCompleted() : Super()
{
	//Task is activate condition only
	UseActivateConditionOnly = false;
}

bool UTask_RemoveDialogFromCompleted::TaskShouldAnnounce_Implementation() const
{
	//Do not announce in UI
	return false;
}

bool UTask_RemoveDialogFromCompleted::TaskShouldShowInUI_Implementation() const
{
	//Do not show in UI
	return false;
}

bool UTask_RemoveDialogFromCompleted::ActivateCondition_Implementation(const FQuestTaskPayload& PayloadIn, const TArray<FString>& CustomPayloadDataIn, const UQuestManager* QuestManagerIn) const
{
	//Always activate

	return true;
}

void UTask_RemoveDialogFromCompleted::OnQuestTaskActivated_Implementation()
{
	//Make the nodes IDs from the payload.
	//Start node is obtained from payload string A.
	FQuestRep QuestRepLocal = UQuestsFunctionLibrary::StringToQuestRep(InitialPayload.StringA);

	//Accept quest directly
	bool succes = QuestManagerReference->RemoveCompletedQuestRep(QuestRepLocal, true); //This removes dialogs only

	//End task
	EndQuestTask(succes, 1);
}