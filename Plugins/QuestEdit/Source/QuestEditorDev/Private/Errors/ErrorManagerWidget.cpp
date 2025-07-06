//Copyright 2020 Marchetti S. Alfredo I. All Rights Reserved.


#include "Errors/ErrorManagerWidget.h"

#include "Errors/QuestEditorTaskError.h"
#include "ErrorsTalk/QuestEditorTalkTaskError.h"


void  UErrorManagerWidget::GetAllErrors(const FQuestTask& QuestTaskIn, const FQuestTaskEditorInfo& TaskInfoIn, TArray <FString>& ErrorOut, TArray <FLinearColor>& ErrorColorOut) const
{
	
	TArray <FString> ErrorsLocal = TArray <FString>();
	TArray <FLinearColor> ColorsLocal= TArray <FLinearColor>();

	FString SingleErrorLocal = FString();
	FLinearColor SingleColorLocal = FLinearColor();

	//Task is not set?
	if (QuestTaskIn.TaskClass.IsNull())
	{
		//Task invalid error. It is handled outside error objects
		ErrorsLocal.Add("Invalid task");
		ColorsLocal.Add({ 0.5,0,0,1 });
	}
	else
	{
		//Error calculation using objects
		for (int i = 0; i < TaskInfoIn.ErrorObjects.Num(); i++)
		{
			//--------------------------------Error task valid?
			if (!TaskInfoIn.ErrorObjects[i].IsValid())
			{
				TaskInfoIn.ErrorObjects[i].LoadSynchronous();
			}

			TSubclassOf <UQuestEditorTaskError> TaskClass = TaskInfoIn.ErrorObjects[i].Get();
			
			//Is valid after load?
			if (TaskClass)
			{
				const UQuestEditorTaskError* TaskCDO = TaskClass.GetDefaultObject();

				if (TaskCDO)
				{
					//Check condition
					TaskCDO->GetErrorFromTask(QuestTaskIn, TaskInfoIn, SingleErrorLocal, SingleColorLocal);
					//if there is error
					if (!SingleErrorLocal.IsEmpty())
					{
						ErrorsLocal.Add(SingleErrorLocal);
						ColorsLocal.Add(SingleColorLocal);
					}
				}				
			}			
		}
	}	

	//Out vars
	ErrorOut = ErrorsLocal;
	ErrorColorOut = ColorsLocal;
	
	return;
}

void  UErrorManagerWidget::GetAllTalkErrors(const FQuestTalkTask& QuestTaskIn, const FQuestTalkTaskEditorInfo& TalkTaskInfoIn, bool IsDialogIn, TArray <FString>& ErrorOut, TArray <FLinearColor>& ErrorColorOut) const
{
	TArray <FString> ErrorsLocal = TArray <FString>();
	TArray <FLinearColor> ColorsLocal = TArray <FLinearColor>();

	FString SingleErrorLocal = FString();
	FLinearColor SingleColorLocal = FLinearColor();

	//Error calculation using objects
	for (int i = 0; i < TalkTaskInfoIn.ErrorObjects.Num(); i++)
	{
		//--------------------------------Error task valid?
		if (!TalkTaskInfoIn.ErrorObjects[i].IsValid())
		{
			TalkTaskInfoIn.ErrorObjects[i].LoadSynchronous();
		}

		TSubclassOf <UQuestEditorTalkTaskError> TaskClass = TalkTaskInfoIn.ErrorObjects[i].Get();

		//Is valid after load?
		if (TaskClass)
		{
			const UQuestEditorTalkTaskError* TaskCDO = TaskClass.GetDefaultObject();

			if (TaskCDO)
			{
				//Check condition
				TaskCDO->GetErrorFromTask(QuestTaskIn, IsDialogIn, SingleErrorLocal, SingleColorLocal);
				//if there is error
				if (SingleErrorLocal != FString())
				{
					ErrorsLocal.Add(SingleErrorLocal);
					ColorsLocal.Add(SingleColorLocal);
				}
			}
		}
	}


	//Out vars
	ErrorOut = ErrorsLocal;
	ErrorColorOut = ColorsLocal;

	return;
}