#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DialogueWidgetBase.generated.h"

class UTextDialogueEvent;

UCLASS()
class DIALOGUESYSTEM_API UDialogueWidgetBase : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	void AppendVisibleText(const FText& NewText);

	void SetOwningTextEvent(UTextDialogueEvent* Event)
	{
		OwningTextEvent = Event;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	float CharacterDisplayInterval = 0.03f;

	UFUNCTION(BlueprintImplementableEvent, Category = "Dialogue")
	void UpdateVisibleText(const FText& NewText);

	UFUNCTION(BlueprintImplementableEvent, Category = "Dialogue")
	void SetSpeakerName(const FText& Name);

	UFUNCTION(BlueprintCallable)
	void InstantComplete();

	UFUNCTION(BlueprintCallable)
	bool IsTextFullyDisplayed() const;
protected:

	virtual void NativeDestruct() override;

	void AppendNextCharacter();


	FString FullText;
	FString CurrentText;
	int32 CurrentCharIndex = 0;

	UPROPERTY()
	UTextDialogueEvent* OwningTextEvent = nullptr;

	FTimerHandle TextDisplayTimer;
};