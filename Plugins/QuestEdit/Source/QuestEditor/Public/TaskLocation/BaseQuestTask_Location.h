//Copyright 2020 Marchetti S. Alfredo I. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "Task/QuestTaskInterface.h"

#include "Task/BaseQuestTask.h"

#include "BaseQuestTask_Location.generated.h"


class ACharacter;
class ABaseQuestLocationActor;

/**
 * This is the basic object for location task for quests. 
 */

UCLASS(Abstract, Blueprintable)
class QUESTEDITOR_API UBaseQuestTask_Location : public UBaseQuestTask, public IQuestTaskInterface
{
	GENERATED_BODY()

public:

	UBaseQuestTask_Location();


	//--------------------------------------------Variables
	/*The current character used for overlap*/
	UPROPERTY()
	ACharacter* CurrentCharacter = nullptr;

	/*Delegate handle for new pawn possesion*/
	FDelegateHandle OnNewPawnHandle = FDelegateHandle();

	//--------------------------------------------Functions

protected:

	/**
	*	Overrideable function to get the character that reaches the quest location
	*	By default is the pawn that is controlled by the player state owner of the quest manager.
	*/
	UFUNCTION(BlueprintNativeEvent, Category = "Base Quest Task Location")
		ACharacter* GetInteractingCharacter();

	/**Overrideable function with the condition that has to be met in order to commit this task succesfully.*/
	UFUNCTION(BlueprintNativeEvent, Category = "Base Quest Task Location")
		bool  OnQuestLocationReachedCondition(ABaseQuestLocationActor* QuestLocationActorIn);

	/**Overrideable function with the logic after overlapping, with potentially can be reaching a location.*/
	UFUNCTION()
		virtual void  OnOverlapEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/**Overrideable function with the logic after changing characters.*/
		virtual void  OnNewPawnEvent(APawn* NewPawn);

	/*Function that binds to the character overlap delegates.*/
	UFUNCTION()
		void  BindOverlapCharacterDelegates(ACharacter* NewCharacter);

	//-----------------------------Base Quest Task Overide

	/*Logic to bind and reach locations*/
	void  OnQuestTaskActivated_Implementation() override;

	/**Overrideable function with the task logic when loading the game. Will only execute for loaded Active tasks.*/
	void  OnQuestTaskLoaded_Implementation() override;

};
