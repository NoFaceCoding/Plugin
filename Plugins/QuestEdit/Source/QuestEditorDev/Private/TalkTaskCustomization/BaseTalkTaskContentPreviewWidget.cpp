//Copyright 2020 Marchetti S. Alfredo I. All Rights Reserved.


#include "TalkTaskCustomization/BaseTalkTaskContentPreviewWidget.h"


#include "QuestEditor/BaseQuestEditor.h"
#include "QuestEditor/QuestEditorDevFunctionLibrary.h"


UBaseTalkTaskContentPreviewWidget::UBaseTalkTaskContentPreviewWidget() : Super()
{
}

void UBaseTalkTaskContentPreviewWidget::NativeConstruct()
{
	//Get quest editor reference
	QuestEditorReference = UQuestEditorDevFunctionLibrary::FindQuestEditorReference(this);
	//Update on start with task exposed on spawn
	UpdateFromTask(QuestTalkTask, TalkTaskID);

	Super::NativeConstruct();
}

void UBaseTalkTaskContentPreviewWidget::UpdateFromTask(const FQuestTalkTask& QuestTaskIn, const FQuestTaskID& TaslTaskIDIn)
{
	//Update task variables
	QuestTalkTask = QuestTaskIn;
	TalkTaskID = TaslTaskIDIn;

	//Call update event
	UpdatePreviewWidget();
}

