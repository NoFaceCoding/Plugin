//Copyright 2020 Marchetti S. Alfredo I. All Rights Reserved.


#include "Task/Task_ForceQuestFail.h"

#include "Quest/QuestsFunctionLibrary.h"
#include "Quest/QuestManager.h"

UTask_ForceQuestFail::UTask_ForceQuestFail() : Super()
{
	//Task is activate condition only
	UseActivateConditionOnly = false;
}

bool UTask_ForceQuestFail::TaskShouldAnnounce_Implementation() const
{
	//Do not announce in UI
	return false;
}

bool UTask_ForceQuestFail::TaskShouldShowInUI_Implementation() const
{
	//Do not show in UI
	return false;
}

bool UTask_ForceQuestFail::ActivateCondition_Implementation(const FQuestTaskPayload& PayloadIn, const TArray<FString>& CustomPayloadDataIn, const UQuestManager* QuestManagerIn) const
{
	//always true
	return true;
}

void UTask_ForceQuestFail::OnQuestTaskActivated_Implementation()
{
	//Get all nodes that are active, excluding this one and make them fail.
	//Get self node
	FQuestTaskID CurrentTaskNode = FQuestTaskID();
	CurrentTaskNode.TaskType = EQuestTaskType::Task;
	CurrentTaskNode.TaskID = TaskSlot;

	//Fail nodes with exception
	QuestManagerReference->ForceActiveNodesFailWithException(QuestSlot, IsDialogTask, CurrentTaskNode); //This works for quest or dialog, depending on how it was assigned

	//End task always with a fail, to naturally fail the quest.
	EndQuestTask(false, 1);
}
