//Copyright 2020 Marchetti S. Alfredo I. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityWidget.h"

#include "QuestEditor/QuestTypesDev.h"
#include "Quest/QuestTypes.h"

#include "BaseTalkTaskContentPreviewWidget.generated.h"

class UBaseQuestEditor;

/**
 *	Base preview widget, contains logic for talk task content preview slots.
 */

UCLASS(Abstract, Meta = (DisplayName = "TalkTaskContentPreview"))
class QUESTEDITORDEV_API UBaseTalkTaskContentPreviewWidget : public UEditorUtilityWidget
{
	GENERATED_BODY()
	
		
public:

	UBaseTalkTaskContentPreviewWidget();

	virtual void NativeConstruct() override;

	/*Quest talk task ID. Use to update widget.*/
	UPROPERTY(meta = (ExposeOnSpawn = "true"), BlueprintReadOnly, Category = "Preview Widget")
		FQuestTaskID TalkTaskID = FQuestTaskID();

	/*Quest talk task current info. Use to update widget.*/
	UPROPERTY(meta = (ExposeOnSpawn = "true"), BlueprintReadOnly, Category = "Preview Widget")
		FQuestTalkTask QuestTalkTask = FQuestTalkTask();

	/*Quest editor reference for calling events and getting datatable information.*/
	UPROPERTY(BlueprintReadOnly, Category = "Preview Widget")
		UBaseQuestEditor* QuestEditorReference = nullptr;

	/**
	*	Main update event for the widget.
	*	Call to update title info and widget.
	*	Updates Quest Task variable and calls update event.
	*/
	UFUNCTION(BlueprintCallable, Category = "Preview Widget")
		void UpdateFromTask(const FQuestTalkTask& QuestTalkTaskIn, const FQuestTaskID& TaslTaskIDIn);

	/**
	*	Main update event for the widget.
	*	All the logic needed for updating this widget should be placed here.
	*	All preview widgets must override this function.
	*	This function will be called by the editor automatically when needed.
	*	QuestTask variable contains current data.
	*/
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Preview Widget")
		void UpdatePreviewWidget();

};
