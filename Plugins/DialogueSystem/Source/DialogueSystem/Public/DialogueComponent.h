#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DialogueDataAsset.h"
#include "DialogueComponent.generated.h"

class UDialogueEvent;
class UUserWidget;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DIALOGUESYSTEM_API UDialogueComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UDialogueComponent();

	/** Startet den Dialog anhand des übergebenen DataAssets */
	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	void StartDialogue(UDialogueDataAsset* InDialogue);

	/** Wird aufgerufen, um den Dialogfluss manuell fortzusetzen */
	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	void ContinueDialogue();

	/** Liefert das aktuell aktive Dialogue-Asset */
	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	const UDialogueDataAsset* GetCurrentDialogue() const { return CurrentDialogue; }

	/** Setzt das aktuell aktive Widget für automatisches Entfernen */
	void SetActiveDialogueWidget(UUserWidget* Widget);
	
	UFUNCTION()
	AActor* SpawnTemporaryCamera(const FVector& Location, const FRotator& Rotation);

protected:
	virtual void BeginPlay() override;

	/** Spielt das nächste Event im Asset ab */
	void PlayNextEvent();

	/** Aktuelles aktives Widget (z. B. Dialogue UI) */
	UPROPERTY()
	UUserWidget* ActiveDialogueWidget = nullptr;

	UPROPERTY()
	UDialogueDataAsset* CurrentDialogue = nullptr;

	int32 CurrentEventIndex = 0;
};