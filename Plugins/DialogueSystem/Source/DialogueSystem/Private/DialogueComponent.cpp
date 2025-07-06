#include "DialogueComponent.h"
#include "DialogueEvent.h"
#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "CineCameraComponent.h"
#include "Events/TextDialogueEvent.h"
#include "Kismet/GameplayStatics.h"

class UTextDialogueEvent;
class UCameraComponent;

UDialogueComponent::UDialogueComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UDialogueComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UDialogueComponent::StartDialogue(UDialogueDataAsset* InDialogue)
{
	if (!InDialogue) return;

	CurrentDialogue = InDialogue;
	CurrentEventIndex = 0;

	PlayNextEvent();
}

void UDialogueComponent::PlayNextEvent()
{
	if (!CurrentDialogue || CurrentEventIndex >= CurrentDialogue->Events.Num())
	{
		// Dialog ist zu Ende
		CurrentDialogue = nullptr;

		// Kamera zurücksetzen
		APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
		if (PC)
		{
			APawn* PlayerPawn = PC->GetPawn();
			if (PlayerPawn)
			{
				PC->SetViewTargetWithBlend(PlayerPawn, 0.5f); // Blende zurück zur Spieler-Kamera
			}
		}
		return;
	}

	if (UDialogueEvent* Event = CurrentDialogue->Events[CurrentEventIndex])
	{
		Event->Execute(this, false);
	}
}

void UDialogueComponent::ContinueDialogue()
{
	// Check: Ist ein aktives TextEvent im Gang?
	if (CurrentDialogue && CurrentEventIndex < CurrentDialogue->Events.Num())
	{
		if (UTextDialogueEvent* TextEvent = Cast<UTextDialogueEvent>(CurrentDialogue->Events[CurrentEventIndex]))
		{
			if (TextEvent->bIsActive)
			{
				// Spieler möchte das aktuelle TextEvent skippen oder abschließen
				TextEvent->TryContinue();
				return;
			}
		}
	}

	// Widget aufräumen (sicherheitshalber)
	if (ActiveDialogueWidget)
	{
		ActiveDialogueWidget->RemoveFromParent();
		ActiveDialogueWidget = nullptr;
	}

	// Nächsten Event starten
	++CurrentEventIndex;
	PlayNextEvent();
}

void UDialogueComponent::SetActiveDialogueWidget(UUserWidget* Widget)
{
	if (ActiveDialogueWidget && ActiveDialogueWidget->IsInViewport())
	{
		ActiveDialogueWidget->RemoveFromParent();
	}

	ActiveDialogueWidget = Widget;
}

AActor* UDialogueComponent::SpawnTemporaryCamera(const FVector& Location, const FRotator& Rotation)
{
	UWorld* World = GetWorld();
	if (!World) return nullptr;

	FActorSpawnParameters Params;
	Params.Owner = GetOwner();
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AActor* CamActor = World->SpawnActor<AActor>(AActor::StaticClass(), Location, Rotation, Params);
	if (!CamActor) return nullptr;

	// Root-Komponente hinzufügen
	USceneComponent* Root = NewObject<USceneComponent>(CamActor);
	Root->RegisterComponent();
	CamActor->SetRootComponent(Root);
	Root->SetWorldLocationAndRotation(Location, Rotation);

	// CineCamera hinzufügen
	UCineCameraComponent* CameraComp = NewObject<UCineCameraComponent>(CamActor);
	CameraComp->RegisterComponent();
	CameraComp->AttachToComponent(Root, FAttachmentTransformRules::KeepRelativeTransform);
	CameraComp->SetRelativeLocation(FVector::ZeroVector);
	// Entferne Film-Look-Effekt (schwarze Balken vermeiden)
	CameraComp->SetConstraintAspectRatio(false);

	// Optional: Voreinstellungen für DOF
	CameraComp->FocusSettings.FocusMethod = ECameraFocusMethod::Manual;
	CameraComp->FocusSettings.ManualFocusDistance = 300.f;

	return CamActor;
}