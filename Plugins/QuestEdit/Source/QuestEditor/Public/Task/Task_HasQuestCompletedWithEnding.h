//Copyright 2020 Marchetti S. Alfredo I. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "Task/QuestTaskInterface.h"
#include "Task/BaseQuestTask.h"

#include "Task_HasQuestCompletedWithEnding.generated.h"

/**
 *	Condition Task to check if a Quest is already completed with a specific end node.
 */

UCLASS(BlueprintType)
class QUESTEDITOR_API UTask_HasQuestCompletedWithEnding : public UBaseQuestTask, public IQuestTaskInterface
{

	GENERATED_BODY()

public:

	UTask_HasQuestCompletedWithEnding();

	//----------------------------Interface override

	/*Get announce UI configuration*/
	bool TaskShouldAnnounce_Implementation() const override;

	/*Get if the task should be shown in ui or not*/
	bool TaskShouldShowInUI_Implementation() const override;

	//-----------------------------Base Quest Task Overide

	bool  ActivateCondition_Implementation(const FQuestTaskPayload& PayloadIn, const TArray <FString>& CustomPayloadDataIn, const UQuestManager* QuestManagerIn) const override;

};