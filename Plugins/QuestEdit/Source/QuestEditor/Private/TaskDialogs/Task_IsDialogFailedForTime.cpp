//Copyright 2020 Marchetti S. Alfredo I. All Rights Reserved.


#include "TaskDialogs/Task_IsDialogFailedForTime.h"
#include "Quest/QuestsFunctionLibrary.h"
#include "Quest/QuestManager.h"

UTask_IsDialogFailedForTime::UTask_IsDialogFailedForTime() : Super()
{
	//Task is activate condition only
	UseActivateConditionOnly = true;
}

bool UTask_IsDialogFailedForTime::TaskShouldAnnounce_Implementation() const
{
	//Do not announce in UI
	return false;
}

bool UTask_IsDialogFailedForTime::TaskShouldShowInUI_Implementation() const
{
	//Do not show in UI
	return false;
}

bool UTask_IsDialogFailedForTime::GetTaskConditionOnlyStatus() const
{
	return true;
}

bool UTask_IsDialogFailedForTime::ActivateCondition_Implementation(const FQuestTaskPayload& PayloadIn, const TArray<FString>& CustomPayloadDataIn, const UQuestManager* QuestManagerIn) const
{
	if (!QuestManagerIn) return false;

	//The time span is defined in hours, just like in quest manager
	FTimespan TimeSpanLocal = FTimespan::FromHours(PayloadIn.AmountInvolved);

	//Determine if the quest is completed. Quest is passed in the Payload string A
	return QuestManagerIn->HasQuestFailTimeSpan(true, UQuestsFunctionLibrary::StringToQuestRep(PayloadIn.StringA), TimeSpanLocal);  //Only checks dialogs
}
