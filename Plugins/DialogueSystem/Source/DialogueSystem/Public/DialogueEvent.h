// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "DialogueEvent.generated.h"

/**
 * 
 */
UCLASS(Abstract, BlueprintType, EditInlineNew)
class DIALOGUESYSTEM_API UDialogueEvent : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent)
	void Execute(UDialogueComponent* DialogueComponent, bool bIsEmbedded = false);
	virtual void Execute_Implementation(UDialogueComponent* DialogueComponent, bool bIsEmbedded = false);
};
