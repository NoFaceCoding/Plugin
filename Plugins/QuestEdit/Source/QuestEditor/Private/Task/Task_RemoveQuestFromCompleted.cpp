//Copyright 2020 Marchetti S. Alfredo I. All Rights Reserved.


#include "Task/Task_RemoveQuestFromCompleted.h"

#include "Quest/QuestsFunctionLibrary.h"
#include "Quest/QuestManager.h"

UTask_RemoveQuestFromCompleted::UTask_RemoveQuestFromCompleted() : Super()
{
	//Task is activate condition only
	UseActivateConditionOnly = false;
}

bool UTask_RemoveQuestFromCompleted::TaskShouldAnnounce_Implementation() const
{
	//Do not announce in UI
	return false;
}

bool UTask_RemoveQuestFromCompleted::TaskShouldShowInUI_Implementation() const
{
	//Do not show in UI
	return false;
}

bool UTask_RemoveQuestFromCompleted::ActivateCondition_Implementation(const FQuestTaskPayload& PayloadIn, const TArray<FString>& CustomPayloadDataIn, const UQuestManager* QuestManagerIn) const
{
	//Always activate

	return true;
}

void UTask_RemoveQuestFromCompleted::OnQuestTaskActivated_Implementation()
{
	//Make the nodes IDs from the payload.
	//Start node is obtained from payload string A.
	FQuestRep QuestRepLocal = UQuestsFunctionLibrary::StringToQuestRep(InitialPayload.StringA);

	//Accept quest directly
	bool succes = QuestManagerReference->RemoveCompletedQuestRep(QuestRepLocal, false); //This removes quests only

	//End task
	EndQuestTask(succes, 1);
}