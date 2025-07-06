//Copyright 2020 Marchetti S. Alfredo I. All Rights Reserved.

#pragma once

#include "Engine/DataTable.h"

//keybinds
#include "Framework/Commands/InputChord.h"

#include "QuestTypesDev.generated.h"


class UBaseQuestTask;
class UBaseTaskTitlePreviewWidget;
class UBaseTaskContentPreviewWidget;
class UBaseStringSelectorWidget;
class UBaseFloatSelectorWidget;
class UBaseTagSelectorWidget;
class UBaseObjectClassSelectorWidget;
class UBaseVectorSelectorWidget;
class UQuestEditorTaskError;
class UCustomPayloadSelectorWidget;


class UBaseTalkTaskContentPreviewWidget;
class UBaseTalkTaskSelectorWidget;
class UQuestEditorTalkTaskError;

UENUM(BlueprintType)
enum class EQuestEditor_EditMode : uint8 {
	NPC,
	Quest,
	Dialog,
};

UENUM(BlueprintType)
enum class ENodeSelectionType : uint8 {
	NoSelection,
	Task,
	TalkTask,
	MultiNode,
	MultiTalkTask,
	MultiTask,
};

UENUM(BlueprintType)
enum class EQuestKeyBindAction : uint8 {
	MoveUp,
	MoveDown,
	MoveLeft,
	MoveRight,
	ZoomIn,
	ZoomOut,
	AlignNodesUp,
	AlignNodesDown,
	AlignNodesLeft,
	AlignNodesRight,
	AlignNodesUpAndEven,
	AlignNodesDownAndEven,
	AlignNodesLeftAndEven,
	AlignNodesRightAndEven,
	AlignNodesToGrid,
	EvenXDistribution,
	EvenYDistribution,
	DeleteNodes,
	IterateStartNodes,
	IterateEndNodes,
	IterateNodesForward,
	IterateNodesBackward,
	FindSelectedNode,
	CenterViewport,
	ResetZoom,
	Undo,
	Redo,
	CopyNodes,
	CutNodes,
	PasteNodes,
	ConnectSelectedNodes,
	ConnectSelectedNodesAsGrid,
	ToggleSelectedConnectionsTypes,
	MutuallyDisableNodes,
	RemoveSelectedNodesConnections,
	RemoveSelectedNodesDisableConnections,
};

/*Struc to define draw arrows.*/
USTRUCT(BlueprintType)
struct FQuestEditorDrawLineData
{
	GENERATED_USTRUCT_BODY()

public:
	FQuestEditorDrawLineData()
	{
		PositionA = {0,0};
		PositionB = { 0,0 };
		Color = { 0,0,0,0 };
		Thickness = 0.f;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DrawData")
		FVector2D PositionA;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DrawData")
		FVector2D PositionB;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DrawData")
		FLinearColor Color;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DrawData")
		float Thickness;
};

/*Struc to define a Quest Editor Keybind.*/
USTRUCT(BlueprintType)
struct FQuestEditorKeyBind
{
	GENERATED_USTRUCT_BODY()

public:
	FQuestEditorKeyBind()
	{
		Action = EQuestKeyBindAction::MoveUp;
		Input = FInputChord();
	}

	UPROPERTY(EditAnywhere, Category = Shortcut)
		EQuestKeyBindAction Action;
	UPROPERTY(EditAnywhere, Category = Shortcut)
		FInputChord Input;
};

/*Struc to define a task editor information. */
USTRUCT(BlueprintType)
struct FQuestTaskEditorInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:

	FQuestTaskEditorInfo()
	{
		TaskName = FString();
		TaskEditorColor = { 0.03 ,0.03 ,0.03 ,1};
		TitlePreviewWidget = nullptr;
		ContentPreviewWidget = nullptr;
		TaskClass = nullptr;
		StringASelector = nullptr;
		StringBSelector = nullptr;
		AmountInvolvedSelector = nullptr;
		TimesToCheckSelector = nullptr;
		TagsRequiredSelector = nullptr;
		TagsIgnoredSelector = nullptr;
		ObjectClassSelector = nullptr;
		VectorSelector = nullptr;
		ErrorObjects = TArray <TSoftClassPtr <UQuestEditorTaskError>>();
		CustomPayloadSelectors = TArray <TSubclassOf <UCustomPayloadSelectorWidget>>();
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Task Editor Info")
		FString TaskName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Task Editor Info")
		FLinearColor TaskEditorColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Task Editor Info")
		TArray <TSoftClassPtr <UQuestEditorTaskError>> ErrorObjects;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Task Editor Info")
		TSubclassOf<UBaseTaskTitlePreviewWidget> TitlePreviewWidget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Task Editor Info")
		TSubclassOf<UBaseTaskContentPreviewWidget> ContentPreviewWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Task Editor Info")
		TSoftClassPtr<UBaseQuestTask> TaskClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Task Editor Info")
		TSubclassOf<UBaseStringSelectorWidget> StringASelector;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Task Editor Info")
		TSubclassOf<UBaseStringSelectorWidget> StringBSelector;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Task Editor Info")
		TSubclassOf<UBaseFloatSelectorWidget> AmountInvolvedSelector;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Task Editor Info")
		TSubclassOf<UBaseFloatSelectorWidget> TimesToCheckSelector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Task Editor Info")
		TSubclassOf<UBaseTagSelectorWidget> TagsRequiredSelector;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Task Editor Info")
		TSubclassOf<UBaseTagSelectorWidget> TagsIgnoredSelector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Task Editor Info")
		TSubclassOf<UBaseObjectClassSelectorWidget> ObjectClassSelector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Task Editor Info")
		TSubclassOf<UBaseVectorSelectorWidget> VectorSelector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Task Editor Info")
		TArray <TSubclassOf <UCustomPayloadSelectorWidget>> CustomPayloadSelectors;
	
};

/*Struc to define a talk task editor information. */
USTRUCT(BlueprintType)
struct FQuestTalkTaskEditorInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:

	FQuestTalkTaskEditorInfo()
	{
		TalkModeName = FString();
		ErrorObjects = TArray <TSoftClassPtr <UQuestEditorTalkTaskError>>();
		ContentPreviewWidget = nullptr;
		TalkTaskSelector = nullptr;
		CustomPayloadSelector = nullptr;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Task Editor Info")
		FString TalkModeName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Task Editor Info")
		TArray <TSoftClassPtr <UQuestEditorTalkTaskError>> ErrorObjects;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Task Editor Info")
		TSubclassOf<UBaseTalkTaskContentPreviewWidget> ContentPreviewWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Task Editor Info")
		TSubclassOf<UBaseTalkTaskSelectorWidget> TalkTaskSelector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Task Editor Info")
		TSubclassOf <UCustomPayloadSelectorWidget> CustomPayloadSelector;

};
