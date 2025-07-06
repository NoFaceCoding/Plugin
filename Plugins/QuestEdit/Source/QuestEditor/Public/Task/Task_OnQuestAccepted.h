//Copyright 2020 Marchetti S. Alfredo I. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "Task/QuestTaskInterface.h"
#include "Task/BaseQuestTask.h"

#include "Task_OnQuestAccepted.generated.h"

/**
 *	Task that reset nodes that are connected with the specified node. This task must be used carefully.
 */

UCLASS(BlueprintType)
class QUESTEDITOR_API UTask_OnQuestAccepted : public UBaseQuestTask, public IQuestTaskInterface
{

	GENERATED_BODY()

public:

	UTask_OnQuestAccepted();

	//----------------------------Interface override

	/*Get announce UI configuration*/
	bool TaskShouldAnnounce_Implementation() const override;

	/*Get if the task should be shown in ui or not*/
	bool TaskShouldShowInUI_Implementation() const override;

	//-----------------------------Base Quest Task Overide

	bool  ActivateCondition_Implementation(const FQuestTaskPayload& PayloadIn, const TArray <FString>& CustomPayloadDataIn, const UQuestManager* QuestManagerIn) const override;
	
	void  OnQuestTaskActivated_Implementation() override;

	void  OnQuestTaskLoaded_Implementation() override;

protected:

	/*Binds to the event and waits untill it happens*/
	UFUNCTION()
	void BindToQuestEvent();

	/*Execute when the event triggers*/
	UFUNCTION()
	void OnQuestEventCallback(int QuestSlotIn, const FQuest& QuestIn);

};