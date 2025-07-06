//Copyright 2020 Marchetti S. Alfredo I. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Quest/QuestTypes.h"


#include "BaseQuestStartLocationActor.generated.h"


class USceneComponent;
class USphereComponent;
class UNPCManager;
class UQuestManager;


UCLASS()
class QUESTEDITOR_API ABaseQuestStartLocationActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseQuestStartLocationActor(const FObjectInitializer& ObjectInitializer);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
		
private:	

	//--------------------------Variables

	/** Scene root.*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Scene Component", meta = (AllowPrivateAccess = "true"))
		USceneComponent* SceneComponent;

	/** The are in wich the quests will be given on overlap*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Location Sphere", meta = (AllowPrivateAccess = "true"))
		USphereComponent* LocationArea;

	/** The NPC manager reference for this location. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "NPC", meta = (AllowPrivateAccess = "true"))
		UNPCManager* NPCManagerReference;
	
	//--------------------------------------------------------------------------
	//-------------------------------------FUNCTIONS
	//--------------------------------------------------------------------------

	//-----------------------Overlap spheres
	UFUNCTION()
		void OnBeginOverlap_LocationSphere(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:

	/**Overrideable function with the logic after overlapping, for this actor in particular, it will try to give the quests to the player.*/
	UFUNCTION(BlueprintNativeEvent, Category = "Quest Start Location Actor")
		void  OnOverlapActorEvent(AActor* OtherActor);

	/**
	*	Overrideable function for a condition to check before executing the OnOverlapEvent.
	*	You can use this condition to filter actor overlaps that you dont want to execute overlap logic.
	*	True by default.
	*/
	UFUNCTION(BlueprintNativeEvent, Category = "Quest Start Location Actor")
		bool  OnOverlapCondition(AActor* OtherActor);

	/**
	*	Overrideable function to get the quest manager reference from the overlapping actor.
	*	By default considers the quest manager is in the player state, and the actor is a character pawn.
	*/
	UFUNCTION(BlueprintNativeEvent, Category = "Quest Start Location Actor")
		UQuestManager* GetQuestManagerReference(AActor* OverlappingActor);
};
