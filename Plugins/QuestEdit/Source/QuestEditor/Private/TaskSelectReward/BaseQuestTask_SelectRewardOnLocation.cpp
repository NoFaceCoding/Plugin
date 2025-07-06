//Copyright 2020 Marchetti S. Alfredo I. All Rights Reserved.


#include "TaskSelectReward/BaseQuestTask_SelectRewardOnLocation.h"

//Location includes
#include "TaskLocation/BaseQuestLocationActor.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/PlayerState.h"


//Reward includes
#include "Quest/QuestManager.h"
#include "Quest/QuestsFunctionLibrary.h"
#include "Npc/NPCManager.h"
#include "Runtime/Engine/Classes/GameFramework/Actor.h"

UBaseQuestTask_SelectRewardOnLocation::UBaseQuestTask_SelectRewardOnLocation() : Super()
{
	UseActivateConditionOnly = false;
}

//---------------------------------------------------------------------------------------------
//------------------LOCATION TASK FUNCTIONS
//---------------------------------------------------------------------------------------------

ACharacter* UBaseQuestTask_SelectRewardOnLocation::GetInteractingCharacter_Implementation()
{
	//get state
	APlayerState* StateCasted = Cast <APlayerState>(QuestManagerOwner);
	if (!StateCasted) return nullptr;
	//Get Character pawn
	APawn* Pawn = StateCasted->GetPawn();
	if (!Pawn) return nullptr;
	ACharacter* CharacterOut = Cast <ACharacter>(Pawn);
	return CharacterOut;
}

bool UBaseQuestTask_SelectRewardOnLocation::OnQuestLocationReachedCondition_Implementation(ABaseQuestLocationActor* QuestLocationActorIn)
{
	//Condition to check when the overlap succeds
	//Override on childs.

	return true;
}

void UBaseQuestTask_SelectRewardOnLocation::OnLocationReachForReward_Implementation(ABaseQuestLocationActor* LocationIN)
{
	//Check if the location is correct.
	//If they are the same location
	if (LocationIN->GetQuestLocationName() == InitialPayload.StringA)
	{
		//--------Call UI for rewards
		QuestManagerReference->RewardTaskActivated(QuestSlot, TaskSlot, IsDialogTask);
	}
}

void UBaseQuestTask_SelectRewardOnLocation::OnOverlapEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//All the check logic goes here.

	//The overlaped actor is a  BaseQuestLocationActor
	ABaseQuestLocationActor* BaseQuestLocation = Cast<ABaseQuestLocationActor>(OtherActor);

	//If is a quest location actor
	if (BaseQuestLocation)
	{
		//Mets condition?
		if (OnQuestLocationReachedCondition_Implementation(BaseQuestLocation))
		{
			//Reached LOCATION logic. This should be
			OnLocationReachForReward(BaseQuestLocation);
		}
	}

	return;
}

void UBaseQuestTask_SelectRewardOnLocation::OnNewPawnEvent(APawn* NewPawn)
{
	//If there is something bound previously, unbind.
	if (CurrentCharacter && OnNewPawnHandle.IsValid())
	{
		//Remove old pawn overlap events on the task
		CurrentCharacter->GetCapsuleComponent()->OnComponentBeginOverlap.RemoveDynamic(this, &UBaseQuestTask_SelectRewardOnLocation::OnOverlapEvent);

		//Remove old bind of controller posses
		if (CurrentCharacter->GetController())
		{
			CurrentCharacter->GetController()->GetOnNewPawnNotifier().Remove(OnNewPawnHandle);
		}		
	}

	//Make sure is character
	ACharacter* LocalCharacter = Cast<ACharacter>(NewPawn);

	//bind to new pawn.
	BindOverlapCharacterDelegates(LocalCharacter);
}

void UBaseQuestTask_SelectRewardOnLocation::BindOverlapCharacterDelegates(ACharacter* NewCharacter)
{
	//Update character reference
	CurrentCharacter = NewCharacter;

	//Bind to character overlap
	if (CurrentCharacter)
	{
		//Bind to character overlap with anything
		CurrentCharacter->GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &UBaseQuestTask_SelectRewardOnLocation::OnOverlapEvent);

		//Bind to new pawn posses of the controller
		OnNewPawnHandle = CurrentCharacter->GetController()->GetOnNewPawnNotifier().AddUObject(this, &UBaseQuestTask_SelectRewardOnLocation::OnNewPawnEvent);
	}
}

void UBaseQuestTask_SelectRewardOnLocation::OnQuestTaskActivated_Implementation()
{
	//Calculate the node using slots. This is more solid than slots.	
	if (IsDialogTask)
	{
		QuestNode.Quest = QuestManagerReference->GetDialogsRepInventory()[QuestSlot];
	}
	else
	{
		QuestNode.Quest = QuestManagerReference->GetQuestsRepInventory()[QuestSlot];
	}
	QuestNode.Node.TaskID = TaskSlot;
	QuestNode.Node.TaskType = EQuestTaskType::Task;

	//Get Character from function overrideable that gets the pawn
	CurrentCharacter = GetInteractingCharacter();

	//Bind to event overlap of the character
	BindOverlapCharacterDelegates(CurrentCharacter);

	//bind to Rewards selections
	BindQuestEvents();
}

void UBaseQuestTask_SelectRewardOnLocation::OnQuestTaskLoaded_Implementation()
{
	//Calculate the node using slots. This is more solid than slots.	
	if (IsDialogTask)
	{
		QuestNode.Quest = QuestManagerReference->GetDialogsRepInventory()[QuestSlot];
	}
	else
	{
		QuestNode.Quest = QuestManagerReference->GetQuestsRepInventory()[QuestSlot];
	}
	QuestNode.Node.TaskID = TaskSlot;
	QuestNode.Node.TaskType = EQuestTaskType::Task;

	//Get Character from function overrideable that gets the pawn
	CurrentCharacter = GetInteractingCharacter();

	//Bind to event overlap of the character
	BindOverlapCharacterDelegates(CurrentCharacter);

	//bind to Rewards selections
	BindQuestEvents();
}

//---------------------------------------------------------------------------------------------
//------------------REWARD TASK FUNCTIONS
//---------------------------------------------------------------------------------------------

bool UBaseQuestTask_SelectRewardOnLocation::OnRewardSelected_Implementation(int QuestSlotIn, int TaskSlotIn, bool IsDialogIn)
{
	//Override to give reward
	return false;
}

void UBaseQuestTask_SelectRewardOnLocation::BindQuestEvents_Implementation()
{
	//--------Bind to reward selected.
	QuestManagerReference->OnPlayerSelectedReward.AddDynamic(this, &UBaseQuestTask_SelectRewardOnLocation::HandleRewardSelection);
}

FString UBaseQuestTask_SelectRewardOnLocation::GetTaskShortDescription_Implementation(const FQuestTaskPayload& PayloadIn, const TArray <FString>& CustomPayloadDataIn) const
{
	//Choose reward from location
	FString LocalString = "Choose a reward from ";
	LocalString.Append(PayloadIn.StringA);

	return LocalString;
}

bool UBaseQuestTask_SelectRewardOnLocation::TaskShowsOnlyOnceInSummary_Implementation(const FQuestTaskPayload& PayloadIn, FString& StringID) const
{
	//Make a ID for each NPC, so they do not overlap in UI if u have multiple rewards at same time for different npcs
	FString LocalString = "Reward";
	LocalString.Append(PayloadIn.StringA);

	StringID = LocalString;
	return true;
}

bool UBaseQuestTask_SelectRewardOnLocation::TaskRewardLocation_Implementation(const FQuestTaskPayload& PayloadIn, FString& LocationNameOut) const
{
	LocationNameOut = PayloadIn.StringA;
	return true;
}
