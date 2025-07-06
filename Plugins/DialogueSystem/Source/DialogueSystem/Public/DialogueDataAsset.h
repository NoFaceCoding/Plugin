#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DialogueEvent.h"
#include "DialogueCharacter.h"
#include "DialogueDataAsset.generated.h"

UCLASS(BlueprintType)
class DIALOGUESYSTEM_API UDialogueDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	// Reihenfolge der Events im Dialog
	UPROPERTY(EditAnywhere, Instanced, BlueprintReadOnly, Category = "Dialogue")
	TArray<UDialogueEvent*> Events;
	
	// Liste der beteiligten Charaktere
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dialogue")
	TArray<FDialogueCharacter> Characters;

	// Sucht Charakterdaten anhand ID
	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	const FDialogueCharacter& GetCharacterByID(FName ID) const;
};