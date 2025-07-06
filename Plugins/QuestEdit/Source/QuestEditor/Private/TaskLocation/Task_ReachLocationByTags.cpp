//Copyright 2020 Marchetti S. Alfredo I. All Rights Reserved.


#include "TaskLocation/Task_ReachLocationByTags.h"

#include "TaskLocation/BaseQuestLocationActor.h"


UTask_ReachLocationByTags::UTask_ReachLocationByTags() : Super()
{
	
}

bool UTask_ReachLocationByTags::OnQuestLocationReachedCondition_Implementation(ABaseQuestLocationActor* QuestLocationActorIn)
{	
	if (!QuestLocationActorIn) return false;

	//Location has all required and none of the ignored?
	return (QuestLocationActorIn->QuestLocationTags.HasAll(InitialPayload.RequiredTags) && !QuestLocationActorIn->QuestLocationTags.HasAny(InitialPayload.IgnoredTags));
}

bool UTask_ReachLocationByTags::TaskShouldAnnounce_Implementation() const
{
	return true;
}

bool UTask_ReachLocationByTags::TaskShouldShowInUI_Implementation() const
{
	return true;
}

FString UTask_ReachLocationByTags::GetTaskShortDescription_Implementation(const FQuestTaskPayload& PayloadIn, const TArray <FString>& CustomPayloadDataIn) const
{
	FString LocalString = "Travel to ";

	//It takes required tag and shows the first one last part

	FString TagString = PayloadIn.RequiredTags.Last().GetTagName().ToString();
	FString LeftString, RightString;

	if (TagString.Split(FString("."), &LeftString, &RightString, ESearchCase::IgnoreCase, ESearchDir::FromEnd))
	{
		TagString = RightString;
	}

	LocalString.Append(TagString);
	return LocalString;
}

FString UTask_ReachLocationByTags::GetTaskName_Implementation() const
{
	return "Reach location";
}
