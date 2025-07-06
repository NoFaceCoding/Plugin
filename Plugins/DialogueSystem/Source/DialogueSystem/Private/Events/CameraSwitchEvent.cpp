#include "Events/CameraSwitchEvent.h"

#include "CameraSettingPreset.h"
#include "DialogueComponent.h"
#include "DialogueDataAsset.h"
#include "DialogueCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Camera/CameraComponent.h"
#include "CineCameraComponent.h"
#include "EngineUtils.h"

void UCameraSwitchEvent::Execute_Implementation(UDialogueComponent* DialogueComponent, bool bIsEmbedded)
{
	if (!DialogueComponent) return;

	const UDialogueDataAsset* Dialogue = DialogueComponent->GetCurrentDialogue();
	if (!Dialogue) return;

	const FDialogueCharacter& TargetChar = Dialogue->GetCharacterByID(TargetCharacterID);
	const FDialogueCharacter& StartChar  = Dialogue->GetCharacterByID(CameraStartCharacterID);

	if (!TargetChar.IsValid() || !StartChar.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("[CameraSwitchEvent] Ungültige Character IDs."));
		if (!bIsEmbedded) DialogueComponent->ContinueDialogue();
		return;
	}

	AActor* TargetActor = TargetChar.FindActorInWorld(DialogueComponent->GetWorld());
	AActor* StartActor  = StartChar.FindActorInWorld(DialogueComponent->GetWorld());

	if (!TargetActor || !StartActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("[CameraSwitchEvent] Actor nicht gefunden."));
		if (!bIsEmbedded) DialogueComponent->ContinueDialogue();
		return;
	}
	const float CameraDistance = CameraPreset ? CameraPreset->CameraDistance : 500.f;
	const float CameraHeight   = CameraPreset ? CameraPreset->CameraHeight : 120.f;
	const float ShoulderOffset = CameraPreset ? CameraPreset->ShoulderOffset : 250.f;
	const EShoulderSide ShoulderSide   = CameraPreset ? CameraPreset->ShoulderSide : EShoulderSide::Right;
	const float FStop = CameraPreset ? CameraPreset->FStop : 2.f;
	const float CurrentFocalLength = CameraPreset ? CameraPreset->FocalLength : 35.f;
	
	// Kamera über Schulter des StartCharakters platzieren
	FVector StartLocation = StartActor->GetActorLocation();
	FVector TargetLocation = TargetActor->GetActorLocation();

	FVector DirectionToTarget = (TargetLocation - StartLocation).GetSafeNormal();
	// Basis
	FVector BaseOffset = -DirectionToTarget * CameraDistance + FVector(0, 0, CameraHeight);
    
    // Quer-Vektor berechnen (seitlicher Versatz, rechts/links von Blickrichtung aus)
	FVector RightVector = FVector::CrossProduct(FVector::UpVector, DirectionToTarget).GetSafeNormal();
    
    // Schulterversatz
    if (ShoulderSide == EShoulderSide::Left)
    {
    	BaseOffset += -RightVector * ShoulderOffset;
    }
    else
    {
    	BaseOffset += RightVector * ShoulderOffset;
    }
    
    FVector CameraPosition = StartLocation + BaseOffset;
	FRotator CameraRotation = UKismetMathLibrary::FindLookAtRotation(CameraPosition, TargetLocation);

	// Dynamische Kamera erzeugen
	AActor* CameraActor = DialogueComponent->SpawnTemporaryCamera(CameraPosition, CameraRotation);
	if (!CameraActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("[CameraSwitchEvent] Kamera konnte nicht erzeugt werden."));
		if (!bIsEmbedded) DialogueComponent->ContinueDialogue();
		return;
	}
	if (CameraActor)
	{
		UCineCameraComponent* CineCam = CameraActor->FindComponentByClass<UCineCameraComponent>();
		if (CineCam && TargetActor)
		{
			const float Distance = FVector::Distance(CameraActor->GetActorLocation(), TargetActor->GetActorLocation());

			CineCam->FocusSettings.FocusMethod =  ECameraFocusMethod::Tracking;
			CineCam->FocusSettings.TrackingFocusSettings.ActorToTrack = TargetActor;
			CineCam->FocusSettings.TrackingFocusSettings.RelativeOffset = FVector::ZeroVector;
			CineCam->FocusSettings.TrackingFocusSettings.bDrawDebugTrackingFocusPoint = false;
			CineCam->LensSettings.MinFStop = FStop;
			CineCam->CurrentFocalLength = CurrentFocalLength; // für Portrait-Look
		}
	}

	// Kamera aktivieren
	APlayerController* PC = UGameplayStatics::GetPlayerController(DialogueComponent, 0);
	if (PC)
	{
		PC->SetViewTargetWithBlend(CameraActor, BlendTime);
	}

	if (!bIsEmbedded)
	{
		DialogueComponent->ContinueDialogue();
	}
}