#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Events/CameraSwitchEvent.h" // Für EShoulderSide
#include "CameraSettingPreset.generated.h"

UENUM(BlueprintType)
enum class ECameraFocusType : uint8
{
	Manual      UMETA(DisplayName = "Manual Distance"),
	Tracking    UMETA(DisplayName = "Track TargetActor")
};

UCLASS(BlueprintType)
class DIALOGUESYSTEM_API UCameraSettingPreset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Position")
	float CameraDistance = 500.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Position")
	float CameraHeight = 120.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Position")
	float ShoulderOffset = 250.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Position")
	EShoulderSide ShoulderSide = EShoulderSide::Right;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Lens")
	float FocalLength = 35.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Lens")
	float FStop = 2.0f;
};