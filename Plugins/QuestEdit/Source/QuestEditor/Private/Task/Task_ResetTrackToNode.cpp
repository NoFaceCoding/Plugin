//Copyright 2020 Marchetti S. Alfredo I. All Rights Reserved.


#include "Task/Task_ResetTrackToNode.h"

#include "Quest/QuestsFunctionLibrary.h"
#include "Quest/QuestManager.h"

UTask_ResetTrackToNode::UTask_ResetTrackToNode() : Super()
{
	//Task is activate condition only
	UseActivateConditionOnly = false;
}

bool UTask_ResetTrackToNode::TaskShouldAnnounce_Implementation() const
{
	//Do not announce in UI
	return false;
}

bool UTask_ResetTrackToNode::TaskShouldShowInUI_Implementation() const
{
	//Do not show in UI
	return false;
}

bool UTask_ResetTrackToNode::ActivateCondition_Implementation(const FQuestTaskPayload& PayloadIn, const TArray<FString>& CustomPayloadDataIn, const UQuestManager* QuestManagerIn) const
{
	//Check that nodes are valid for the quest slot. I am doing this in the  HandleTaskEnd_ResetTrackToNode because it is easier.
	return true;
}

void UTask_ResetTrackToNode::OnQuestTaskActivated_Implementation()
{
	//Make the nodes IDs.
	FQuestTaskID StartNode = FQuestTaskID();
	StartNode.TaskID = TaskSlot;
	StartNode.TaskType = EQuestTaskType::Task;

	//End node is obtained from payload string A.
	FQuestTaskID EndNode = UQuestsFunctionLibrary::StringToTaskID(InitialPayload.StringA);

	//Calls quests manager owner function to be handled. This task ends with a custom function. The task does no logic inside it. This avoids messing with the execution.
	if (QuestManagerReference) QuestManagerReference->HandleTaskEnd_ResetTrackToNode(QuestSlot, StartNode, EndNode, IsDialogTask, false);
}