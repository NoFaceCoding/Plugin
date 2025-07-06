//Copyright 2020 Marchetti S. Alfredo I. All Rights Reserved.

#include "QuestEditor/QuestEditorFunctionLibrary.h"
#include "Quest/QuestsFunctionLibrary.h"
#include "Engine/DataTable.h"
#include "DataTableUtils.h"
#include "Runtime/CoreUObject/Public/UObject/Object.h"

FQuest UQuestEditorFunctionLibrary::GetDTQuestRowFromPath(FName RowName, TSoftObjectPtr <UDataTable> DTPath)
{
	FQuest QuestLocal;
	UDataTable* DT = UQuestsFunctionLibrary::GetDTFromPath(DTPath);

	//invalid path?
	if (!DT->IsValidLowLevel()) return QuestLocal;

	TArray<FName> Names = DT->GetRowNames();
	//Exist?
	if (Names.Contains(RowName))
	{	 
		FQuest* QuestPointer = DT->FindRow<FQuest>(RowName, FString(), true);
		if (QuestPointer != nullptr)
		{
			QuestLocal = *QuestPointer;
		}
	}
	return QuestLocal;
}

FDialog UQuestEditorFunctionLibrary::ConvertQuestToDialog(const FQuest& QuestIn)
{
	//Converts quest into a dialog struct
	FDialog DialogLocal = FDialog();
	DialogLocal.DialogEditorName = QuestIn.QuestName.ToString();
	DialogLocal.Dialogs = QuestIn.TalkTasks;
	DialogLocal.EditorPosition = QuestIn.QuestEditorPosition;
	DialogLocal.TalkMode = QuestIn.TalkMode;
	DialogLocal.UniqueID = QuestIn.UniqueID;
	return DialogLocal;
}

FQuest UQuestEditorFunctionLibrary::ConvertDialogToQuest(const FDialog& DialogIn)
{
	//Converts quest into a dialog struct
	FQuest QuestLocal = FQuest();
	QuestLocal.QuestName = FText::FromString(DialogIn.DialogEditorName);
	QuestLocal.TalkTasks = DialogIn.Dialogs;
	QuestLocal.QuestEditorPosition = DialogIn.EditorPosition;
	QuestLocal.TalkMode = DialogIn.TalkMode;

	return QuestLocal;
}

FQuest UQuestEditorFunctionLibrary::GetDTDialogRowFromPath(FName RowName, TSoftObjectPtr <UDataTable> DTPath)
{
	FQuest QuestLocal = FQuest();
	UDataTable* DT = UQuestsFunctionLibrary::GetDTFromPath(DTPath);

	//invalid path?
	if (!DT->IsValidLowLevel()) return QuestLocal;

	TArray<FName> Names = DT->GetRowNames();

	//Exist?
	if (Names.Contains(RowName))
	{
		FDialog* DialogPointer = DT->FindRow<FDialog>(RowName, FString(), true);
		FDialog Dialog = FDialog();
		if (DialogPointer != nullptr)
		{
			Dialog = *DialogPointer;
			//Sets quest talktasks with dialogs info.
			QuestLocal = ConvertDialogToQuest(Dialog);
		}
	}
	return QuestLocal;
}

int UQuestEditorFunctionLibrary::GetAmountOfQuestEnds(const FQuest& QuestIn) 
{
	int amount = 0;

	for (int i = 0; i < QuestIn.Tasks.Num(); i++)
	{
		if (QuestIn.Tasks[i].InferiorNodes.Num() == 0) amount++;
	}

	for (int i = 0; i < QuestIn.TalkTasks.Num(); i++)
	{
		if (QuestIn.TalkTasks[i].InferiorNodes.Num() == 0) amount++;
	}

	return amount;
}

TArray<FQuestTaskID> UQuestEditorFunctionLibrary::GetQuestEndTasks(const FQuest& QuestIn)
{
	TArray<FQuestTaskID> EndsOut;
	EndsOut.Empty();
	FQuestTaskID TaskIDLocal;

	//Task ends
	TaskIDLocal.TaskType = EQuestTaskType::Task;
	for (int i = 0; i < QuestIn.Tasks.Num(); i++)
	{
		if (QuestIn.Tasks[i].InferiorNodes.Num() == 0) 
		{
			TaskIDLocal.TaskID = i;
			EndsOut.Add(TaskIDLocal);
		}
	}
	// Talk ends
	TaskIDLocal.TaskType = EQuestTaskType::TalkTask;
	for (int i = 0; i < QuestIn.TalkTasks.Num(); i++)
	{
		if (QuestIn.TalkTasks[i].InferiorNodes.Num() == 0)
		{
			TaskIDLocal.TaskID = i;
			EndsOut.Add(TaskIDLocal);
		}
	}
	return EndsOut;
}

TArray<FQuestTaskID> UQuestEditorFunctionLibrary::GetQuestTasksIDs(const FQuest& QuestIn)
{
	TArray<FQuestTaskID> NodesOut = TArray<FQuestTaskID>();

	FQuestTaskID TaskIDLocal;

	//Task 
	TaskIDLocal.TaskType = EQuestTaskType::Task;
	for (int i = 0; i < QuestIn.Tasks.Num(); i++)
	{
		TaskIDLocal.TaskID = i;
		NodesOut.Add(TaskIDLocal);		
	}

	// Talk
	TaskIDLocal.TaskType = EQuestTaskType::TalkTask;
	for (int i = 0; i < QuestIn.TalkTasks.Num(); i++)
	{
		TaskIDLocal.TaskID = i;
		NodesOut.Add(TaskIDLocal);
	}
	
	return NodesOut;
}

int UQuestEditorFunctionLibrary::GetAmountOfQuestStarts(const FQuest& QuestIn)
{
	int amount = 0;

	for (int i = 0; i < QuestIn.Tasks.Num(); i++)
	{
		if (QuestIn.Tasks[i].SuperiorNodes.Num() == 0) amount++;
	}

	for (int i = 0; i < QuestIn.TalkTasks.Num(); i++)
	{
		if (QuestIn.TalkTasks[i].SuperiorNodes.Num() == 0) amount++;
	}

	return amount;
}
