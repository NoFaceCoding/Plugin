//Copyright 2020 Marchetti S. Alfredo I. All Rights Reserved.


#include "Tooltip/BaseTooltipContentWidget.h"
#include "QuestEditor/QuestEditorDevFunctionLibrary.h"


UBaseTooltipContentWidget::UBaseTooltipContentWidget() : Super()
{
}

void UBaseTooltipContentWidget::NativeConstruct()
{
	//Get quest editor reference
	QuestEditorReference = UQuestEditorDevFunctionLibrary::FindQuestEditorReference(this);
	//Update on start with payload exposed on spawn
	UpdateFromPayload(Payload);

	Super::NativeConstruct();
}


void UBaseTooltipContentWidget::UpdateFromPayload(const FQuestTaskPayload& PayloadIn)
{
	//Update payload
	Payload = PayloadIn;
	//Call update event
	UpdateTooltipWidget();
}

