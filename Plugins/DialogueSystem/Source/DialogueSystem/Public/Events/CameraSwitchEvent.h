#pragma once

#include "CoreMinimal.h"
#include "DialogueEvent.h"
#include "CameraSwitchEvent.generated.h"


class UCameraSettingPreset;

UENUM(BlueprintType)
enum class EShoulderSide : uint8
{
	Right	UMETA(DisplayName = "Rechte Schulter"),
	Left	UMETA(DisplayName = "Linke Schulter")
};

/**
 * Dieses Event blendet zur Kamera, die sich dynamisch über der Schulter von StartCharacter befindet
 * und auf TargetCharacter schaut. Die Kamera wird automatisch erzeugt.
 */
UCLASS(BlueprintType, EditInlineNew)
class DIALOGUESYSTEM_API UCameraSwitchEvent : public UDialogueEvent
{
	GENERATED_BODY()

public:
	virtual void Execute_Implementation(class UDialogueComponent* DialogueComponent, bool bIsEmbedded = false) override;

	/** Vordefinierte Kameraeinstellungen (optional) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	TObjectPtr<UCameraSettingPreset> CameraPreset;
	
	/** Charakter, der im Fokus stehen soll */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	FName TargetCharacterID;

	/** Charakter, über dessen Schulter geschaut werden soll */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	FName CameraStartCharacterID;
	
	/** Blendzeit in Sekunden */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float BlendTime = 0.5f;
};