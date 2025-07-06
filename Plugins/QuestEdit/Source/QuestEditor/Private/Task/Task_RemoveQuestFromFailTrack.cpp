//Copyright 2020 Marchetti S. Alfredo I. All Rights Reserved.


#include "Task/Task_RemoveQuestFromFailTrack.h"

#include "Quest/QuestsFunctionLibrary.h"
#include "Quest/QuestManager.h"

UTask_RemoveQuestFromFailTrack::UTask_RemoveQuestFromFailTrack() : Super()
{
	//Task is activate condition only
	UseActivateConditionOnly = false;
}

bool UTask_RemoveQuestFromFailTrack::TaskShouldAnnounce_Implementation() const
{
	//Do not announce in UI
	return false;
}

bool UTask_RemoveQuestFromFailTrack::TaskShouldShowInUI_Implementation() const
{
	//Do not show in UI
	return false;
}

bool UTask_RemoveQuestFromFailTrack::ActivateCondition_Implementation(const FQuestTaskPayload& PayloadIn, const TArray<FString>& CustomPayloadDataIn, const UQuestManager* QuestManagerIn) const
{
	//Always activate

	return true;
}

void UTask_RemoveQuestFromFailTrack::OnQuestTaskActivated_Implementation()
{
	//Make the nodes IDs from the payload.
	//Start node is obtained from payload string A.
	FQuestRep QuestRepLocal = UQuestsFunctionLibrary::StringToQuestRep(InitialPayload.StringA);

	//Accept quest directly
	bool succes = QuestManagerReference->RemoveQuestFromFailTrack(false, QuestRepLocal);  //Only for quests

	//End task
	EndQuestTask(succes, 1);
}