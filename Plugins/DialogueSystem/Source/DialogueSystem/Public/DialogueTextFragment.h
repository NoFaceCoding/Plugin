#pragma once

#include "CoreMinimal.h"
#include "DialogueEvent.h"
#include "DialogueTextFragment.generated.h"

USTRUCT(BlueprintType)
struct FDialogueTextFragment
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsText = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(EditCondition="bIsText"))
	FText Text;

	UPROPERTY(EditAnywhere, Instanced, BlueprintReadWrite, meta=(EditCondition="!bIsText"))
	UDialogueEvent* EmbeddedEvent;

	/** Optionaler Delay nach diesem Fragment, bevor das nächste abgespielt wird (Sekunden) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timing")
	float PostDelay = 0.1f;
};