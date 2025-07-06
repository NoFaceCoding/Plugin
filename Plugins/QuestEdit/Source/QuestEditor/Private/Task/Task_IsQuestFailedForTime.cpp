//Copyright 2020 Marchetti S. Alfredo I. All Rights Reserved.


#include "Task/Task_IsQuestFailedForTime.h"
#include "Quest/QuestsFunctionLibrary.h"
#include "Quest/QuestManager.h"

UTask_IsQuestFailedForTime::UTask_IsQuestFailedForTime() : Super()
{
	//Task is activate condition only
	UseActivateConditionOnly = true;
}

bool UTask_IsQuestFailedForTime::TaskShouldAnnounce_Implementation() const
{
	//Do not announce in UI
	return false;
}

bool UTask_IsQuestFailedForTime::TaskShouldShowInUI_Implementation() const
{
	//Do not show in UI
	return false;
}

bool UTask_IsQuestFailedForTime::GetTaskConditionOnlyStatus() const
{
	return true;
}

bool UTask_IsQuestFailedForTime::ActivateCondition_Implementation(const FQuestTaskPayload& PayloadIn, const TArray<FString>& CustomPayloadDataIn, const UQuestManager* QuestManagerIn) const
{
	if (!QuestManagerIn) return false;

	//The time span is defined in hours, just like in quest manager
	FTimespan TimeSpanLocal = FTimespan::FromHours(PayloadIn.AmountInvolved);

	//Determine if the quest is completed. Quest is passed in the Payload string A
	return QuestManagerIn->HasQuestFailTimeSpan(false, UQuestsFunctionLibrary::StringToQuestRep(PayloadIn.StringA), TimeSpanLocal);  //Only checks quest, does not check dialogs
}
