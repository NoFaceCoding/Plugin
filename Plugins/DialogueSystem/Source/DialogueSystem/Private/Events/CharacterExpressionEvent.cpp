#include "Events/CharacterExpressionEvent.h"
#include "DialogueComponent.h"
#include "DialogueDataAsset.h"
#include "DialogueCharacter.h"
#include "Interfaces/CharacterExpressionInterface.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"

void UCharacterExpressionEvent::Execute_Implementation(UDialogueComponent* DialogueComponent, bool bIsEmbedded)
{
	if (!DialogueComponent) return;

	const UDialogueDataAsset* DialogueAsset = DialogueComponent->GetCurrentDialogue();
	if (!DialogueAsset)
	{
		UE_LOG(LogTemp, Warning, TEXT("CharacterExpressionEvent: Kein DialogueAsset."));
		return;
	}

	const FDialogueCharacter& CharInfo = DialogueAsset->GetCharacterByID(CharacterID);

	if (!CharInfo.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("CharacterExpressionEvent: Charakter-ID '%s' nicht im Asset gefunden."), *CharacterID.ToString());
		return;
	}

	// Actor in der Szene über ActorTag suchen
	UWorld* World = DialogueComponent->GetWorld();
	if (!World) return;

	AActor* TargetActor = nullptr;
	for (TActorIterator<AActor> It(World); It; ++It)
	{
		if (It->ActorHasTag(CharInfo.ActorTag))
		{
			TargetActor = *It;
			break;
		}
	}

	if (!TargetActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("CharacterExpressionEvent: Kein Actor mit Tag '%s' gefunden."), *CharInfo.ActorTag.ToString());
		return;
	}

	// Prüfen, ob Interface implementiert ist
	if (TargetActor->GetClass()->ImplementsInterface(UCharacterExpressionInterface::StaticClass()))
	{
		ICharacterExpressionInterface::Execute_PlayExpression(TargetActor, ExpressionID);
		UE_LOG(LogTemp, Log, TEXT("CharacterExpressionEvent: Ausdruck '%s' auf '%s' abgespielt."), *ExpressionID.ToString(), *TargetActor->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("CharacterExpressionEvent: Actor '%s' implementiert das Interface nicht."), *TargetActor->GetName());
	}
	if (!bIsEmbedded && DialogueComponent)
	{
		DialogueComponent->ContinueDialogue();
	}
}