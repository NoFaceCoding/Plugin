//Copyright 2020 Marchetti S. Alfredo I. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Quest/QuestTypes.h"
#include "BaseQuestTask.generated.h"

class UQuestManager;

/**
 * This is the basic object for any task for quests. 
 * 
 * 
 * The execution logic must always end with a call to the EndTask Function.
 */

UCLASS(Abstract, Blueprintable, Meta = (DisplayName = "Base Quest Task"))
class QUESTEDITOR_API UBaseQuestTask : public UObject
{
	GENERATED_BODY()

public:

	UBaseQuestTask();

	virtual class UWorld* GetWorld() const override;	
	virtual int32 GetFunctionCallspace(UFunction* Function, FFrame* Stack) override;
	virtual bool CallRemoteFunction(UFunction* Function, void* Parameters, FOutParmRec* OutParms, FFrame* Stack) override;	
	
	//--------------------------------------------Variables
	UPROPERTY(BlueprintReadOnly, Category = "Base Quest Task")
		int QuestSlot;

	UPROPERTY(BlueprintReadOnly, Category = "Base Quest Task")
		int TaskSlot;

	UPROPERTY(BlueprintReadOnly, Category = "Base Quest Task")
		bool IsDialogTask;
	
	UPROPERTY(BlueprintReadOnly, Category = "Base Quest Task")
		AActor* QuestManagerOwner;

	UPROPERTY(BlueprintReadOnly, Category = "Base Quest Task")
		UQuestManager* QuestManagerReference;
	
	UPROPERTY(BlueprintReadOnly, Category = "Base Quest Task")
		FQuestTaskPayload InitialPayload;
		
	/**
	*	Determines if the task is only a condition check.
	*	If true task will complete immediatly after meeting activation condition. Any other logic in the task will be ignored.
	*	If they task only performs a condition check via ActivateCondition function, this should be true, to increase performance.
	*/
	UPROPERTY(EditDefaultsOnly, Category = "Base Quest Task")
		bool UseActivateConditionOnly;

	//--------------------------------------------Functions

	/**Called when this object is activated. This event will only trigger once*/	
	UFUNCTION()
		void  StartQuestTask(int QuestSlotIN, int TaskSlotIN, bool IsDialogTaskIN, AActor* QuestManagerOwnerIN, UQuestManager* QuestManagerReferenceIN, const FQuestTaskPayload& PayloadIN);

	/**Called when the quest manager loads from a previous save game. Will only execute for active tasks. This event will trigger everytime a active task is loaded*/
	UFUNCTION()
		void  LoadQuestTask(int QuestSlotIN, int TaskSlotIN, bool IsDialogTaskIN, AActor* QuestManagerOwnerIN, UQuestManager* QuestManagerReferenceIN, const FQuestTaskPayload& PayloadIN);

	/**Callend when the task is saved.*/
	UFUNCTION()
		void  SaveQuestTask();

	/*Callend when the task is completed*/
	UFUNCTION()
		void  CompleteQuestTask();

	/*Callend when the task is disabled while being active. If the task is not active, this will not execute, even when the task is internally disabled. Tasks are disabled by other tasks or when fail*/
	UFUNCTION()
		void  DisableQuestTask();

	/*Callend when the task fails*/
	UFUNCTION()
		void  FailQuestTask();

	/*Callend when the task is destroyed*/
	UFUNCTION()
		void  DestroyQuestTask();

	/*Gets the task score.*/
	UFUNCTION()
		int  GetParticularTaskScore();

	/*Gets the task ActivateConditionOnly status.*/
	UFUNCTION()
		virtual bool  GetTaskConditionOnlyStatus() const;

	/*Gets the task Activate Condition check.*/
	UFUNCTION()
		bool  GetTaskActivationCondition(const FQuestTaskPayload& PayloadIn, const TArray <FString>& CustomPayloadDataIn, UQuestManager* QuestManagerIn);
	
	/**
	*	Update Quest Slot.
	*	Reduces Quest Slot in one.
	*	Called when a quest is removed. Higher Quests need to reduce their quest slot value.
	*/
	UFUNCTION()
		void  ReduceQuestSlot();

protected:

	/**Overrideable function with the task logic on Activation. Always has to end with a call to EndQuestTask. If not this tasks will remain active. This event will only trigger once*/
	UFUNCTION(BlueprintNativeEvent, Category = "Base Quest Task")
		void  OnQuestTaskActivated();

	/**Overrideable function with the task logic when loading the game. Will only execute for loaded Active tasks.*/
	UFUNCTION(BlueprintNativeEvent, Category = "Base Quest Task")
		void  OnQuestTaskLoaded();

	/**Overrideable function with the task logic when saving the game. Will only execute for loaded Active tasks.*/
	UFUNCTION(BlueprintNativeEvent, Category = "Base Quest Task")
		void  OnQuestTaskSaved();

	/**Overrideable function with the task logic when completed.*/
	UFUNCTION(BlueprintNativeEvent, Category = "Base Quest Task")
		void  OnQuestTaskCompleted();

	/**
	*	Overrideable function with the task logic for when is disabled while being active. 
	*	If the task is not active, this will not execute, even when the task is internally disabled. 
	*	Tasks are disabled by other tasks.*/
	UFUNCTION(BlueprintNativeEvent, Category = "Base Quest Task")
		void  OnQuestTaskDisabled();

	/**
	*	Overrideable function with the task logic for when the task fails.
	*	Tasks fails when end in false or when other tasks that disables them fail. 
	*	Also fails if it not mets the Activation Condition on ActivateAttempt.
	*/
	UFUNCTION(BlueprintNativeEvent, Category = "Base Quest Task")
		void  OnQuestTaskFailed();

	/**Overrideable function with the task logic for when the task is destroyed.*/
	UFUNCTION(BlueprintNativeEvent, Category = "Base Quest Task")
		void  OnQuestTaskDestroyed();

public:
	/**
	*	Overrideable function with the task logic for the task condition check on activate. 
	*	This function needs to function without spawning the object, therefore cant use variables from the task. Only inputs of this function.
	*	When task mets all required nodes states, it will attempt to activate. 
	*	If the condition is met, the task will activate.
	*	If the condition is not met the task will fail.
	*	Nodes that are condition only will complete upon activation.
	*/
	UFUNCTION(BlueprintNativeEvent, Category = "Base Quest Task")
		bool  ActivateCondition(const FQuestTaskPayload& PayloadIn, const TArray <FString>& CustomPayloadDataIn, const UQuestManager* QuestManagerIn) const;

protected:
	/**
	*	Overrideable function to determine this task score. Task score is used to calculate quest score.
	*	Quest score can be useful for determining quest rewards or dificiulty. Only completed tasks will be considered.
	*/

	UFUNCTION(BlueprintNativeEvent, Category = "Base Quest Task")
		int  GetTaskScore();

	/**
	* Called when the task ends. Must always be called in the task execution.
	* If returns true, the task track will be updated acording to the amount value. Negative values can decrease the track.
	* If returns false, the task will fail. The amount does not matter in this case.
	* WARNING - This function should be called last in the execution. If you connect something after this node it is possible that it does NOT work correctly or even crashes.
	* @Param Amount This is the amount that the track considers when updates the task current value. It can be usefull to check several tasks at once and commit only one time the final value. It can also be negative.
	* The value on track can't go below 0 or past max. The value will be clamped if that is the case.
	*/
	UFUNCTION(BlueprintCallable, Category = "Base Quest Task")
		void  EndQuestTask(bool Result = true, int Amount = 1);

	/*Retrieves Custom Payload data for this task.*/
	UFUNCTION(BlueprintCallable, Category = "Base Quest Task")
		TArray <FString>  GetCustomPayloadData() const;

	/*Retrieves Custom Payload data for this task with a specific index. If the index is invalid returns empty string.*/
	UFUNCTION(BlueprintCallable, Category = "Base Quest Task")
		FString  GetCustomPayloadDataByIndex(int CustomPayloadSlotIn) const;
		
};
