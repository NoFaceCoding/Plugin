//Copyright 2020 Marchetti S. Alfredo I. All Rights Reserved.


#include "TaskSelectReward/BaseQuestRewardTab.h"

#include "TimerManager.h"
#include "GameFramework/PlayerState.h"
#include "Quest/QuestManager.h"


void UBaseQuestRewardTab::NativeConstruct()
{
	//BeginPlay
	Super::NativeConstruct();

	//Bind to reward activated events
	if (GetQuestManagerReference())
	{
		GetQuestManagerReference()->OnPlayerRewardActivated.AddDynamic(this, &UBaseQuestRewardTab::OnRewardActivateBind);
		GetQuestManagerReference()->OnPlayerSelectedReward.AddDynamic(this, &UBaseQuestRewardTab::OnRewardSelectedUpdate);
	}
}

UQuestManager* UBaseQuestRewardTab::GetQuestManagerReference_Implementation()
{
	//Get Player State of the widget
	APlayerController* WidgetController = GetOwningPlayer();
	if (!WidgetController) return nullptr;
	APlayerState* WidgetState = WidgetController->PlayerState;
	if (!WidgetState) return nullptr;

	//Get Quest Manager
	UQuestManager* QuestManagerComponent = nullptr;
	QuestManagerComponent = Cast<UQuestManager>(WidgetState->FindComponentByClass(UQuestManager::StaticClass()));

	return QuestManagerComponent;
}

void UBaseQuestRewardTab::OnRewardActivateBind(int QuestSlotIn, int TaskSlotIn, bool IsDialogTaskIn)
{
	/*
		Procces all reward activations and detect if the update is needed or not.
		Avoid many updates in same frame
		Next frame i will update
	*/
	
	//We need to clean up all tasks that remaing that are no longer active.
	TArray <FRewardTabSlotInfo> ClearedActivatedTasks;
	for (int32 i = 0; i < ActivatedTasks.Num(); i++)
	{
		FQuestTaskID Id = FQuestTaskID();
		Id.TaskID = ActivatedTasks[i].TaskSlot;
		Id.TaskType = EQuestTaskType::Task;
		EQuestTaskState state = GetQuestManagerReference()->GetNodeStateInTrack(ActivatedTasks[i].QuestSlot, Id, ActivatedTasks[i].IsDialogTask);
		if (state == EQuestTaskState::Active)
		{
			ClearedActivatedTasks.Add(ActivatedTasks[i]);
		}
	}
	ActivatedTasks = ClearedActivatedTasks;

	FRewardTabSlotInfo RewardTaskLocal = FRewardTabSlotInfo();
	RewardTaskLocal.QuestSlot = QuestSlotIn;
	RewardTaskLocal.TaskSlot = TaskSlotIn;
	RewardTaskLocal.IsDialogTask = IsDialogTaskIn;

	//Add the ID to the local track to update the tab
	ActivatedTasks.AddUnique(RewardTaskLocal);

	if (!GetWorld()->GetTimerManager().IsTimerActive(UpdateTimer))
	{
		UpdateTimer = GetWorld()->GetTimerManager().SetTimerForNextTick(FTimerDelegate::CreateUObject(this, &UBaseQuestRewardTab::OnRewardActivateUpdate));
	}	
}

void UBaseQuestRewardTab::OnRewardActivateUpdate()
{
	//Get information from quest manager for updating UI
	if (GetQuestManagerReference())
	{
		//Get current info
		TArray<FRewardTabSlotInfo> CurrentRewardsInfo = GetQuestManagerReference()->GetCurrentRewardsUIData(ActivatedTasks, Retrigger_IsNPCReward, Retrigger_LocationName, Retrigger_NPCDef);

		//if there is no rewards, clear the object
		if (CurrentRewardsInfo.Num() == 0)
		{
			//Clear the tasks used. This forces to go to the location again for rewards
			ActivatedTasks.Empty();
		}
		else
		{
			//Update event to bind in blueprints
			UpdateRewardTab(CurrentRewardsInfo);
		}
	}
}

void UBaseQuestRewardTab::OnRewardSelectedUpdate(const FQuestNode& QuestNodeIn, bool IsDialogTaskIn)
{	
	//Set a timer for next tick to update tab
	UpdateTimer = GetWorld()->GetTimerManager().SetTimerForNextTick(FTimerDelegate::CreateUObject(this, &UBaseQuestRewardTab::UpdateTabWithRetriggerData));
}

void UBaseQuestRewardTab::UpdateTabWithRetriggerData()
{
	//Get retrigger data for quests
	TArray<FRewardTabSlotInfo> CurrentRewardsInfo = GetQuestManagerReference()->GetRetriggerRewardsUIData(false, Retrigger_IsNPCReward, Retrigger_LocationName, Retrigger_NPCDef);

	//Should update
	if (CurrentRewardsInfo.Num() > 0)
	{
		//Update event to bind in blueprints
		UpdateRewardTab(CurrentRewardsInfo);

		//No need to keep going
		return;
	}

	//Get retrigger data for dialogs
	CurrentRewardsInfo = GetQuestManagerReference()->GetRetriggerRewardsUIData(true, Retrigger_IsNPCReward, Retrigger_LocationName, Retrigger_NPCDef);

	//Should update
	if (CurrentRewardsInfo.Num() > 0)
	{
		//Update event to bind in blueprints
		UpdateRewardTab(CurrentRewardsInfo);
	}
}
