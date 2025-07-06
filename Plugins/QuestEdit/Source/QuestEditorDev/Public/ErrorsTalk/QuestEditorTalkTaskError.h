//Copyright 2020 Marchetti S. Alfredo I. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"

#include "Quest/QuestTypes.h"
#include "QuestEditor/QuestTypesDev.h"

#include "QuestEditorTalkTaskError.generated.h"

/**
 *	Base object to analize talk tasks for errors inside the quest editor.
 */


UCLASS(Abstract, BlueprintType, Blueprintable, Meta = (DisplayName = "Talk Task Error"))
class QUESTEDITORDEV_API UQuestEditorTalkTaskError : public UObject
{

	GENERATED_BODY()

public:
	
	UQuestEditorTalkTaskError();
	

	/**
	*	Overrideable function with the logic to determine errors from Talk Task.
	*	If returns null string it will not show the error.
	*/
	UFUNCTION(BlueprintNativeEvent, Category = "Quest Task Error")
		void GetErrorFromTask(const FQuestTalkTask& QuestTaskIn, bool IsDialogIn, FString& ErrorTextOut, FLinearColor& ErrorColorOut) const;

};
