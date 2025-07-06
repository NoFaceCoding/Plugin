#pragma once

#include "CoreMinimal.h"
#include "DialogueEvent.h"
#include "DialogueTextFragment.h"
#include "TextDialogueEvent.generated.h"

class UDialogueWidgetBase;

UCLASS(BlueprintType, EditInlineNew)
class DIALOGUESYSTEM_API UTextDialogueEvent : public UDialogueEvent
{
	GENERATED_BODY()

public:
	virtual void Execute_Implementation(class UDialogueComponent* DialogueComponent, bool bIsEmbedded = false) override;

	UFUNCTION()
	void NotifyTextFinished();
	void TryContinue();
	bool bIsActive = false;
	
protected:
	UFUNCTION()
	void PlayNextFragment();

	UPROPERTY(EditAnywhere, Category = "Dialogue")
	TSubclassOf<UDialogueWidgetBase> DialogueWidgetClass;

	UPROPERTY(EditAnywhere, Category = "Dialogue")
	FName SpeakingCharacterID;

	UPROPERTY(EditAnywhere, Category = "Dialogue")
	TArray<FDialogueTextFragment> Fragments;

	UPROPERTY()
	UDialogueComponent* CachedComponent = nullptr;

	UPROPERTY()
	UDialogueWidgetBase* CurrentWidget = nullptr;

	int32 CurrentFragmentIndex = 0;
	FTimerHandle PostDelayHandle;
	bool bIsSkipping = false;
};