//Copyright 2020 Marchetti S. Alfredo I. All Rights Reserved.


#include "Errors/StandarTaskError.h"
#include "Quest/QuestTypes.h"
#include "QuestEditor/QuestTypesDev.h"
#include "Selectors/BaseObjectClassSelectorWidget.h"
#include "Selectors/BaseStringSelectorWidget.h"

void UStandarTaskError::GetErrorFromTask_Implementation(const FQuestTask& QuestTaskIn, const FQuestTaskEditorInfo& TaskInfoIn, FString& ErrorTextOut, FLinearColor& ErrorColorOut) const
{	
	//Times to check should be always more than 0
	if (QuestTaskIn.Payload.TimesToCheckTask <= 0)
	{
		ErrorTextOut = "TimesToCheck should be more than 0";
		ErrorColorOut = { 0.5,0,0,1 };
		return;
	}
	
	//NAme A
	if (TaskInfoIn.StringASelector.Get())
	{
		//Is variable set?
		if (QuestTaskIn.Payload.StringA == FString())
		{
			ErrorTextOut = "String A not set";
			ErrorColorOut = {0.5,0,0,1};
			return;
		}		
	}
	
	//Name B
	if (TaskInfoIn.StringBSelector.Get())
	{
		//Is variable set?
		if (QuestTaskIn.Payload.StringB == FString())
		{
			ErrorTextOut = "String B not set";
			ErrorColorOut = { 0.5,0,0,1 };
			return;
		}		
	}

	//Object
	if (TaskInfoIn.ObjectClassSelector.Get())
	{
		//Is variable set?
		if (QuestTaskIn.Payload.ClassInvolved == nullptr)
		{
			ErrorTextOut = "Invalid or null class";
			ErrorColorOut = { 0.5,0,0,1 };
			return;
		}		
	}

	//Custom Payload
	for (int i = 0; i < TaskInfoIn.CustomPayloadSelectors.Num(); i++)
	{
		//If invalid
		if (!QuestTaskIn.CustomPayloadData.IsValidIndex(i))
		{
			ErrorTextOut = ("Custom Payload Index " + FString::FromInt(i) + " has null data");
			ErrorColorOut = { 0.5,0,0,1 };
			return;
		}	
		else
		{
			//If not set
			if (QuestTaskIn.CustomPayloadData[i].IsEmpty())
			{
				ErrorTextOut = ("Custom Payload Index " + FString::FromInt(i) + " has null data");
				ErrorColorOut = { 0.5,0,0,1 };
				return;
			}
		}
	}
}
