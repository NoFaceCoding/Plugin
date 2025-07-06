//Copyright 2020 Marchetti S. Alfredo I. All Rights Reserved.


#include "Preview/BasePreviewWidget.h"


#include "QuestEditor/BaseQuestEditor.h"
#include "QuestEditor/QuestEditorDevFunctionLibrary.h"


UBasePreviewWidget::UBasePreviewWidget() : Super()
{
}

void UBasePreviewWidget::NativeConstruct()
{
	//Get quest editor reference
	QuestEditorReference = UQuestEditorDevFunctionLibrary::FindQuestEditorReference(this);
	//Update on start with task exposed on spawn
	UpdateFromTask(QuestTask);

	Super::NativeConstruct();
}

void UBasePreviewWidget::UpdateFromTask(const FQuestTask& QuestTaskIn)
{
	//Update task
	QuestTask = QuestTaskIn;
	//Call update event
	UpdatePreviewWidget();
}

FQuestTaskEditorInfo UBasePreviewWidget::GetTaskInfo()
{
	int SlotTask = 0;
	int SlotQuest = 0;
	FQuestTaskEditorInfo TaskInfo = FQuestTaskEditorInfo();
	UQuestEditorDevFunctionLibrary::GetTaskQuestEditorInfo(QuestTask.TaskClass, QuestEditorReference->TaskInfoDatatables, SlotTask, SlotQuest, TaskInfo);
	return TaskInfo;
}
