//Copyright 2020 Marchetti S. Alfredo I. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityWidget.h"

#include "QuestEditor/QuestTypesDev.h"
#include "Quest/QuestTypes.h"

#include "BasePreviewWidget.generated.h"

class UBaseQuestEditor;

/**
 *	Base preview widget, contains shared logic for title and content preview slots.
 */

UCLASS(Abstract)
class QUESTEDITORDEV_API UBasePreviewWidget : public UEditorUtilityWidget
{
	GENERATED_BODY()
	
		
public:

	UBasePreviewWidget();

	virtual void NativeConstruct() override;

	/*Quest task ID. Use to update widget.*/
	UPROPERTY(meta = (ExposeOnSpawn = "true"), BlueprintReadOnly, Category = "Preview Widget")
		FQuestTaskID TaskID = FQuestTaskID();

	/*Quest task current info. Use to update widget.*/
	UPROPERTY(meta = (ExposeOnSpawn = "true"), BlueprintReadOnly, Category = "Preview Widget")
		FQuestTask QuestTask = FQuestTask();

	/*Quest editor reference for calling events and getting datatable information.*/
	UPROPERTY(BlueprintReadOnly, Category = "Preview Widget")
		UBaseQuestEditor* QuestEditorReference = nullptr;

	/**
	*	Main update event for the widget.
	*	Call to update title info and widget.
	*	Updates Quest Task variable and calls update event.
	*/
	UFUNCTION(BlueprintCallable, Category = "Preview Widget")
		void UpdateFromTask(const FQuestTask& QuestTaskIn);

	/**
	*	Main update event for the widget.
	*	All the logic needed for updating this widget should be placed here.
	*	All preview widgets must override this function.
	*	This function will be called by the editor automatically when needed.
	*	QuestTask variable contains current data.
	*/
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Preview Widget")
		void UpdatePreviewWidget();

	/**
	*	Gets current task information for quest editor.
	*/
	UFUNCTION(BlueprintCallable, Category = "Preview Slot")
		FQuestTaskEditorInfo GetTaskInfo();
};
