//Copyright 2020 Marchetti S. Alfredo I. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityWidget.h"
#include "GameplayTagContainer.h"
#include "Engine/DataTable.h"
#include "Quest/QuestTypes.h"
#include "Sound/SoundCue.h"

#include "BaseTalkTaskSelectorWidget.generated.h"

class UBaseQuestEditor;


/**
 *	Base widget that contains logic for talk tasks selectors
 */

 //------------------------------------------DISPATCHERS DEFINITIONS-----------------------------------------------------
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTalkSelectedText, const FText&, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTalkSelectedString, FString, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTalkSelectedSound, TSoftObjectPtr<USoundCue>, NewValue);

UCLASS(Abstract, Blueprintable, Meta = (DisplayName = "TalkTaskSelector"))
class QUESTEDITORDEV_API UBaseTalkTaskSelectorWidget : public UEditorUtilityWidget
{
	GENERATED_BODY()
	
public:

	UBaseTalkTaskSelectorWidget();
	virtual void NativeConstruct() override;

	//------------------------------------------DISPATCHERS------------------------------------------------------
	
	/*Called when the Talk selector commits Player Say value*/
	UPROPERTY(BlueprintAssignable, Category = "Selector")
		FTalkSelectedText SelectedPlayerSay;

	/*Called when the Talk selector commits Player Say value*/
	UPROPERTY(BlueprintAssignable, Category = "Selector")
		FTalkSelectedText SelectedNPCResponse;
	
	/*Called when the Talk selector commits short description*/
	UPROPERTY(BlueprintAssignable, Category = "Selector")
		FTalkSelectedText SelectedShortDescription;

	/*Called when the Talk selector commits custom payload data*/
	UPROPERTY(BlueprintAssignable, Category = "Selector")
		FTalkSelectedString SelectedCustomPayloadData;

	/*Called when the Talk selector commits short description*/
	UPROPERTY(BlueprintAssignable, Category = "Selector")
		FTalkSelectedSound SelectedPlayerSound;

	/*Called when the Talk selector commits short description*/
	UPROPERTY(BlueprintAssignable, Category = "Selector")
		FTalkSelectedSound SelectedNPCSound;

	//------------------------------------------Variables------------------------------------------------------
	/*Quest talk task ID. Use to update widget.*/
	UPROPERTY(meta = (ExposeOnSpawn = "true"), BlueprintReadOnly, Category = "Preview Widget")
		bool IsDialog = false;

	/*Quest talk task current info. Use to update widget.*/
	UPROPERTY(meta = (ExposeOnSpawn = "true"), BlueprintReadOnly, Category = "Preview Widget")
		FQuestTalkTask QuestTalkTask = FQuestTalkTask();

	/*Quest editor reference for calling events and getting datatable information if needed.*/
	UPROPERTY(BlueprintReadOnly, Category = "Preview Widget")
		UBaseQuestEditor* QuestEditorReference = nullptr;


	//------------------------------------------FUNCTIONS------------------------------------------------------

	/**
	*	Selector PlayerSay commit function.
	*	Must be called when the specified variable changes.
	*/
	UFUNCTION(BlueprintCallable, Category = "Selector")
		void SelectorCommitPlayerSay(FText PlayerSayIn);

	/**
	*	Selector NPCResponse commit function.
	*	Must be called when the specified variable changes.
	*/
	UFUNCTION(BlueprintCallable, Category = "Selector")
		void SelectorCommitNPCResponse(FText NPCResponseIn);

	/**
	*	Selector ShortDescription commit function.
	*	Must be called when the specified variable changes.
	*/
	UFUNCTION(BlueprintCallable, Category = "Selector")
		void SelectorCommitShortDescription(FText ShortDescriptionIn);

	/**
	*	Selector CustomPayloadData commit function.
	*	Must be called when the specified variable changes.
	*/
	UFUNCTION(BlueprintCallable, Category = "Selector")
		void SelectorCommitCustomPayloadData(FString NewData);

	/**
	*	Selector Player sound commit function.
	*	Must be called when the specified variable changes.
	*/
	UFUNCTION(BlueprintCallable, Category = "Selector")
		void SelectorCommitPlayerSound(TSoftObjectPtr<USoundCue> NewSoundIn);

	/**
	*	Selector NPC sound commit function.
	*	Must be called when the specified variable changes.
	*/
	UFUNCTION(BlueprintCallable, Category = "Selector")
		void SelectorCommitNPCSound(TSoftObjectPtr<USoundCue> NewSoundIn);

	/**
	*	Main update event for the selector.
	*	All the logic needed for updating this widget should be placed here.
	*	All selectors must override this function.
	*/
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Selector")
		void UpdateFromTask(const FQuestTalkTask& QuestTaskIn, bool IsDialogIn);

};
