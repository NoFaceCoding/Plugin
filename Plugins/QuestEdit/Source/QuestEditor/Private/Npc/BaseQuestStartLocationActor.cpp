//Copyright 2020 Marchetti S. Alfredo I. All Rights Reserved.


#include "Npc/BaseQuestStartLocationActor.h"
#include "Npc/NPCManager.h"
#include "Quest/QuestManager.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/Pawn.h"
#include "Components/SphereComponent.h"
#include "Runtime/Engine/Classes/Components/SceneComponent.h"

ABaseQuestStartLocationActor::ABaseQuestStartLocationActor(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	//root
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComp"));
	SetRootComponent(SceneComponent);

	//Area sphere
	LocationArea = CreateDefaultSubobject<USphereComponent>(TEXT("LocationSphereComponent"));
	LocationArea->SetupAttachment(SceneComponent);
	LocationArea->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	LocationArea->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	LocationArea->SetGenerateOverlapEvents(true);
	LocationArea->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	LocationArea->SetSphereRadius(500);

	//NPCManager
	NPCManagerReference = CreateDefaultSubobject<UNPCManager>(TEXT("NPCManagerComponent"));

}

void ABaseQuestStartLocationActor::BeginPlay()
{
	Super::BeginPlay();

	if (!LocationArea->OnComponentBeginOverlap.IsAlreadyBound(this, &ABaseQuestStartLocationActor::OnBeginOverlap_LocationSphere))
	{
		LocationArea->OnComponentBeginOverlap.AddDynamic(this, &ABaseQuestStartLocationActor::OnBeginOverlap_LocationSphere);
	}
	
}

void ABaseQuestStartLocationActor::OnBeginOverlap_LocationSphere(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//Check condition on overlap
	if (!OnOverlapCondition(OtherActor)) return;

	//Accept quest directly for all the contained ones in the NPC manager
	OnOverlapActorEvent(OtherActor);
}

void ABaseQuestStartLocationActor::OnOverlapActorEvent_Implementation(AActor* OtherActor)
{
	//Get Quest Manager
	UQuestManager* QuestManagerLocal = GetQuestManagerReference(OtherActor);
	//if the actor is not a quest manager holder, dont do anything.
	if (!QuestManagerLocal) return;

	//Accept all quests directly for the manager.
	TArray <FQuestRep> NPCQuests = NPCManagerReference->GetNPCQuestsRep();

	//For each quest, Accept directly
	for (int i = 0; i < NPCQuests.Num(); i++)
	{
		QuestManagerLocal->Client_AcceptQuestFromStartActor(NPCQuests[i]);
	}
}

bool ABaseQuestStartLocationActor::OnOverlapCondition_Implementation(AActor* OtherActor)
{
	return true;
}

UQuestManager* ABaseQuestStartLocationActor::GetQuestManagerReference_Implementation(AActor* OverlappingActor)
{
	//Get Player State of the widget
	APawn* PawnReference = Cast<APawn>(OverlappingActor);
	if (!PawnReference) return nullptr;
		
	APlayerState* OwnerState = PawnReference->GetPlayerState();
	if (!OwnerState) return nullptr;

	//Get Quest Manager
	UQuestManager* QuestManagerComponent = nullptr;
	QuestManagerComponent = Cast<UQuestManager>(OwnerState->FindComponentByClass(UQuestManager::StaticClass()));

	return QuestManagerComponent;
}