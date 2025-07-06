#pragma once

#include "CoreMinimal.h"
#include "DialogueEvent.h"
#include "CharacterExpressionEvent.generated.h"

UCLASS(BlueprintType, EditInlineNew)
class DIALOGUESYSTEM_API UCharacterExpressionEvent : public UDialogueEvent
{
	GENERATED_BODY()

public:
	// Referenz auf Character im DataAsset
	UPROPERTY(EditAnywhere, Category = "Dialogue")
	FName CharacterID;

	// Ausdrucks-ID, z. B. "Smile", "Nod", "Frown"
	UPROPERTY(EditAnywhere, Category = "Dialogue")
	FName ExpressionID;

	virtual void Execute_Implementation(class UDialogueComponent* DialogueComponent, bool bIsEmbedded) override;
};