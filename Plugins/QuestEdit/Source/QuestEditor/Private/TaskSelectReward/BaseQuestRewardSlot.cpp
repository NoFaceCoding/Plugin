//Copyright 2020 Marchetti S. Alfredo I. All Rights Reserved.


#include "TaskSelectReward/BaseQuestRewardSlot.h"

#include "Quest/QuestManager.h"

void UBaseQuestRewardSlot::NativeConstruct()
{
	//BeginPlay
	Super::NativeConstruct();

	//Update slot on start

	//Get Task From Manager and set in a variable for easy updating and logic in blueprints.
	if (QuestManagerReference)
	{
		QuestTask = QuestManagerReference->GetTask(EQuestSelectedType::Active, QuestSlot, TaskSlot, IsDialogTask);
	}
	
	//Update from task.
	UpdateFromTask(QuestTask);
}

void UBaseQuestRewardSlot::UpdateFromTask(const FQuestTask& QuestTaskIn)
{
	//Update task
	QuestTask = QuestTaskIn;

	//Call update event
	UpdateRewardSlot();
}

void UBaseQuestRewardSlot::SelectSlotReward()
{
	//Call pred reward select
	if (QuestManagerReference) QuestManagerReference->Pred_PlayerSelectReward(QuestSlot, TaskSlot, IsDialogTask);
}
