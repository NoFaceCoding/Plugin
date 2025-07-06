//Copyright 2020 Marchetti S. Alfredo I. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "EditorUtilityWidget.h"
#include "Quest/QuestTypes.h"


#include "BaseTooltipContentWidget.generated.h"

class UBaseQuestEditor;

/**
 *	Base widget for populating the tooltip inside the quest editor.
 * 
 * 
 *	Uses a payload to update.
 */
UCLASS(Abstract, Blueprintable, Meta = (DisplayName = "TooltipContent"))
class QUESTEDITORDEV_API UBaseTooltipContentWidget : public  UEditorUtilityWidget
{
	GENERATED_BODY()
	

public:

	UBaseTooltipContentWidget();

	virtual void NativeConstruct() override;

	/*QuestTask info for updating the tooltip.*/
	UPROPERTY(meta = (ExposeOnSpawn = "true"), BlueprintReadOnly, Category = "Preview Widget")
		FQuestTaskPayload Payload = FQuestTaskPayload();

	/*Quest editor reference for calling events and getting datatable information.*/
	UPROPERTY(BlueprintReadOnly, Category = "Preview Widget")
		UBaseQuestEditor* QuestEditorReference = nullptr;

	/**
	*	Main update event for the widget.
	*	Call to update title info and widget.
	*	Updates Quest Task variable and calls update event.
	*/
	UFUNCTION(BlueprintCallable, Category = "Preview Widget")
		void UpdateFromPayload(const FQuestTaskPayload& PayloadIn);

	/**
	*	Main update event for the widget.
	*	All the logic needed for updating this widget should be placed here.
	*	All tooltip content widgets must override this function.
	*	This function will be called by the editor automatically when needed.
	*	Payload variable contains current data.
	*/
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Preview Widget")
		void UpdateTooltipWidget();

};
