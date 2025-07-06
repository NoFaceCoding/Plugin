//Copyright 2020 Marchetti S. Alfredo I. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"


#include "GameplayTagContainer.h"


#include "BaseQuestLocationActor.generated.h"

UCLASS()
class QUESTEDITOR_API ABaseQuestLocationActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseQuestLocationActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	//--------------------------Variables

	/**
	*	Location name.
	*	Will be used for tasks and also used to show the player in UI.
	*/
	UPROPERTY(EditAnywhere, Category = "Quest Location")
		FString QuestLocationName = "Base Quest Location";

	/**
	*	Location tags.
	*	Will be used for tasks.
	*/
	UPROPERTY(EditAnywhere, Category = "Quest Location")
		FGameplayTagContainer QuestLocationTags;


	/**
	*	Get the location name.
	*/
	UFUNCTION(BlueprintCallable, Category = "NPC Start")
		FString GetQuestLocationName() const;
};
