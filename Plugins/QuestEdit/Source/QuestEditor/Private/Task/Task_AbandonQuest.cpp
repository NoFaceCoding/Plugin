//Copyright 2020 Marchetti S. Alfredo I. All Rights Reserved.


#include "Task/Task_AbandonQuest.h"

#include "Quest/QuestsFunctionLibrary.h"
#include "Quest/QuestManager.h"

UTask_AbandonQuest::UTask_AbandonQuest() : Super()
{
	//Task is activate condition only
	UseActivateConditionOnly = false;
}

bool UTask_AbandonQuest::TaskShouldAnnounce_Implementation() const
{
	//Do not announce in UI
	return false;
}

bool UTask_AbandonQuest::TaskShouldShowInUI_Implementation() const
{
	//Do not show in UI
	return false;
}

bool UTask_AbandonQuest::ActivateCondition_Implementation(const FQuestTaskPayload& PayloadIn, const TArray<FString>& CustomPayloadDataIn, const UQuestManager* QuestManagerIn) const
{
	//Is always true. This task can fail in the execution.

	return true;
}

void UTask_AbandonQuest::OnQuestTaskActivated_Implementation()
{
	//Make the nodes IDs from the payload.
	//Start node is obtained from payload string A.
	FQuestRep QuestRepLocal = UQuestsFunctionLibrary::StringToQuestRep(InitialPayload.StringA);

	//Abandon quest directly
	bool succes = QuestManagerReference->AbandonQuestRep(false, QuestRepLocal); //This is only for abandon Quests

	//End task
	EndQuestTask(succes, 1);
}
