//Copyright 2020 Marchetti S. Alfredo I. All Rights Reserved.

#pragma once


#include "Engine/DataTable.h"
#include "GameplayTagContainer.h"
#include "Sound/SoundCue.h"
#include "Misc/DateTime.h"

#include "QuestTypes.generated.h"

class UBaseQuestTask;
class UBaseQuestRewardSlot;

//----------------------------------- QUEST ENUMS

UENUM(BlueprintType)
enum class EQuestTaskState : uint8 {
	Locked,
	PreActive,
	Active,
	Completed,
	Disabled,
	Failed,
};

UENUM(BlueprintType)
enum class EQuestSelectedType : uint8 {
	Active,
	Completed,
};

UENUM(BlueprintType)
enum class ETalkTextType : uint8 {
	PreActivateTalkTask,
	TalkTask,
	AcceptQuest,
};

UENUM(BlueprintType)
enum class ETalkTextMode : uint8 {
	Quest,
	Dialog,
};

UENUM(BlueprintType)
enum class ENodeLinkType : uint8 {
	Required,
	Optional,
};

UENUM(BlueprintType)
enum class EQuestTaskType : uint8 {
	Task,
	TalkTask,
};

UENUM(BlueprintType)
enum class EQuestTaskTypeSelection : uint8 {
	All,
	Task,
	TalkTask,
};

UENUM(BlueprintType)
enum class ENodeConnectorType : uint8 {
	Inferior,
	Superior,
	Disable,
};

UENUM(BlueprintType)
enum class EDialogType : uint8 {
	NPC,
	Player,
};


//-----------------------------------STRUCTS QUEST Objectives Completed

//----------Talk modes-
USTRUCT(BlueprintType)
struct FSingleSentenceModeData
{
	GENERATED_USTRUCT_BODY()
public:

	FSingleSentenceModeData()
	{
		Type = EDialogType::NPC;
		FontSize = 12;
		FontColor = FLinearColor::White;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Single Sentence Mode")
		EDialogType Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Single Sentence Mode")
		float FontSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Single Sentence Mode")
		FLinearColor FontColor;
};

USTRUCT(BlueprintType)
struct FTalkModeDef
{
	GENERATED_USTRUCT_BODY()
public:

	FTalkModeDef()
	{
		RowName = NAME_None;
		Datatable = nullptr;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
		FName RowName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
		TSoftObjectPtr <UDataTable> Datatable;

	FORCEINLINE bool operator==(const FTalkModeDef& Other) const
	{
		return (RowName == Other.RowName && Datatable == Other.Datatable);
	}

	FORCEINLINE bool operator!=(const FTalkModeDef& Other) const
	{
		return (RowName != Other.RowName || Datatable != Other.Datatable);
	}

};

//-------------------------------------Rep

USTRUCT(BlueprintType)
struct FQuestRep
{
	GENERATED_USTRUCT_BODY()
public:

	FQuestRep()
	{
		QuestDTName = NAME_None;
		Datatable = nullptr;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
		FName QuestDTName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
		TSoftObjectPtr <UDataTable> Datatable;

	FORCEINLINE bool operator==(const FQuestRep& Other) const
	{
		return (QuestDTName == Other.QuestDTName && Datatable == Other.Datatable);
	}
	
	FORCEINLINE bool operator!=(const FQuestRep& Other) const
	{
		return (QuestDTName != Other.QuestDTName || Datatable != Other.Datatable);
	}

};

/**Struc to define a address for a Dialog DT.*/
USTRUCT(BlueprintType)
struct FDialogRep
{
	GENERATED_USTRUCT_BODY()
public:

	FDialogRep()
	{
		DialogsDTName = FName();
		Datatable = nullptr;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogs")
		FName DialogsDTName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
		TSoftObjectPtr <UDataTable> Datatable;

	FORCEINLINE bool operator==(const FDialogRep& Other) const
	{
		return (DialogsDTName == Other.DialogsDTName && Datatable == Other.Datatable);
	}

	FORCEINLINE bool operator!=(const FDialogRep& Other) const
	{
		return (DialogsDTName != Other.DialogsDTName || Datatable != Other.Datatable);
	}

};

/*Struct to define a address for a FNPCQuestDef. Should add to your own NPC struct or logic, to have a way to retrieve quests and dialogs information.*/
USTRUCT(BlueprintType)
struct FNPCQuestDefRep
{
	GENERATED_USTRUCT_BODY()
public:

	FNPCQuestDefRep()
	{
		NPCDefDTName = FName();
		Datatable = nullptr;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
		FName NPCDefDTName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
		TSoftObjectPtr <UDataTable> Datatable;

	FORCEINLINE bool operator==(const FNPCQuestDefRep& Other) const
	{
		return (NPCDefDTName == Other.NPCDefDTName && Datatable == Other.Datatable);
	}

	FORCEINLINE bool operator!=(const FNPCQuestDefRep& Other) const
	{
		return (NPCDefDTName != Other.NPCDefDTName || Datatable != Other.Datatable);
	}
};


//----------------------------------- Task IDS
USTRUCT(BlueprintType)
struct FQuestTaskID
{
	GENERATED_BODY()

public:

	FQuestTaskID()
	{
		TaskID = 0;
		TaskType = EQuestTaskType::Task;
	}
	FQuestTaskID(int InTaskID, EQuestTaskType InTaskType)
	{
		TaskID = InTaskID;
		TaskType = InTaskType;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Quest)
		int TaskID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Quest)
		EQuestTaskType TaskType;

	FORCEINLINE bool operator==(const FQuestTaskID& Other) const
	{
		return (TaskID == Other.TaskID && TaskType == Other.TaskType);
	}

	FORCEINLINE bool operator!=(const FQuestTaskID& Other) const
	{
		return (TaskID != Other.TaskID || TaskType != Other.TaskType);
	}
};

//Hashing
FORCEINLINE uint32 GetTypeHash(const FQuestTaskID& b)
{
	return FCrc::MemCrc32(&b, sizeof(FQuestTaskID));
}


/**Struct to define a link between tasks.*/
USTRUCT(BlueprintType)
struct FQuestTaskLink
{
	GENERATED_USTRUCT_BODY()
public:

	FQuestTaskLink()
	{
		Node = FQuestTaskID();
		LinkType = ENodeLinkType::Required;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestTaskLink")
		FQuestTaskID Node;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestTaskLink")
		ENodeLinkType LinkType;

};

//----------------------------------- STRUCTS QUEST TASKS

USTRUCT(BlueprintType)
struct FQuestTalkTask
{
	GENERATED_BODY()

public:

	FQuestTalkTask()
	{
		PlayerDialog = FText();
		NPCResponse = FText();
		TalkNPC = FNPCQuestDefRep();
		TaskShortDesc = FText();
		PlayerSoundCue = nullptr;
		NPCSoundCue = nullptr;
		SuperiorNodes = TArray <FQuestTaskLink>();
		InferiorNodes = TArray <FQuestTaskLink>();
		DisabledNodesOnComplete = TArray <FQuestTaskID>();
		QuestEditorPosition = FVector2D::ZeroVector;
		ConversationID = 0;
		CustomPayloadData = FString();
		UniqueID = -1;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
		FVector2D QuestEditorPosition;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
		TArray <FQuestTaskLink> SuperiorNodes;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
		TArray <FQuestTaskLink> InferiorNodes;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
		TArray <FQuestTaskID> DisabledNodesOnComplete;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
		int ConversationID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
		FNPCQuestDefRep TalkNPC;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
		FText PlayerDialog;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
		FText NPCResponse;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
		TSoftObjectPtr<USoundCue> PlayerSoundCue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
		TSoftObjectPtr<USoundCue> NPCSoundCue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
		FText TaskShortDesc;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
		FString CustomPayloadData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
		int UniqueID;
};

USTRUCT(BlueprintType)
struct FQuestTaskPayload
{
	GENERATED_BODY()

public:

	FQuestTaskPayload()
	{
		TimesToCheckTask = 1;
		AmountInvolved = 0.f;
		StringA = FString();
		StringB = FString();
		ClassInvolved = nullptr;
		VectorInvolved = FVector::ZeroVector;
	}
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
		int TimesToCheckTask;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
		float AmountInvolved;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
		FString StringA;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
		FString StringB;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
		FGameplayTagContainer RequiredTags;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
		FGameplayTagContainer IgnoredTags;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
		TSoftClassPtr<UObject> ClassInvolved;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
		FVector VectorInvolved;
};

USTRUCT(BlueprintType)
struct FQuestTask
{
	GENERATED_BODY()

public:

	FQuestTask()
	{
		TaskShortDesc = FText();
		Payload = FQuestTaskPayload();
		TaskClass = nullptr;
		InverseActivateCondition = false;
		SuperiorNodes = TArray <FQuestTaskLink>();
		InferiorNodes = TArray <FQuestTaskLink>();
		DisabledNodesOnComplete = TArray <FQuestTaskID>();
		QuestEditorPosition = FVector2D::ZeroVector;
		CustomPayloadData = TArray <FString>();
		UniqueID = -1;
	}
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
		FVector2D QuestEditorPosition;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
		TArray <FQuestTaskLink> SuperiorNodes;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
		TArray <FQuestTaskLink> InferiorNodes;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
		TArray <FQuestTaskID> DisabledNodesOnComplete;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
		TSoftClassPtr<UBaseQuestTask> TaskClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
		FQuestTaskPayload Payload;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
		TArray <FString> CustomPayloadData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
		FText TaskShortDesc;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
		bool InverseActivateCondition;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
		int UniqueID;
};

//----------------------------------- STRUCTS QUEST MAIN
USTRUCT(BlueprintType)
struct FQuest : public FTableRowBase
{
	GENERATED_BODY()

public:

	FQuest()
	{
		QuestName = FText();
		QuestDescription = FText();
		Tasks = TArray <FQuestTask>();
		TalkTasks = TArray <FQuestTalkTask>();
		QuestEditorPosition = FVector2D::ZeroVector;
		TalkMode = FTalkModeDef();
		UniqueID = -1;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
		FText QuestName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
		FText QuestDescription;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
		TArray <FQuestTask> Tasks;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
		TArray <FQuestTalkTask> TalkTasks;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
		FGameplayTagContainer QuestTags;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
		FVector2D QuestEditorPosition;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
		FTalkModeDef TalkMode;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
		int UniqueID;

};

USTRUCT(BlueprintType)
struct FTalkTextInfo
{
	GENERATED_BODY()

public:

	FTalkTextInfo()
	{
		QuestSlot = 0;
		TaskSlot = 0;
		State = EQuestTaskState::Locked;
		Type = ETalkTextType::TalkTask;
		Mode = ETalkTextMode::Dialog;
		PlayerSoundCue = nullptr;
		NPCSoundCue = nullptr;
		PlayerDialog = FText();
		NPCResponse = FText();
		CustomPayloadData = FString();
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
		FText PlayerDialog;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
		FText NPCResponse;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
		TSoftObjectPtr<USoundCue> PlayerSoundCue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
		TSoftObjectPtr<USoundCue> NPCSoundCue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
		ETalkTextMode Mode;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
		ETalkTextType Type;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
		EQuestTaskState State;
	UPROPERTY()
		int32 QuestSlot;
	UPROPERTY()
		int32 TaskSlot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
		FString CustomPayloadData;

	//Button type should not be checked. Quest slot shouldt not be checked,
	//if the slot is a new quest, it represents the questslot of NPC, 
	//if it is for a quest that is already in progress, represents the slot in inv
	//Mode should always be checked.
	FORCEINLINE bool operator==(const FTalkTextInfo& Other) const
	{
		return (NPCResponse.ToString() == Other.NPCResponse.ToString() && PlayerDialog.ToString() == Other.PlayerDialog.ToString() && TaskSlot == Other.TaskSlot && Mode == Other.Mode);
	}

	FORCEINLINE bool operator!=(const FTalkTextInfo& Other) const
	{
		return (NPCResponse.ToString() != Other.NPCResponse.ToString() || PlayerDialog.ToString() != Other.PlayerDialog.ToString() || TaskSlot != Other.TaskSlot || Mode != Other.Mode);
	}
	   	 
};

//Hashing
FORCEINLINE uint32 GetTypeHash(const FTalkTextInfo& b)
{
	return FCrc::MemCrc32(&b, sizeof(FTalkTextInfo));
}

//----------------------------------- STRUCTS QUEST TRACK

USTRUCT(BlueprintType)
struct FTalkTaskTrack
{
	GENERATED_BODY()

public:

	FTalkTaskTrack()
	{
		State = EQuestTaskState::Locked;
		ActivationOrder = -1;
		CompletionOrder = -1;
	}

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Quest)
		EQuestTaskState State;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Quest)
		int ActivationOrder;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Quest)
		int CompletionOrder;
};

USTRUCT(BlueprintType)
struct FDialogTrack
{
	GENERATED_BODY()

public:

	FDialogTrack()
	{
		NpcDefRep = FNPCQuestDefRep();
		DialogsStates = TArray <FTalkTaskTrack>();
	}

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Quest)
		FNPCQuestDefRep NpcDefRep;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Quest)
		TArray <FTalkTaskTrack> DialogsStates;
};

/*Tracks the reference of a task object for a particular tasks*/
USTRUCT(BlueprintType)
struct FTaskObjectTrack
{
	GENERATED_BODY()

public:

	FTaskObjectTrack()
	{
		BaseTaskReference = nullptr;
	}

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Quest)
		UBaseQuestTask* BaseTaskReference;
};

/*Tracks all the task objects of a quest*/
USTRUCT(BlueprintType)
struct FQuestObjectTrack
{
	GENERATED_BODY()

public:

	FQuestObjectTrack()
	{
		ObjectTasksTrack = TArray <FTaskObjectTrack>();
	}
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Quest)
		TArray <FTaskObjectTrack> ObjectTasksTrack;
};

USTRUCT(BlueprintType)
struct FTaskTrack
{
	GENERATED_BODY()

public:

	FTaskTrack()
	{
		State = EQuestTaskState::Locked;
		CurrentPayload = FQuestTaskPayload();
		CurrentAmount = 0;
		ActivationOrder = -1;
		CompletionOrder = -1;
	}

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Quest)
		EQuestTaskState State;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Quest)
		int CurrentAmount;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Quest)
		int ActivationOrder;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Quest)
		int CompletionOrder;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Quest)
		FQuestTaskPayload CurrentPayload;
};

/*Quest track for active quest*/
USTRUCT(BlueprintType)
struct FQuestTrack
{
	GENERATED_BODY()

public:

	FQuestTrack() 
	{
		PreActiveTasks = TArray <FQuestTaskID>();
		ActiveTasks = TArray <FQuestTaskID>();
		TasksTracks = TArray <FTaskTrack>();
		TalkTasksTracks = TArray <FTalkTaskTrack>();
		//-1 is invalid, i dont care about type here.
		NodeHandler = FQuestTaskID(-1, EQuestTaskType::Task);
		bIsPendingCompletion = false;
	}
	//Node that is handling the Main activation or completion operation
	UPROPERTY()
		FQuestTaskID NodeHandler;
	//If the quest should be completed at end of handling operation
	UPROPERTY()
		bool bIsPendingCompletion;
	//Preactivation is something rather internal for tracking npc talk multistarts
	UPROPERTY()
		TArray <FQuestTaskID> PreActiveTasks;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Quest)
		TArray <FQuestTaskID> ActiveTasks;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Quest)
		TArray <FTaskTrack> TasksTracks;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Quest)
		TArray <FTalkTaskTrack> TalkTasksTracks;
};

/*Quest track for a completed quest*/
USTRUCT(BlueprintType)
struct FCompletedQuestTrack
{
	GENERATED_BODY()

public:

	FCompletedQuestTrack() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Quest)
		FQuestRep  QuestRep;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Quest)
		FQuestTrack  QuestTrack;
};

/*Base dialog struct for npc*/
USTRUCT(BlueprintType)
struct FDialog : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:

	FDialog()
	{
		DialogEditorName = FString();
		//Makes a basic text, to have something set there
		FQuestTalkTask TalkBase;
		TalkBase.PlayerDialog = FText::FromString("Hello");
		TalkBase.NPCResponse = FText::FromString("What can i do for you?");
		Dialogs = { FQuestTalkTask() };
		EditorPosition = FVector2D::ZeroVector;
		TalkMode = FTalkModeDef();
		UniqueID = -1;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogs")
		FString DialogEditorName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogs")
		TArray <FQuestTalkTask> Dialogs;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogs")
		FVector2D EditorPosition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogs")
		FTalkModeDef TalkMode;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogs")
		int UniqueID;
};

/*Base struct for npcs to define quests and dialogs. Should have at least a datatable to hold this info*/
USTRUCT(BlueprintType)
struct FNPCQuestDef : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:

	FNPCQuestDef()
	{
		NPCName = FString();
		Dialogs = TArray <FQuestRep>();
		Quests = TArray <FQuestRep>();
	}
	/*This is the name used to indentify npc in the editor.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = QuestNPCDef)
		FString NPCName;
	/*Dialogs reference*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = QuestNPCDef)
		TArray <FQuestRep> Dialogs;
	/*Quests references*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = QuestNPCDef)
		TArray <FQuestRep> Quests;
};

/*Base Quest ending struct*/
USTRUCT(BlueprintType)
struct FQuestNode : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:

	FQuestNode()
	{
		Quest = FQuestRep();
		Node = FQuestTaskID();
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
		FQuestRep Quest;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
		FQuestTaskID Node;

	FORCEINLINE bool operator==(const FQuestNode& Other) const
	{
		return (Quest == Other.Quest && Node == Other.Node);
	}

	FORCEINLINE bool operator!=(const FQuestNode& Other) const
	{
		return (Quest != Other.Quest || Node != Other.Node);
	}
};

/*Structure to track Quest fail*/
USTRUCT(BlueprintType)
struct FQuestFailTrack
{
	GENERATED_USTRUCT_BODY()
public:

	FQuestFailTrack()
	{
		Quest = FQuestRep();
		FailDate = FDateTime(0);
	}

	/*The quest that failed*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
		FQuestRep Quest;

	/*Time of the fail quest in UTC time*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
		FDateTime FailDate;

	FORCEINLINE bool operator==(const FQuestNode& Other) const
	{
		return (Quest == Other.Quest);
	}

	FORCEINLINE bool operator!=(const FQuestNode& Other) const
	{
		return (Quest != Other.Quest);
	}
};

/*Structure to provide info for updating the Rewards Tab in the UI.*/
USTRUCT(BlueprintType)
struct FRewardTabSlotInfo
{
	GENERATED_USTRUCT_BODY()
public:

	FRewardTabSlotInfo()
	{
		TaskRewardSlot = nullptr;
		QuestSlot = 0;
		TaskSlot = 0;
		IsDialogTask = false;
	}

	/*Slot class for creating the widget*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
		TSubclassOf<UBaseQuestRewardSlot> TaskRewardSlot;

	/*Quest index in the inventory*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
		int QuestSlot;

	/*Task ID inside the quest*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
		int TaskSlot;

	/*If is a dialog task slot*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
		bool IsDialogTask;

	FORCEINLINE bool operator==(const FRewardTabSlotInfo& Other) const
	{
		return (QuestSlot == Other.QuestSlot && TaskSlot == Other.TaskSlot && IsDialogTask == Other.IsDialogTask);
	}

	FORCEINLINE bool operator!=(const FRewardTabSlotInfo& Other) const
	{
		return (QuestSlot != Other.QuestSlot || TaskSlot != Other.TaskSlot || IsDialogTask != Other.IsDialogTask);
	}
};


/**
*	Base Quest Id track struct.
*	Be careful using this as the slot in quest is not reliable if there are changes to the quest inventory.
*/
USTRUCT(BlueprintType)
struct FQuestNodeID 
{
	GENERATED_USTRUCT_BODY()
public:

	FQuestNodeID()
	{
		QuestSlot = 0;
		Node = FQuestTaskID();
	}
	/*Active Quest Slot*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
		int QuestSlot;

	/*Node ID*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
		FQuestTaskID Node;

	FORCEINLINE bool operator==(const FQuestNodeID& Other) const
	{
		return (QuestSlot == Other.QuestSlot && Node == Other.Node);
	}

	FORCEINLINE bool operator!=(const FQuestNodeID& Other) const
	{
		return (QuestSlot != Other.QuestSlot || Node != Other.Node);
	}
};
