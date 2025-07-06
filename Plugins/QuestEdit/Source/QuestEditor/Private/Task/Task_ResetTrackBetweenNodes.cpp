//Copyright 2020 Marchetti S. Alfredo I. All Rights Reserved.


#include "Task/Task_ResetTrackBetweenNodes.h"

#include "Quest/QuestsFunctionLibrary.h"
#include "Quest/QuestManager.h"

UTask_ResetTrackBetweenNodes::UTask_ResetTrackBetweenNodes() : Super()
{
	//Task is activate condition only
	UseActivateConditionOnly = false;
}

bool UTask_ResetTrackBetweenNodes::TaskShouldAnnounce_Implementation() const
{
	//Do not announce in UI
	return false;
}

bool UTask_ResetTrackBetweenNodes::TaskShouldShowInUI_Implementation() const
{
	//Do not show in UI
	return false;
}

bool UTask_ResetTrackBetweenNodes::ActivateCondition_Implementation(const FQuestTaskPayload& PayloadIn, const TArray<FString>& CustomPayloadDataIn, const UQuestManager* QuestManagerIn) const
{
	//Check that nodes are valid for the quest slot could be done.
	return true;
}

void UTask_ResetTrackBetweenNodes::OnQuestTaskActivated_Implementation()
{
	//Make the nodes IDs from the payload.
	//Start node is obtained from payload string A.
	FQuestTaskID StartNode = UQuestsFunctionLibrary::StringToTaskID(InitialPayload.StringA);
	//End node is obtained from payload string B.
	FQuestTaskID EndNode = UQuestsFunctionLibrary::StringToTaskID(InitialPayload.StringB);

	//Lock nodes
	bool succes = QuestManagerReference->LockNodesInBetweenNodes(QuestSlot, StartNode, EndNode, IsDialogTask);

	//End task
	EndQuestTask(succes, 1);
}