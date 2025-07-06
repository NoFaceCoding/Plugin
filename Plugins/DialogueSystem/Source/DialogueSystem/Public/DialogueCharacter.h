#pragma once

#include "CoreMinimal.h"
#include "EngineUtils.h"
#include "DialogueCharacter.generated.h"

USTRUCT(BlueprintType)
struct FDialogueCharacter
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName CharacterID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName ActorTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName LookAtBone = "head";

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName CameraActorTag;

	// Hilfswert zur Prüfung auf Gültigkeit
	bool IsValid() const { return !CharacterID.IsNone(); }
	
	AActor* FindActorInWorld(UWorld* World) const
	{
		if (!World || ActorTag.IsNone())
			return nullptr;

		for (TActorIterator<AActor> It(World); It; ++It)
		{
			if (It->ActorHasTag(ActorTag))
			{
				return *It;
			}
		}
		return nullptr;
	}
};