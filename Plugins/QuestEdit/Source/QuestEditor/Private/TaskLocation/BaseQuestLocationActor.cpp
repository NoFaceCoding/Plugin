//Copyright 2020 Marchetti S. Alfredo I. All Rights Reserved.


#include "TaskLocation/BaseQuestLocationActor.h"

// Sets default values
ABaseQuestLocationActor::ABaseQuestLocationActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ABaseQuestLocationActor::BeginPlay()
{
	Super::BeginPlay();
	
}

FString ABaseQuestLocationActor::GetQuestLocationName() const
{
	return QuestLocationName;
}

