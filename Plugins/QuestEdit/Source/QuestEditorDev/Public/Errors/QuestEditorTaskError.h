//Copyright 2020 Marchetti S. Alfredo I. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/CoreUObject/Public/UObject/Object.h"
#include "Quest/QuestTypes.h"
#include "QuestEditor/QuestTypesDev.h"

#include "QuestEditorTaskError.generated.h"

/**
 *	Base object to analize tasks for errors inside the quest editor.
 */


UCLASS(Abstract, BlueprintType, Blueprintable, Meta = (DisplayName = "Task Error"))
class QUESTEDITORDEV_API UQuestEditorTaskError : public UObject
{

	GENERATED_BODY()

public:
	
	UQuestEditorTaskError();
	

	/**
	*	Overrideable function with the logic to determine errors from Task.
	*	If returns null string it will not show the error.
	*/
	UFUNCTION(BlueprintNativeEvent, Category = "Quest Task Error")
		void  GetErrorFromTask(const FQuestTask& QuestTaskIn, const FQuestTaskEditorInfo& TaskInfoIn, FString& ErrorTextOut, FLinearColor& ErrorColorOut) const;

};
