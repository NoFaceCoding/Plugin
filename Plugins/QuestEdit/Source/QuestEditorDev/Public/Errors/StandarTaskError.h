//Copyright 2020 Marchetti S. Alfredo I. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"


#include "Errors/QuestEditorTaskError.h"
#include "StandarTaskError.generated.h"

/**
 *		Standar error list function. Detects if a selector does not have its property set correctly.
 */
UCLASS(BlueprintType, Blueprintable)
class QUESTEDITORDEV_API UStandarTaskError : public UQuestEditorTaskError
{
	GENERATED_BODY()
	
public:

	void  GetErrorFromTask_Implementation(const FQuestTask& QuestTaskIn, const FQuestTaskEditorInfo& TaskInfoIn, FString& ErrorTextOut, FLinearColor& ErrorColorOut) const override;
	
};
