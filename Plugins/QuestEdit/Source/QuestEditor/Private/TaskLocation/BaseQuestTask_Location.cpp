//Copyright 2020 Marchetti S. Alfredo I. All Rights Reserved.

#include "TaskLocation/BaseQuestTask_Location.h"
#include "TaskLocation/BaseQuestLocationActor.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/Controller.h"

UBaseQuestTask_Location::UBaseQuestTask_Location() : Super()
{
	UseActivateConditionOnly = false;
}

ACharacter* UBaseQuestTask_Location::GetInteractingCharacter_Implementation()
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

bool UBaseQuestTask_Location::OnQuestLocationReachedCondition_Implementation(ABaseQuestLocationActor* QuestLocationActorIn)
{
	//Condition to check when the overlap succeds
	//Override on childs.
	return true;
}

void UBaseQuestTask_Location::OnOverlapEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
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
			//End task
			EndQuestTask(true, 1);
		}
	}

	return;
}

void UBaseQuestTask_Location::OnNewPawnEvent(APawn* NewPawn)
{
	if (NewPawn)
	{
		//If there is something bound previously, unbind.
		if (CurrentCharacter)
		{
			//Remove old pawn overlap events on the task
			CurrentCharacter->GetCapsuleComponent()->OnComponentBeginOverlap.RemoveDynamic(this, &UBaseQuestTask_Location::OnOverlapEvent);

			//Remove old bind of controller posses
			if (OnNewPawnHandle.IsValid() && CurrentCharacter->GetController())
			{
				CurrentCharacter->GetController()->GetOnNewPawnNotifier().Remove(OnNewPawnHandle);
			}			
		}

		//Make sure is character
		ACharacter* LocalCharacter = Cast<ACharacter>(NewPawn);

		//bind to new pawn.
		BindOverlapCharacterDelegates(LocalCharacter);
	}	
}

void UBaseQuestTask_Location::BindOverlapCharacterDelegates(ACharacter* NewCharacter)
{
	if (NewCharacter)
	{
		//Update character reference
		CurrentCharacter = NewCharacter;

		//Bind to character overlap
		if (CurrentCharacter)
		{
			//Bind to character overlap with anything
			CurrentCharacter->GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &UBaseQuestTask_Location::OnOverlapEvent);

			//Bind to new pawn posses of the controller
			if (CurrentCharacter->GetController())
			{
				OnNewPawnHandle = CurrentCharacter->GetController()->GetOnNewPawnNotifier().AddUObject(this, &UBaseQuestTask_Location::OnNewPawnEvent);
			}		
		}
	}	
}

void UBaseQuestTask_Location::OnQuestTaskActivated_Implementation()
{
	//Get Character from function overrideable that gets the pawn
	CurrentCharacter = GetInteractingCharacter();
	
	//Bind to event overlap of the character
	BindOverlapCharacterDelegates(CurrentCharacter);
}

void UBaseQuestTask_Location::OnQuestTaskLoaded_Implementation()
{
	//Get Character from function overrideable that gets the pawn
	CurrentCharacter = GetInteractingCharacter();

	//Bind to event overlap of the character
	BindOverlapCharacterDelegates(CurrentCharacter);
}
