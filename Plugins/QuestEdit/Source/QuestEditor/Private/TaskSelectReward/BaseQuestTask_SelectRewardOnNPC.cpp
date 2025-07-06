//Copyright 2020 Marchetti S. Alfredo I. All Rights Reserved.


#include "TaskSelectReward/BaseQuestTask_SelectRewardOnNPC.h"

#include "Quest/QuestManager.h"
#include "Quest/QuestsFunctionLibrary.h"
#include "Npc/NPCManager.h"
#include "Runtime/Engine/Classes/GameFramework/Actor.h"

UBaseQuestTask_SelectRewardOnNPC::UBaseQuestTask_SelectRewardOnNPC() : Super()
{
	UseActivateConditionOnly = false;
}

void UBaseQuestTask_SelectRewardOnNPC::OnNPCReachForReward_Implementation(AActor* NPCIN)
{
	//Check if the NPC of String A is the current one in the quest manager.

	//Get NPC of task
	FNPCQuestDefRep NPCOfTask = UQuestsFunctionLibrary::StringToNPCQuestDefRep(InitialPayload.StringA);

	//GetNPCManager of the NPC Provided
	if (!NPCIN) return;
	UNPCManager* NPCManagerComponent;
	NPCManagerComponent = Cast<UNPCManager>(NPCIN->FindComponentByClass(UNPCManager::StaticClass()));

	//NPC null
	if (!NPCManagerComponent) return;

	FNPCQuestDefRep NPCReached = NPCManagerComponent->GetNPCQuestDefRep();

	//If they are the same NPC
	if (NPCOfTask == NPCReached)
	{
		//--------Call UI for rewards
		QuestManagerReference->RewardTaskActivated(QuestSlot, TaskSlot, IsDialogTask);
	}
}

bool UBaseQuestTask_SelectRewardOnNPC::OnRewardSelected_Implementation(int QuestSlotIn, int TaskSlotIn, bool IsDialogIn)
{
	//Override to give reward
	return false;
}

void UBaseQuestTask_SelectRewardOnNPC::BindQuestEvents_Implementation()
{
	//--------Bind to NPC reached
	QuestManagerReference->OnNPCReached.AddDynamic(this, &UBaseQuestTask_SelectRewardOnNPC::OnNPCReachForReward);

	//--------Bind to reward selected.
	QuestManagerReference->OnPlayerSelectedReward.AddDynamic(this, &UBaseQuestTask_SelectRewardOnNPC::HandleRewardSelection);
}

FString UBaseQuestTask_SelectRewardOnNPC::GetTaskShortDescription_Implementation(const FQuestTaskPayload& PayloadIn, const TArray <FString>& CustomPayloadDataIn) const
{
	//Get NPC of task
	FNPCQuestDefRep NPCOfTask = UQuestsFunctionLibrary::StringToNPCQuestDefRep(PayloadIn.StringA);
	FNPCQuestDef NPCDef = UQuestsFunctionLibrary::NPCQuestDefRepToNPCQuestDef(NPCOfTask);

	FString LocalString = "Choose a reward from ";
	LocalString.Append(NPCDef.NPCName);

	return LocalString;
}

bool UBaseQuestTask_SelectRewardOnNPC::TaskShowsOnlyOnceInSummary_Implementation(const FQuestTaskPayload& PayloadIn, FString& StringID) const
{
	//Get NPC of task
	FNPCQuestDefRep NPCOfTask = UQuestsFunctionLibrary::StringToNPCQuestDefRep(PayloadIn.StringA);
	FNPCQuestDef NPCDef = UQuestsFunctionLibrary::NPCQuestDefRepToNPCQuestDef(NPCOfTask);

	//Make a ID for each NPC, so they do not overlap in UI if u have multiple rewards at same time for different npcs
	FString LocalString = "Reward";
	LocalString.Append(NPCDef.NPCName);

	StringID = LocalString;
	return true;
}

bool UBaseQuestTask_SelectRewardOnNPC::TaskRewardGiver_Implementation(const FQuestTaskPayload& PayloadIn, FNPCQuestDefRep& RewardGiverOut) const
{
	RewardGiverOut = UQuestsFunctionLibrary::StringToNPCQuestDefRep(PayloadIn.StringA);
	return true;
}
