//Copyright 2020 Marchetti S. Alfredo I. All Rights Reserved.


#include "TaskLocation/Task_ReachLocationByName.h"


#include "TaskLocation/BaseQuestLocationActor.h"






UTask_ReachLocationByName::UTask_ReachLocationByName() : Super()
{

}

bool UTask_ReachLocationByName::OnQuestLocationReachedCondition_Implementation(ABaseQuestLocationActor* QuestLocationActorIn)
{
	if (!QuestLocationActorIn) return false;

	//Is the location name of payload A?
	return (InitialPayload.StringA == QuestLocationActorIn->QuestLocationName);
}

bool UTask_ReachLocationByName::TaskShouldAnnounce_Implementation() const
{
	return true;
}

bool UTask_ReachLocationByName::TaskShouldShowInUI_Implementation() const
{
	return true;
}

FString UTask_ReachLocationByName::GetTaskShortDescription_Implementation(const FQuestTaskPayload& PayloadIn, const TArray <FString>& CustomPayloadDataIn) const
{
	FString LocalString = "Travel to ";
	LocalString.Append(PayloadIn.StringA);
	return LocalString;
}

FString UTask_ReachLocationByName::GetTaskName_Implementation() const
{
	return "Reach location";
}
