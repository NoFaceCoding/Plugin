//Copyright 2020 Marchetti S. Alfredo I. All Rights Reserved.


#include "Task/Task_AcceptQuestDirectly.h"

#include "Quest/QuestsFunctionLibrary.h"
#include "Quest/QuestManager.h"

UTask_AcceptQuestDirectly::UTask_AcceptQuestDirectly() : Super()
{
	//Task is activate condition only
	UseActivateConditionOnly = false;
}

bool UTask_AcceptQuestDirectly::TaskShouldAnnounce_Implementation() const
{
	//Do not announce in UI
	return false;
}

bool UTask_AcceptQuestDirectly::TaskShouldShowInUI_Implementation() const
{
	//Do not show in UI
	return false;
}

bool UTask_AcceptQuestDirectly::ActivateCondition_Implementation(const FQuestTaskPayload& PayloadIn, const TArray<FString>& CustomPayloadDataIn, const UQuestManager* QuestManagerIn) const
{
	//It always activates. Can fail on execution.
	return true;
}

void UTask_AcceptQuestDirectly::OnQuestTaskActivated_Implementation()
{
	//Make the nodes IDs from the payload.
	//Start node is obtained from payload string A.
	FQuestRep QuestRepLocal = UQuestsFunctionLibrary::StringToQuestRep(InitialPayload.StringA);

	//Accept quest directly
	bool succes = QuestManagerReference->AcceptQuestDirectly(QuestRepLocal, false); //This is only for quest, you cannot accept a dialog with this object

	//End task
	EndQuestTask(succes, 1);
}