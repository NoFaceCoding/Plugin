//Copyright 2020 Marchetti S. Alfredo I. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"


#include "Quest/QuestTypes.h"
#include "QuestEditor/QuestTypesDev.h"

#include "EditorUtilityWidget.h"
#include "ErrorManagerWidget.generated.h"

/**
 * 
 */
UCLASS()
class QUESTEDITORDEV_API UErrorManagerWidget : public UEditorUtilityWidget
{
	GENERATED_BODY()
	
public:

	/**
	*	Gets all error for this task.
	*	This function will call each of the error objects and execute their error functions.
	*/
	UFUNCTION(BlueprintCallable, Category = "Quest Task Error")
		void  GetAllErrors(const FQuestTask& QuestTaskIn, const FQuestTaskEditorInfo& TaskInfoIn, TArray <FString>& ErrorOut, TArray <FLinearColor>& ErrorColorOut) const;

	/**
	*	Gets all error for this talk task.
	*	This function will call each of the error objects and execute their error functions.
	*/
	UFUNCTION(BlueprintCallable, Category = "Quest Task Error")
		void  GetAllTalkErrors(const FQuestTalkTask& QuestTaskIn, const FQuestTalkTaskEditorInfo& TalkTaskInfoIn, bool IsDialogIn, TArray <FString>& ErrorOut, TArray <FLinearColor>& ErrorColorOut) const;
};
