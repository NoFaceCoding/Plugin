//Copyright 2020 Marchetti S. Alfredo I. All Rights Reserved.


#include "TaskDialogs/Task_RemoveDialogFromFailTrack.h"

#include "Quest/QuestsFunctionLibrary.h"
#include "Quest/QuestManager.h"

UTask_RemoveDialogFromFailTrack::UTask_RemoveDialogFromFailTrack() : Super()
{
	//Task is activate condition only
	UseActivateConditionOnly = false;
}

bool UTask_RemoveDialogFromFailTrack::TaskShouldAnnounce_Implementation() const
{
	//Do not announce in UI
	return false;
}

bool UTask_RemoveDialogFromFailTrack::TaskShouldShowInUI_Implementation() const
{
	//Do not show in UI
	return false;
}

bool UTask_RemoveDialogFromFailTrack::ActivateCondition_Implementation(const FQuestTaskPayload& PayloadIn, const TArray<FString>& CustomPayloadDataIn, const UQuestManager* QuestManagerIn) const
{
	//Always activate

	return true;
}

void UTask_RemoveDialogFromFailTrack::OnQuestTaskActivated_Implementation()
{
	//Make the nodes IDs from the payload.
	//Start node is obtained from payload string A.
	FQuestRep QuestRepLocal = UQuestsFunctionLibrary::StringToQuestRep(InitialPayload.StringA);

	//Accept quest directly
	bool succes = QuestManagerReference->RemoveQuestFromFailTrack(true, QuestRepLocal);  //Only for dialog

	//End task
	EndQuestTask(succes, 1);
}