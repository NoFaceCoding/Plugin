//Copyright 2020 Marchetti S. Alfredo I. All Rights Reserved.


#include "Task/BaseQuestTask.h"
#include "Quest/QuestManager.h"
#include "TimerManager.h"
#include "Engine/NetDriver.h"
#include "Engine/Engine.h"
#include "Engine/BlueprintGeneratedClass.h"

UBaseQuestTask::UBaseQuestTask() : Super()
{
	UseActivateConditionOnly = false;
}

UWorld* UBaseQuestTask::GetWorld() const
{
	if (HasAllFlags(RF_ClassDefaultObject))
	{
		// If we are a CDO, we must return nullptr instead of calling Outer->GetWorld() to fool UObject::ImplementsGetWorld.
		return nullptr;
	}
	return GetOuter()->GetWorld();
}

int32 UBaseQuestTask::GetFunctionCallspace(UFunction* Function, FFrame* Stack)
{
	if (HasAnyFlags(RF_ClassDefaultObject) || !IsSupportedForNetworking())
	{
		return FunctionCallspace::Local;
	}
	check(GetOuter() != nullptr);
	return GetOuter()->GetFunctionCallspace(Function, Stack);
}

bool UBaseQuestTask::CallRemoteFunction(UFunction* Function, void* Parameters, FOutParmRec* OutParms, FFrame* Stack)
{
	check(!HasAnyFlags(RF_ClassDefaultObject));
	check(GetOuter() != nullptr);

	AActor* Owner = CastChecked<AActor>(GetOuter());

	bool bProcessed = false;

	FWorldContext* const Context = GEngine->GetWorldContextFromWorld(GetWorld());
	if (Context != nullptr)
	{
		for (FNamedNetDriver& Driver : Context->ActiveNetDrivers)
		{
			if (Driver.NetDriver != nullptr && Driver.NetDriver->ShouldReplicateFunction(Owner, Function))
			{
				Driver.NetDriver->ProcessRemoteFunction(Owner, Function, Parameters, OutParms, Stack, this);
				bProcessed = true;
			}
		}
	}

	return bProcessed;
}

void UBaseQuestTask::StartQuestTask(int QuestSlotIN, int TaskSlotIN, bool IsDialogTaskIN, AActor* QuestManagerOwnerIN, UQuestManager* QuestManagerReferenceIN, const FQuestTaskPayload& PayloadIN)
{
	//Variables set
	QuestSlot = QuestSlotIN;
	TaskSlot = TaskSlotIN;
	IsDialogTask = IsDialogTaskIN;
	QuestManagerOwner = QuestManagerOwnerIN;
	QuestManagerReference = QuestManagerReferenceIN;
	InitialPayload = PayloadIN;
	//Call execute task to start object
	OnQuestTaskActivated();
}

void UBaseQuestTask::LoadQuestTask(int QuestSlotIN, int TaskSlotIN, bool IsDialogTaskIN, AActor* QuestManagerOwnerIN, UQuestManager* QuestManagerReferenceIN, const FQuestTaskPayload& PayloadIN)
{
	//Variables set
	QuestSlot = QuestSlotIN;
	TaskSlot = TaskSlotIN;
	IsDialogTask = IsDialogTaskIN;
	QuestManagerOwner = QuestManagerOwnerIN;
	QuestManagerReference = QuestManagerReferenceIN;
	InitialPayload = PayloadIN;
	//Call execute task to start object
	OnQuestTaskLoaded();
}

void UBaseQuestTask::SaveQuestTask()
{
	OnQuestTaskSaved();
}

void UBaseQuestTask::CompleteQuestTask()
{
	OnQuestTaskCompleted();
}

void UBaseQuestTask::DisableQuestTask()
{
	OnQuestTaskDisabled();
}

void UBaseQuestTask::FailQuestTask()
{
	OnQuestTaskFailed();
}

void UBaseQuestTask::DestroyQuestTask()
{
	OnQuestTaskDestroyed();
}

int UBaseQuestTask::GetParticularTaskScore()
{
	return GetTaskScore();
}

bool UBaseQuestTask::GetTaskConditionOnlyStatus() const
{
	return UseActivateConditionOnly;
}

bool UBaseQuestTask::GetTaskActivationCondition(const FQuestTaskPayload& PayloadIn, const TArray <FString>& CustomPayloadDataIn, UQuestManager* QuestManagerIn)
{
	return ActivateCondition(PayloadIn, CustomPayloadDataIn, QuestManagerIn);
}

void UBaseQuestTask::ReduceQuestSlot()
{
	//Reduce slot in 1
	if (QuestSlot > 0) QuestSlot = QuestSlot-1;
	return;
}

void UBaseQuestTask::OnQuestTaskActivated_Implementation()
{
}

void UBaseQuestTask::OnQuestTaskLoaded_Implementation()
{
}

void UBaseQuestTask::OnQuestTaskSaved_Implementation()
{
}

void UBaseQuestTask::OnQuestTaskCompleted_Implementation()
{
}

void UBaseQuestTask::OnQuestTaskDisabled_Implementation()
{
}

void UBaseQuestTask::OnQuestTaskFailed_Implementation()
{
}

void UBaseQuestTask::OnQuestTaskDestroyed_Implementation()
{
}

int UBaseQuestTask::GetTaskScore_Implementation()
{
	return 0;
}

bool UBaseQuestTask::ActivateCondition_Implementation(const FQuestTaskPayload& PayloadIn, const TArray <FString>& CustomPayloadDataIn, const UQuestManager* QuestManagerIn) const
{
	return true;
}

void UBaseQuestTask::EndQuestTask(bool Result, int Amount)
{
	//Calls quests manager owner function to be handled
	if (QuestManagerReference) QuestManagerReference->HandleTaskEnd(QuestSlot, TaskSlot, IsDialogTask, Result, Amount);
}

TArray<FString> UBaseQuestTask::GetCustomPayloadData() const
{
	if (!QuestManagerReference) return TArray<FString>();

	TArray<FString> DataLocal = QuestManagerReference->GetQuestTaskCustomPayloadData(EQuestSelectedType::Active, QuestSlot, TaskSlot, IsDialogTask);
	FString DataOut = FString();

	return DataLocal;
}

FString UBaseQuestTask::GetCustomPayloadDataByIndex(int CustomPayloadSlotIn) const
{
	if (!QuestManagerReference) return FString();

	FString DataLocal = QuestManagerReference->GetQuestTaskCustomPayloadDataByIndex(EQuestSelectedType::Active, QuestSlot, TaskSlot, CustomPayloadSlotIn, IsDialogTask);
	   
	return DataLocal;
}
