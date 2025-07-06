//Copyright 2020 Marchetti S. Alfredo I. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Quest/QuestTypes.h"
#include "QuestManager.generated.h"


//------------------------------------------DISPATCHERS DEFINITIONS-----------------------------------------------------

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FNoParamsDelegateQuest);

//Quest
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FQuestCompleted, const FQuest&, Quest, int, CompletedSlot);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FQuestSignature, int, QuestSlot, const FQuest&, Quest);


//Selected quest
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSelectedQuest, int, NewSelectedQuest);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSelectedQuestCompChanged, int, NewSelectedQuestCompleted);

//Tasks
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FQuestTaskSignature, int, QuestSlot, int, TaskSlot, const FQuestTask&, Task);

//TalkTask
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FQuestTalkTaskSignature, int, QuestSlot, int, TalkTaskSlot, const FQuestTalkTask&, TalkTask);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FQTaskUpdated, int, QuestSlot, const FQuestTaskID&, TaskID, bool, IsDialog);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FTalkConversation, int, QuestSlot, int, ConversationID, bool, IsDialog, const FQuestTalkTask&, ConversationTalkTask);

//Nodes (Task and TalkTasks)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FQTaskUpdatedState, int, QuestSlot, const FQuestTaskID&, TaskID, bool, IsDialog, EQuestTaskState, NewState);

//Dialogs
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FDialogReaded, int, DialogOut, AActor*, NPCOut, const FTalkTextInfo&, TalkTextInfo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FActiveTalkTextChanged, const FTalkTextInfo&, TalkTextInfo);

//Manager Message
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FQM_Message, FText, Message);

//Dialog Track
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FNewDialogTrackAdded, const FDialogTrack&, NewDialogTrack);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDialogTrackUpdated, const TArray <FDialogTrack>&, DialogTrack);

//NPC
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FNPCReached, AActor*, NPC);

//Select reward tasks
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FQRewardSelectSignature, const FQuestNode&, QuestNode, bool, IsDialog);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FQRewardActivatedSignature, int, QuestSlotIn, int, TaskSlotIn, bool, IsDialog);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FQRewardCompletedSignature, const FQuestTask&, QuestTask, bool, IsDialog);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class QUESTEDITOR_API UQuestManager : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UQuestManager();

	//For replication
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

	//------------------------------------------DISPATCHERS------------------------------------------------------

	//------------------------------------------Notify

	/*Call on repnotify event when the quest inventory is updated*/
	UPROPERTY(BlueprintAssignable, Category = "Quest Notify")
		FNoParamsDelegateQuest QuestRepInventoryUpdated;

	/*Called on repnotify event when the completed quests track is updated*/
	UPROPERTY(BlueprintAssignable, Category = "Quest Notify")
		FNoParamsDelegateQuest CompletedQuestsTrackRepUpdated;

	/*Call on repnotify event when the quest track inventory is updated*/
	UPROPERTY(BlueprintAssignable, Category = "Quest Notify")
		FNoParamsDelegateQuest QuestTrackRepUpdated;

	/*Call on repnotify event when the quest fail track inventory is updated*/
	UPROPERTY(BlueprintAssignable, Category = "Quest Notify")
		FNoParamsDelegateQuest QuestFailTrackRepUpdated;
	
	//------------------------------------------Quests Events

	/*Called when a quest is accepted*/
	UPROPERTY(BlueprintAssignable, Category = "Quest")
		FQuestSignature QuestAccepted;

	/*Called everytime a new quest is added to the quest inventory.Only triggers on client, useful for cosmetic effects*/
	UPROPERTY(BlueprintAssignable, Category = "Quest")
		FQuestSignature Client_QuestAccepted;

	/*Called when a Dialog is accepted*/
	UPROPERTY(BlueprintAssignable, Category = "Dialog")
		FQuestSignature DialogAccepted;

	/*Called when a quest is completed*/
	UPROPERTY(BlueprintAssignable, Category = "Quest")
		FQuestCompleted QuestCompleted;

	/*Called when a Dialog is completed*/
	UPROPERTY(BlueprintAssignable, Category = "Dialog")
		FQuestCompleted DialogCompleted;

	/*Called when a quest is abandoned*/
	UPROPERTY(BlueprintAssignable, Category = "Quest")
		FQuestSignature QuestAbandoned;

	/*Called when a Dialog is abandoned*/
	UPROPERTY(BlueprintAssignable, Category = "Dialog")
		FQuestSignature DialogAbandoned;

	/*Called when a quest fails*/
	UPROPERTY(BlueprintAssignable, Category = "Quest")
		FQuestSignature QuestFailed;

	/*Called when a Dialog fails*/
	UPROPERTY(BlueprintAssignable, Category = "Dialog")
		FQuestSignature DialogFailed;
	   
	/*Called when a quest is removed from inventory, can be because completion, fail or abandon.*/
	UPROPERTY(BlueprintAssignable, Category = "Quest")
		FQuestSignature QuestRemoved;

	/*Called when a quest is removed from inventory, can be because completion, fail or abandon. Only triggers on client, useful for cosmetic effects*/
	UPROPERTY(BlueprintAssignable, Category = "Quest")
		FQuestSignature Client_QuestRemoved;

	/*Called when a Dialog is removed from inventory, can be because completion, fail or abandon.*/
	UPROPERTY(BlueprintAssignable, Category = "Dialog")
		FQuestSignature DialogRemoved;
		   	
	/**
	*	Called when a quest is removed from the complete quest inventory.
	*	This only happens if its forced by the user.
	*	Normally, quests that are completed are never removed from the completed quest inventory, as it holds useful information.
	*/
	UPROPERTY(BlueprintAssignable, Category = "Quest")
		FQuestCompleted QuestCompletedRemoved;

	/**
	*	Called when a Dialog is removed from the complete quest inventory.
	*	This only happens if its forced by the user.
	*	Normally, Dialog that are completed are never removed from the completed quest inventory, as it holds useful information.
	*/
	UPROPERTY(BlueprintAssignable, Category = "Dialog")
		FQuestCompleted DialogCompletedRemoved;

	//------------------------------------------Talk
	/*Single event for updating all talk texts at same time. It triggers when the current talk texts for a npc changes.*/
	UPROPERTY(BlueprintAssignable, Category = "Quest Talk Task")
		FNoParamsDelegateQuest OnNPCTalkTextsChanged;
	
	//------------------------------------------Dialogs

	/*Called when a new dialog is selected.*/
	UPROPERTY(BlueprintAssignable, Category = "Dialogs")
		FActiveTalkTextChanged ActiveTalkTextChanged;

	/*Called when a NPC is reached.*/
	UPROPERTY(BlueprintAssignable, Category = "Dialogs")
		FNPCReached OnNPCReached;	

	/*Called when a NPC is reached. Only client*/
	UPROPERTY(BlueprintAssignable, Category = "Dialogs")
		FNPCReached Client_OnNPCReached;

	//------------------------------------------Display
	/*Called when the selected quest changes*/
	UPROPERTY(BlueprintAssignable, Category = "Quest Selected")
		FSelectedQuest SelectedQuestChanged;

	/*Called when the selected completed quest changes*/
	UPROPERTY(BlueprintAssignable, Category = "Quest Selected")
		FSelectedQuestCompChanged SelectedQuestCompChanged;

	//------------------------------------------Quest Task

	/*Called when a task is activated.*/
	UPROPERTY(BlueprintAssignable, Category = "Quest Task")
		FQuestTaskSignature QuestTaskActivated;

	/*Called when a task is activated.*/
	UPROPERTY(BlueprintAssignable, Category = "Dialog Task")
		FQuestTaskSignature DialogTaskActivated;

	/*Called when a task is completed.*/
	UPROPERTY(BlueprintAssignable, Category = "Quest Task")
		FQuestTaskSignature QuestTaskCompleted;

	/*Called when a task is completed.*/
	UPROPERTY(BlueprintAssignable, Category = "Dialog Task")
		FQuestTaskSignature DialogTaskCompleted;

	/*Called when a task is fails.*/
	UPROPERTY(BlueprintAssignable, Category = "Quest Task")
		FQuestTaskSignature QuestTaskFailed;

	/*Called when a task is fails.*/
	UPROPERTY(BlueprintAssignable, Category = "Dialog Task")
		FQuestTaskSignature DialogTaskFailed;

	/*Called when a talk task is activated.*/
	UPROPERTY(BlueprintAssignable, Category = "Quest Task")
		FQuestTalkTaskSignature QuestTalkTaskActivated;

	/*Called when a talk task is activated.*/
	UPROPERTY(BlueprintAssignable, Category = "Dialog Task")
		FQuestTalkTaskSignature DialogTalkTaskActivated;

	/*Called when a task is completed.*/
	UPROPERTY(BlueprintAssignable, Category = "Quest Task")
		FQuestTalkTaskSignature QuestTalkTaskCompleted;

	/*Called when a task is completed.*/
	UPROPERTY(BlueprintAssignable, Category = "Dialog Task")
		FQuestTalkTaskSignature DialogTalkTaskCompleted;

	/**
	*	Callend when a talk task fails.
	*	Talk tasks cannot fail on their own, but they can fail if a task that disables them fails.
	*/
	UPROPERTY(BlueprintAssignable, Category = "Quest Task")
	FQuestTalkTaskSignature QuestTalkTaskFailed;

	/**
	*	Callend when a talk task fails.
	*	Talk tasks cannot fail on their own, but they can fail if a task that disables them fails.
	*/
	UPROPERTY(BlueprintAssignable, Category = "Dialog Task")
	FQuestTalkTaskSignature DialogTalkTaskFailed;
	
	//---------------------------------Conversation
	/**
	*	Called when a conversation starts.
	*	Conversations are a group of connected Talk tasks of same NPC.
	*	The ID of the conversation is the node with lower ID.
	*	The talk task information of the conversation ID is also provided.
	*/
	UPROPERTY(BlueprintAssignable, Category = "Quest Task")
		FTalkConversation QuestConversationActivated;

	/**
	*	Called when a conversation ends.
	*	Conversations are a group of connected Talk tasks of same NPC.
	*	The ID of the conversation is the node with lower ID.
	*	The talk task information of the conversation ID is also provided.
	*/
	UPROPERTY(BlueprintAssignable, Category = "Quest Task")
		FTalkConversation QuestConversationCompleted;

	//-------------------------------- Tasks general

	/*Any task from dialog or quest updated amount in track. Does not include completion*/
	UPROPERTY(BlueprintAssignable, Category = "Quest Task")
		FQTaskUpdated Client_QTaskUpdatedTrackAmount;

	/*Any task from dialog or quest is completed*/
	UPROPERTY(BlueprintAssignable, Category = "Quest Task")
		FQTaskUpdated QTaskCompleted;

	/*Any task  from dialog or quest is Activated*/
	UPROPERTY(BlueprintAssignable, Category = "Quest Task")
		FQTaskUpdated QTaskActivated;
	
	/*Client only event that triggers when a task is completed o deactivated from dialog or quest or for any other reason leaves the active task pool*/
	UPROPERTY(BlueprintAssignable, Category = "Quest Task")
		FQTaskUpdated Client_QTaskIsNoLongerActive;

	/*Client only event that triggers when a task activates from dialog or quest*/
	UPROPERTY(BlueprintAssignable, Category = "Quest Task")
		FQTaskUpdated Client_QTaskActivated;

	/*Client only event that triggers when a task completes from dialog or quest*/
	UPROPERTY(BlueprintAssignable, Category = "Quest Task")
		FQTaskUpdated Client_QTaskCompleted;

	/*Any task from dialog or quest is Activated*/
	UPROPERTY(BlueprintAssignable, Category = "Quest Task")
		FQTaskUpdated QTaskDeactivated;

	//--------------------------------------NODE------------------
	/**	
	*	Called when any node from dialog or quest changes state. 
	*/
	UPROPERTY(BlueprintAssignable, Category = "Quest Task")
		FQTaskUpdatedState OnNodeChangedState;
	
	//------------------------------------------General UI
	/*Called when there is a message (gameplay related) that needs to be delivered to the player.*/
	UPROPERTY(BlueprintAssignable, Category = "Quest Task")
		FQM_Message QM_GameplayMessage;

	//-----------------------------------------Manager specific
	/*Called when quest manager loads all saved data to independant save game object. Only fires if HandleSaveAutomatically is true*/
	UPROPERTY(BlueprintAssignable, Category = "Quest Framework")
		FNoParamsDelegateQuest QM_QuestDataLoaded;

	/*Called when quest manager saves all data to independant save game object. Only fires if HandleSaveAutomatically is true*/
	UPROPERTY(BlueprintAssignable, Category = "Quest Framework")
		FNoParamsDelegateQuest QM_QuestDataSaved;

	//------------------------------------------Select rewards
	/*Called when the player selects a reward.*/
	UPROPERTY(BlueprintAssignable, Category = "Quest Reward Select")
		FQRewardSelectSignature OnPlayerSelectedReward;

	/**
	*	Called when the player has a new reward available.
	*	Used to open the reward selection UI.
	*/
	UPROPERTY(BlueprintAssignable, Category = "Quest Reward Select")
		FQRewardActivatedSignature OnPlayerRewardActivated;

	/**
	*	Called when the player completes a reward task and the reward is handled succesfully.
	*	Use to notify the player that a new reward has been obtained.
	*/
	UPROPERTY(BlueprintAssignable, Category = "Quest Reward Select")
		FQRewardCompletedSignature OnRewardTaskCompleted;

	//------------------------------------------PUBLIC EVENTS------------------------------------------------------
protected:
	/*Server start quest manager*/
	UFUNCTION(Server, WithValidation, Reliable, BlueprintCallable, Category = "Quest Server Events")
		void ROS_StartQuestManager(const TArray<FQuestRep>& QuestInventoryIN, const TArray<FCompletedQuestTrack>& QuestCompletedIN, const TArray<FQuestFailTrack>& QuestFailTrackIN, const TArray<FQuestTrack>& QuestTrackIN,
			const TArray<FQuestRep>& DialogInventoryIN, const TArray<FCompletedQuestTrack>& DialogCompletedIN, const TArray<FQuestFailTrack>& DialogFailTrackIN, const TArray<FQuestTrack>& DialogTrackIN);

	/*Client start quest manager*/
	UFUNCTION(Client, Reliable, BlueprintCallable, Category = "Quest Framework")
		void Client_StartQuestManager(const TArray<FQuestRep>& QuestInventoryIN, const TArray<FCompletedQuestTrack>& QuestCompletedIN, const TArray<FQuestTrack>& QuestTrackIN, const TArray<FQuestFailTrack>& QuestFailTrackIN, 
			const TArray<FQuestRep>& DialogInventoryIN, const TArray<FCompletedQuestTrack>& DialogCompletedIN, const TArray<FQuestFailTrack>& DialogFailTrackIN, const TArray<FQuestTrack>& DialogTrackIN);

	/*Server loads game from save*/
	UFUNCTION(Server, WithValidation, Reliable, BlueprintCallable, Category = "Quest Server Events")
		void ROS_LoadFromSaveGame(const FString& PlayerSaveNameIN);

	/*Server reaches npc*/
	UFUNCTION(Client, WithValidation, Reliable)
		void ROS_NPCReached(AActor* NPC);

	/*Server accepts quest from talk*/
	UFUNCTION(Server, WithValidation, Reliable)
		void ROS_AcceptQuestFromTalk(AActor* NPC, int QuestSlotIn, int TalkTaskIDIn);

	/*Server checks all talk tasks for a talk text*/
	UFUNCTION(Server, WithValidation, Reliable)
		void ROS_CompleteQuestTalkTask(int QuestSlotIn, int TaskIDIn, AActor* NPC);
	
	/*Server side only event to abandon quest.*/
	UFUNCTION(Server, WithValidation, Reliable)
		void ROS_AbandonQuest(int QuestSelected);

	/*Server side only event to accept quest directly.*/
	UFUNCTION(Server, WithValidation, Reliable)
		void ROS_AcceptQuestDirectly(const FQuestRep& QuestRepIn, bool IsDialogIn);
public:
	/*Client event side only event to accept quest directly from start quest actors.*/
	UFUNCTION(Client, Reliable)
		void Client_AcceptQuestFromStartActor(const FQuestRep& QuestRepIn);

protected:
	/**
	*	Server side only event to remove completed quest directly.
	*	Removes quest slot from the complete quest inventory.
	*	Quests that are removed from completed track will be able to be shown again in the npc, as it was never completed before.
	*	Might be useful for quests that are designed to be completed multiple times.
	*/
	UFUNCTION(Server, WithValidation, Reliable)
		void  ROS_RemoveCompletedQuestSlot(int QuestSlotIn);

	/**
	*	Server side only event to remove completed quest directly.
	*	Removes a QuestRep from the complete quest inventory.
	*	Quests that are removed from completed track will be able to be shown again in the npc, as it was never completed before.
	*	Might be useful for quests that are designed to be completed multiple times.
	*/
	UFUNCTION(Server, WithValidation, Reliable)
		void  ROS_RemoveCompletedQuestRep(const FQuestRep& QuestRepIn);

	/*Select reward on server*/
	UFUNCTION(Server, WithValidation, Reliable)
		void  ROS_PlayerSelectReward(const FQuestNode& QuestNodeIn, bool IsDialogIn);

	//------------------------------------------VARIABLES------------------------------------------------------

	//---------------------------------------------------------------------------------------------------------------------------
	//------------------------------------------Quet Manager Configuration
	//---------------------------------------------------------------------------------------------------------------------------

	/**
	*	Datatables paths to folders that contains datatables for the quest system that should be loaded on manager start.
	*	This helps reducing fps drops during gameplay for loading datatables in memory.
	*	If you don't add folders here, the datatables will be loaded when needed by the system.
	*/
	UPROPERTY(EditDefaultsOnly, Category = "General")
		TArray <FString> PreLoadedDatatablePaths;

	/**
	*	When getting talktext information for an NPC, if completed ones should be included or not.
	*	Can improve performance in big quests.
	*/
	UPROPERTY(EditDefaultsOnly, Category = "General")
		bool ShowCompletedTalkTexts = true;

	/*Cancel loops if they reach a huge amount. Just to avoid any issues. This should never be reached.*/
	UPROPERTY()
		int32 MaxWhileLoopCount = 500;

	/*Pre loaded datatables*/
	UPROPERTY()
		TArray <UDataTable*> PreLoadedDatatables;

	/**
	*	Maximum amount of quests that a player can have active at a time.
	*	If the player reaches this amount of quest, he will not be able to accept more quests untill completes, fails or abandon one.
	*/
	UPROPERTY(EditDefaultsOnly, Category = "Quest")
		int MaxActiveQuests = 20;

	/**
	*	Maximum amount of dialogs that the player can have active at a time.
	*	If the player reaches this amount, he will "forget" previous dialogs of NPCs. This dialogs will be reseted when talking with that NPC again.
	*/
	UPROPERTY(EditDefaultsOnly, Category = "Dialog")
		int MaxActiveDialogs = 20;

	/**
	*	Try to accept dialogs when the NPC has a valid dialog and the player has no active ones of that NPC.
	*	For example, if you abandon a dialog, and that is the only dialog for that NPC, you will immediatly accept it back from start.
	*/
	UPROPERTY(EditDefaultsOnly, Category = "Dialog")
		bool AutoAcceptDialogsIfPossible = false;

	/*Player name used to save games. Using a existing name will overwritte any existing savegame.*/
	UPROPERTY(EditAnywhere, Category = "Quest Save Game")
		FString PlayerSaveName = "Player";

	/**
	*	If enabled, the quest manager will save automatically using the PlayerSaveName. 
	*	This will create a Quest Savegame per player.
	*	If disabled, the save must be done manually with QM_SaveToSaveGame or EndManager functions. This allows more control to when perform this action and how to store the data.
	*/
	UPROPERTY(EditAnywhere, Category = "Quest Save Game")
		bool HandleSaveAutomatically = true;

	/**
	*	If enabled, the quest manager will load automatically using the PlayerSaveName on Manager BeginPlay. 
	*	The PlayerSaveName must be set before BeginPlay if it is changed at runtime.
	*	If disabled, the and load must be done manually with QM_LoadFromSaveGame or StartManager functions. This allows more control to when perform this action and how to load the data.
	*/
	UPROPERTY(EditAnywhere, Category = "Quest Save Game")
		bool HandleLoadAutomatically = true;

	//-------------------------------------------Quest fail
	/**
	*	If enabled, all quests that fail will be tracked.
	*	If disabled, failed quests will be removed from quest inventory and will be available for the player to redo immediately.
	*/
	UPROPERTY(EditAnywhere, Category = "Quest")
		bool TrackFailQuests = true;

	/**
	*	Maximun time that a quest can remain in fail state. 
	*	After this time, the quest will be available for the player again.
	*	Measured in hours.
	*	If set to zero, it means that there is no time limit and the quest will remain in permanently fail state.
	*/
	UPROPERTY(EditAnywhere, Category = "Quest")
		float MaxQuestFailTime = 0.f;

	/**
	*	If enabled, all dialog that fail will be tracked.
	*	If disabled, failed dialog will be removed from quest inventory and will be available for the player to redo immediately.
	*/
	UPROPERTY(EditAnywhere, Category = "Dialog")
		bool TrackFailDialogs = true;

	/**
	*	Maximun time that a dialog can remain in fail state.
	*	After this time, the dialog will be available for the player again.
	*	Measured in hours.
	*	If set to zero, it means that there is no time limit and the quest will remain in permanently fail state.
	*/
	UPROPERTY(EditAnywhere, Category = "Dialog")
		float MaxDialogFailTime = 0.f;

	//------------------------------------------Quet Manager Talk tasks
	/*Single Talk task score. Used for calculating quest score.*/
	UPROPERTY(EditAnywhere, Category = "General")
		int TalkTaskScore = 1;

	
	//---------------------------------------------------------------------------------------------------------------------------
	//----------------------------------------------Manager Variables
	//---------------------------------------------------------------------------------------------------------------------------
	//------------------------------------------Quests
	/*Contains all active quests*/
	UPROPERTY(ReplicatedUsing = OnRep_QuestInventory)
		TArray<FQuestRep> QuestInventory = TArray<FQuestRep>();
	UFUNCTION()
		void OnRep_QuestInventory();

	/*Contains all completed quests tracks*/
	UPROPERTY(ReplicatedUsing = OnRep_CompletedQuestsTrack)
		TArray<FCompletedQuestTrack> CompletedQuestsTrack = TArray<FCompletedQuestTrack>();
	UFUNCTION()
		void OnRep_CompletedQuestsTrack();

	/*Contains all active quests tracks*/
	UPROPERTY(ReplicatedUsing = OnRep_QuestsTrack)
		TArray<FQuestTrack> QuestsTrack = TArray<FQuestTrack>();
	UFUNCTION()
		void OnRep_QuestsTrack();

	/*Contains all fail quests tracks*/
	UPROPERTY(ReplicatedUsing = OnRep_QuestsFailTrack)
		TArray<FQuestFailTrack> QuestsFailTrack = TArray<FQuestFailTrack>();
	UFUNCTION()
		void OnRep_QuestsFailTrack();

	/*Track local of reference of tasks objects. Is not replicated but the servers has a parallel one.*/
	UPROPERTY()
		TArray<FQuestObjectTrack> LocalObjectsQuestsTrack = TArray<FQuestObjectTrack>();

	//------------------------------------------Dialogs
	/*Contains all active dialogs*/
	UPROPERTY()
		TArray<FQuestRep> DialogsInventory = TArray<FQuestRep>();

	/*Contains all completed dialog tracks*/
	UPROPERTY()
		TArray<FCompletedQuestTrack> CompletedDialogsTrack = TArray<FCompletedQuestTrack>();

	/**
	*	All npc dialogs that are beign tracked.
	*	This variable is not replicated because is not really important for gameplay
	*/
	UPROPERTY()
		TArray<FQuestTrack> DialogsTrack = TArray<FQuestTrack>();

	/*Contains all fail dialogs tracks*/
	UPROPERTY()
		TArray<FQuestFailTrack> DialogsFailTrack = TArray<FQuestFailTrack>();

	/*Track local of reference of tasks objects for dialogs. Is only client side and servers should not use it since dialogs are client side.*/
	UPROPERTY()
		TArray<FQuestObjectTrack> LocalObjectsDialogsTrack = TArray<FQuestObjectTrack>();

	//------------------------------------------Display
	/*Selected completed quest slot*/
	UPROPERTY()
		int SelectedCompQuest = 0;

	/*Selected active quest slot*/
	UPROPERTY()
		int SelectedQuestSlot = 0;

	/*Selected active talk text*/
	UPROPERTY()
		FTalkTextInfo ActiveTalkText = FTalkTextInfo();

	/**
	*	Selected NPC. This is the npc that is currently talking to the player.
	*	This is set on NPC reached.
	*	WARNING - This variable will not be set to null as there is no way to know when you stop talking to a NPC from the quest manager. It will overriden on next NPC reached.
	*/
	UPROPERTY()
		AActor* CurrentNPC;

	//------------------------------------------FUNCTIONS------------------------------------------------------
	//-----------------------------------------On destroy override
	/*Override of begin destroy, to clear objects spawned*/
	void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	/*Override of begin play for this manager. Handles loading from own save*/
	void BeginPlay() override;	
	
	//-----------------------------------------Framework

	/*Handles pre loading of datatables to avoid fps drops during gameplay*/
	UFUNCTION()
		void  HandleQuestDTLoading();

	/*Start quest manager variables*/
	UFUNCTION()
		void  StartQuestManagerVariables(const TArray<FQuestRep>& QuestInventoryIN, const TArray<FCompletedQuestTrack>& QuestCompletedIN, const TArray<FQuestTrack>& QuestTrackIN, const TArray<FQuestFailTrack>& QuestFailTrackIN, 
			const TArray<FQuestRep>& DialogInventoryIN, const TArray<FCompletedQuestTrack>& DialogCompletedIN, const TArray<FQuestFailTrack>& DialogFailTrackIN, const TArray<FQuestTrack>& DialogTrackIN);

public:

	/**
	*	Handles end event of manager and gives the variables that should be saved.
	*	Executes all "OnQuestTaskDestroyed" events from active tasks. 
	*	If called in server, dialogs track will return empty, cuz this is a only client variable. 
	*	Dialogs should be saved on client with diferent function
	*	Useful for handling quest save games manually.
	*/
	UFUNCTION(BlueprintCallable, Category = "Quest Framework")
		void  EndQuestManager(TArray<FQuestRep>& QuestInventoryOUT, TArray<FCompletedQuestTrack>& QuestCompletedOUT, TArray<FQuestTrack>& QuestTrackOUT, TArray<FQuestFailTrack>& QuestFailTrackOUT,
			TArray<FQuestRep>& DialogInventoryOUT, TArray<FCompletedQuestTrack>& DialogCompletedOUT, TArray<FQuestTrack>& DialogTrackOUT, TArray<FQuestFailTrack>& DialogFailTrackOUT);

	/*Variable that holds the player name for saving and loading quest manager data.*/
	UFUNCTION(BlueprintCallable, Category = "Quest Framework")
		void QM_SetSaveGameName(FString PlayerSaveNameIN);

	/**
	*	Saves all quest data.
	*	Handles objects save events.
	*	Can be used to save mid game.
	*/
	UFUNCTION(BlueprintCallable, Category = "Quest Framework")
		void SaveQuestData(FString PlayerSaveNameIN);

private:
	/*
	*	Load all variables from a existing savegameobject and initiate the quest manager. 
	*	If HandleSaveAutomatically = true, this is called on QM Begin play with the Default player name
	*	Useful when wanting to load game from diferent player namee dynamically.
	*/
	UFUNCTION(BlueprintCallable, Category = "Quest Framework")
		void QM_LoadFromSaveGame(FString PlayerSaveNameIN);

	/**
	*	Saves all variables, rep and no rep in a savegame object.
	*	Used if the save is done by listen server.
	*	Does not handle objects.
	*	If HandleSaveAutomatically = true, this is called on QM End play with the Default player name
	*/
	UFUNCTION(BlueprintCallable, Category = "Quest Framework")
		void QM_SaveToSaveGame(FString PlayerSaveNameIN);

	/*Save all non replicated variables to save game. If save already exists, it wont replace the savegame, only update the variables*/
	UFUNCTION(BlueprintCallable, Category = "Quest Framework")
		void QM_SaveNoRepToSaveGame(FString PlayerSaveNameIN);

	/*Save all replicated variables to save game. If save already exists, it wont replace the savegame, only update the variables*/
	UFUNCTION(BlueprintCallable, Category = "Quest Framework")
		void QM_SaveRepToSaveGame(FString PlayerSaveNameIN);

public:
	//------------------------------------------Quests

	/**
	*	Logic for reward selecting tasks.
	*	This calls a dispatcher to start handling rewards in UI.
	* 	Called from reward tasks.
	*	@param RewardInteractActor NPC or Location actor reached for the reward.
	*	@param IsNPCIn wether the rewardactor is NPC or location.
	*/
	UFUNCTION()
		void  RewardTaskActivated(int QuestSlotIn, int TaskSlotIn, bool IsDialogIn);

private:
	/**
	*	Call to select rewards from reward slots.
	*	This calls a dispatcher to start handling rewards in tasks
	*/
	UFUNCTION()
		void  PlayerSelectReward(const FQuestNode& QuestNodeIn, bool IsDialogIn);

public:

	/**
	*	Called when a reward was selected succesfully in the task. 
	*	Used for dispatchers and notify.
	*/
	UFUNCTION()
		void  PlayerHandledRewardSuccesfully(const FQuestNode& QuestNodeIn, bool IsDialogIn);

	/**
	*	Call to select rewards from reward slots.
	*	Call from client only. Designed to call from UI.
	*	Do not call inside tasks.
	*/
	UFUNCTION(BlueprintCallable, Category = "Quest rewards")
		void  Pred_PlayerSelectReward(int QuestSlotIn, int TaskSlotIn, bool IsDialogIn);

	/**
	*	Get data for current rewards to Update Rewards Tab from retrigger data.
	*	@Param IsDialogIn If should search in dialogs or quest inventories for rewards.
	*/
	UFUNCTION()
		TArray <FRewardTabSlotInfo>  GetRetriggerRewardsUIData(bool IsDialogIn, bool IsNPCRewardIn, const FString& LocationNameIn, const FNPCQuestDefRep& NPCDefIn) const;

protected:
	UFUNCTION()
		TArray <FRewardTabSlotInfo>  GetRetriggerRewardsUIDataWithArrays(bool IsDialogIn, bool IsNPCRewardIn, const FString& LocationNameIn, const FNPCQuestDefRep& NPCDefIn, const TArray<FQuestTrack>& QuestsTrackIN) const;

public:
	/**
	*	Get data for current rewards to Update Rewards Tab.
	*	It uses a reward data struct just because it has the same important variables.
	*/
	UFUNCTION()
		TArray <FRewardTabSlotInfo>  GetCurrentRewardsUIData(const TArray <FRewardTabSlotInfo>& ActivatedRewardsIn, bool& IsNPCRewardOut, FString& LocationNameOut, FNPCQuestDefRep& NPCDefOut) const;

private:
	/**
	*	Get data for current rewards node. Analizes a node and determines if is a reward for location or NPC and the variables needed to process the operation.
	*	@return Returns if the node is a reward node or not.
	*/
	UFUNCTION()
		bool  GetRewardNodeData(const FQuestNodeID& NodeIn, bool IsDialogIn, const TArray<FQuestTrack>& QuestsTrackIN, bool& IsNPCRewardOUT, bool& IsLocationRewardOut, FNPCQuestDefRep& NPCDefOut, FString& LocationNameOUT) const;

public:
	/*Determines if a quest can be accepted.*/
	UFUNCTION()
		bool  CanAcceptQuest(const FQuestRep& QuestRepIN, bool IsDirectModeIn, bool IsDialogIn, AActor* NPCIN, bool ShouldPrintErrors) const;

private:

	/*Determines if a quest can be accepted.*/
	UFUNCTION()
		bool  CanAcceptQuestWithArrays(const FQuestRep& QuestRepIN, bool IsDirectModeIn, bool IsDialogIn, AActor* NPCIN, bool ShouldPrintErrors, const TArray<FQuestRep>& QuestsInventoryIN, const TArray<FCompletedQuestTrack>& CompletedQuestsTrackIN, const TArray<FQuestFailTrack>& QuestsFailTrackIN) const;

	/**	
	*	Accept a quest via talk system.
	*	When accepting quest via talking, all the initial tasks will activate and the selected talk task corresponding to the talk text provided.
	*	@Param QuestSlotIn QuestSlotIndex in the NPC inventory.
	*/
	UFUNCTION()
		bool  AcceptQuestFromTalk(AActor* NPC, int QuestSlotIn, int TalkTaskIDIn, bool IsDialogIN);
	
	UFUNCTION()
		bool  AcceptQuestFromTalkWithArrays(AActor* NPC, int QuestSlotIn, int TalkTaskIDIn, bool IsDialogIN, TArray<FQuestRep>& QuestsInventoryIN, TArray<FQuestTrack>& QuestsTrackIN, TArray<FQuestObjectTrack>& ObjectsTrackIN);

	/*Ends a quest succesfully.*/
	UFUNCTION()
		bool  CompleteQuest(int QuestSlotIn, const FQuest& QuestIn, bool IsDialogIN, TArray<FQuestRep>& QuestsInventoryIN, TArray<FQuestTrack>& QuestsTrackIN, TArray<FQuestObjectTrack>& ObjectsTrackIN, TArray<FCompletedQuestTrack>& CompletedQuestsTrackIN);
public:

	/**
	*	Abandons a quest rep.
	*	Can be called inside a Base Quest Task object.
	*/
	UFUNCTION(BlueprintCallable, Category = "Quest")
		bool  AbandonQuestRep(bool IsDialogIN, const FQuestRep& QuestRepIn);

	/**
	*	Abandons a quest by slot.
	*	Can be called inside a Base Quest Task object.
	*/
	UFUNCTION(BlueprintCallable, Category = "Quest")
		bool  AbandonQuest(bool IsDialogIN, int Slot);

	/**
	*	Handles client and server logic for abandoning selected quest with prediction.
	*	Call from client only. Designed to call from UI.
	*	Do not call inside tasks. Use AbandonQuest in that case if necessary.
	*/
	UFUNCTION(BlueprintCallable, Category = "Quest")
		void Pred_AbandonSelectedQuest();

	/**
	*	Handles client and server logic for abandoning quest from slot with prediction.
	*	Call from client only. Designed to call from UI.
	*	Do not call inside tasks. Use AbandonQuest in that case if necessary.
	*/
	UFUNCTION(BlueprintCallable, Category = "Quest")
		void Pred_AbandonQuestBySlot(int QuestSlotIn);

	/**
	*	Accepts a quest directly, without requiring any talk to a NPC.
	*	When accepting quests directly, will only activate initial tasks.
	*	Can be used inside a Base Quest Task object.
	*/
	UFUNCTION(BlueprintCallable, Category = "Quest")
		bool  AcceptQuestDirectly(const FQuestRep& QuestRepIn, bool IsDialogIn);
	
	/**
	*	Handles client and server logic for accepting a quest or dialog directly with prediction.
	*	Call from client only. Designed to call from UI.
	*	Do not call inside tasks. Use AcceptQuestDirectly in that case if necessary.
	*/
	UFUNCTION(BlueprintCallable, Category = "Quest")
		void Pred_AcceptQuestDirectly(const FQuestRep& QuestRepIn, bool IsDialogIn);


protected:
	/*Removes a quest slot from the quest inventory.*/
	UFUNCTION()
		bool  RemoveQuestSlot(int Slot, bool IsDialogIn, TArray<FQuestRep>& QuestsInventoryIN, TArray<FQuestTrack>& QuestsTrackIN, TArray<FQuestObjectTrack>& ObjectsTrackIN);

	/**
	*	Attemps to complete a talk tasks.
	*	Uses Quests arrays directly.
	*/
	UFUNCTION()
		bool  CompleteQuestTalkTask(int QuestSlotIn, int TaskIDIn, bool IsDialogIN, AActor* NPC);

	/**
	*	Attemps to complete a talk tasks.
	*	Uses configurable arrays.
	*/
	UFUNCTION()
		bool  CompleteQuestTalkTaskWithArrays(int QuestSlotIn, int TaskIDIn, bool IsDialogIN, AActor* NPC, TArray<FQuestRep>& QuestsInventoryIN, TArray<FQuestTrack>& QuestsTrackIN, TArray<FQuestObjectTrack>& ObjectsTrackIN, TArray<FCompletedQuestTrack>& CompletedQuestsTrackIN, TArray<FQuestFailTrack>& QuestsFailTrackIN);

	/**
	*	Adds quest to inventory and handles first tasks. Returns the QuestSlot*
	*	@Param QuestRepIN Quest to add
	*	@Param Is the add direct or via talk?
	*/
	UFUNCTION()
		int  AddQuestToInv(const FQuestRep& QuestRepIN, bool DirectModeIn, bool IsDialogIN);

	UFUNCTION()
		int  AddQuestToInvWithArrays(const FQuestRep& QuestRepIN, bool DirectModeIn, bool IsDialogIN, TArray<FQuestRep>& QuestsInventoryIN, TArray<FQuestTrack>& QuestsTrackIN, TArray<FQuestObjectTrack>& ObjectsTrackIN, TArray<FCompletedQuestTrack>& CompletedQuestsTrackIN, TArray<FQuestFailTrack>& QuestsFailTrackIN);

	/**	
	*	Adds quest to inventory via talk system and handles first tasks. 
	*/
	UFUNCTION()
		bool  AddQuestToInvFromTalk(const FQuestRep& QuestRepIN, bool IsDialogIN, AActor* NPCIN, int QuestSlotIn, int TalkTaskIDIn);

	/*Handles quest fail, this happens when a task is failed and there is no more active tasks.*/
	UFUNCTION()
		bool  HandleQuestFail(int QuestSlotIn, const FQuest& QuestIn, bool IsDialogIN, TArray<FQuestRep>& QuestsInventoryIN, TArray<FQuestTrack>& QuestsTrackIN, TArray<FQuestObjectTrack>& ObjectsTrackIN, TArray<FQuestFailTrack>& QuestsFailTrackIN);

	/*Determines if a task should fail.*/
	UFUNCTION()
		bool  QuestShouldFail(int QuestIN, TArray<FQuestTrack>& QuestsTrackIN);

	/**
	*	Forces the  quest fail. Called in tasks only.
	*	All active nodes are forced to fail before the quest fail.
	*/
	UFUNCTION()
		bool  ForceQuestFail(int QuestSlotIn, bool IsDialogIn, TArray<FQuestRep>& QuestsInventoryIN, TArray<FQuestTrack>& QuestsTrackIN, TArray<FQuestObjectTrack>& ObjectsTrackIN, TArray<FQuestFailTrack>& QuestsFailTrackIN);

public:

	/**
	*	Forces all active nodes of quest fail, with the exception of the node selected (the one that calls this function)
	*	Used for the "Task_ForceQuestFail". This allows to execute normally after this and fail the quest "naturally".
	*/
	UFUNCTION()
		bool  ForceActiveNodesFailWithException(int QuestSlotIn, bool IsDialogTaskIN, const FQuestTaskID& ExceptionNodeIn);

protected:
	UFUNCTION()
		bool  ForceActiveNodesFailWithExceptionWithArrays(int QuestSlotIn, bool IsDialogIn, const FQuestTaskID& ExceptionNodeIn, TArray<FQuestRep>& QuestsInventoryIN, TArray<FQuestTrack>& QuestsTrackIN, TArray<FQuestObjectTrack>& ObjectsTrackIN);

public:
	/**
	*	Removes quest slot from the complete quest or dialog inventory.
	*	Quests that are removed from completed track will be able to be shown again in the npc, as it was never completed before.
	*	Might be useful for quests that are designed to be completed multiple times.
	*	Can be used inside a Base Quest Task object.
	*/
	UFUNCTION(BlueprintCallable, Category = "Quest")
		bool  RemoveCompletedQuestSlot(int QuestSlotIn, bool IsDialogIn);

	/**
	*	Removes a QuestRep from the complete quest or dialog inventory.
	*	Quests that are removed from completed track will be able to be shown again in the npc, as it was never completed before.
	*	Might be useful for quests that are designed to be completed multiple times.
	*	Can be used inside a Base Quest Task object.
	*/
	UFUNCTION(BlueprintCallable, Category = "Quest")
		bool  RemoveCompletedQuestRep(const FQuestRep& QuestRepIn, bool IsDialogIn);

	/**
	*	Removes quest slot from the complete quest inventory.
	*	Quests that are removed from completed track will be able to be shown again in the npc, as it was never completed before.
	*	Might be useful for quests that are designed to be completed multiple times.
	*	Call from client only. Designed to call from client.
	*/
	UFUNCTION(BlueprintCallable, Category = "Quest")
		void  Pred_RemoveCompletedQuestSlot(int QuestSlotIn);

	/**
	*	Removes a QuestRep from the complete quest inventory.
	*	Quests that are removed from completed track will be able to be shown again in the npc, as it was never completed before.
	*	Might be useful for quests that are designed to be completed multiple times.
	*	Call from client only. Designed to call from client.
	*/
	UFUNCTION(BlueprintCallable, Category = "Quest")
		void  Pred_RemoveCompletedQuestRep(const FQuestRep& QuestRepIn);

	//------------------------------------------Quests Info

	/*Determines if quest slot and task slot are valid in an active quest or dialog.*/
	UFUNCTION(BlueprintCallable, Category = "Quest Info")
		bool  IsTaskIDValid(int QuestSlotIN, const FQuestTaskID& TaskIDIN, bool IsDialogIn) const;

	/*Determines if a quest is active.*/
	UFUNCTION(BlueprintCallable, Category = "Quest Info")
		bool  IsQuestInProgress(bool IsDialogIn, const FQuestRep& QuestRepIn) const;

protected:
	/*Determines if a quest is active.*/
	UFUNCTION()
		bool  IsQuestInProgressWithArrays(const FQuestRep& QuestRepIn, const TArray<FQuestRep>& QuestsInventoryIN) const;

public:
	/*Determines if a quest is completed.*/
	UFUNCTION(BlueprintCallable, Category = "Quest Info")
		bool  IsQuestCompleted(bool IsDialogIn, const FQuestRep& QuestRepIn) const;

protected:
	/*Determines if a quest is completed.*/
	UFUNCTION(BlueprintCallable, Category = "Quest Info")
		bool  IsQuestCompletedWithArrays(const FQuestRep& QuestRepIN, const TArray<FCompletedQuestTrack>& CompletedQuestsTrackIN) const;

public:
	/*Determines if a quest is completed with a specific ending.*/
	UFUNCTION(BlueprintCallable, Category = "Quest Info")
		bool  IsQuestCompletedWithEnding(bool IsDialogIn, const FQuestNode& QuestEndIn) const;

protected:
	/*Determines if a quest is completed with a specific ending.*/
	UFUNCTION(BlueprintCallable, Category = "Quest Info")
		bool  IsQuestCompletedWithEndingWithArrays(const FQuestNode& QuestEndIn, const TArray<FCompletedQuestTrack>& CompletedQuestsTrackIN) const;

public:
	/**
	*	Gets a Quest state.
	*	Uses task state enum. Quest can be active, completed or locked for quests that the player has not started yet.
	*/
	UFUNCTION(BlueprintCallable, Category = "Quest Info")
		EQuestTaskState  GetQuestState(bool IsDialogIn, const FQuestRep& QuestIn) const;

public:
	/*Gets all task Slots of same type that matches state. If only want active tasks, should use GetActiveTasksSlotsFromTrack instead for performance gain.*/
	UFUNCTION(BlueprintCallable, Category = "Quest Info")
		TArray <int> GetTasksSlotsByState(int QuestSlotIN, EQuestTaskType TaskTypeIN, bool IsDialogIn, EQuestTaskState StateIN = EQuestTaskState::Active) const;
	
	/*Obtains all quest tasks of given state.*/
	UFUNCTION(BlueprintCallable, Category = "Quest Info")
		TArray <FQuestTask> GetTasksByState(int QuestSlotIN, bool IsDialogIn, EQuestTaskState StateIN = EQuestTaskState::Active) const;
	
	/*Obtains all quest talk tasks of given state.*/
	UFUNCTION(BlueprintCallable, Category = "Quest Info")
		TArray <FQuestTalkTask> GetTalkTasksByState(int QuestSlotIN, bool IsDialogIn, EQuestTaskState StateIN = EQuestTaskState::Active) const;

	/*Determine if a task is preactivated.*/
	UFUNCTION(BlueprintCallable, Category = "Quest Info")
		bool IsTaskPreActive(int QuestSlotIn, FQuestTaskID TaskIDIn, bool IsDialogIn) const;

	/**	
	*	Finds the state of a node for a active quest.
	*	If the quest is not found, the state is considered locked.
	*/
	UFUNCTION(BlueprintCallable, Category = "Quest Info")
		EQuestTaskState  GetNodeStateInActiveTrack(const FQuestRep& QuestRepIn, const FQuestTaskID& NodeIn, bool IsDialogIn) const;

	/**
	*	Finds the state of a node for a completed quest.
	*	If the quest is not found, the state is considered locked.
	*/
	UFUNCTION(BlueprintCallable, Category = "Quest Info")
		EQuestTaskState  GetNodeStateInCompletedTrack(bool IsDialogIn, const FQuestRep& QuestRepIn, const FQuestTaskID& NodeIn) const;

	/**
	*	Gets the completed track for a quest. 
	*	Returns null if not found.
	*/
	UFUNCTION(BlueprintCallable, Category = "Quest Info")
		FQuestTrack  GetCompletedQuestTrackForQuest(bool IsDialogIn, const FQuestRep& QuestRepIn) const;

	/**
	*	Gets the track for a quest.
	*	Returns null if not found.
	*/
	UFUNCTION(BlueprintCallable, Category = "Quest Info")
		FQuestTrack  GetActiveQuestTrackForQuest(bool IsDialogIn, const FQuestRep& QuestRepIn) const;

	/**
	*	Gets a quest track for a specific quest.
	*	Can be a active or a completed quest.
	*/
	UFUNCTION(BlueprintCallable, Category = "Quest Info")
		FQuestTrack  GetQuestTrackForQuest(EQuestSelectedType SelectType, bool IsDialogIn, const FQuestRep& QuestRepIn) const;

	/**
	*	Finds the state of a node for a provided quest track.
	*	If operation fails, the state is considered locked.
	*/
	UFUNCTION()
		EQuestTaskState  GetNodeStateFromTrackStruct(const FQuestTaskID& NodeIn, const FQuestTrack& QuestTrackIn) const;

	/*Gets a completed quest in a slot.*/
	UFUNCTION(BlueprintCallable, Category = "Quest Info")
		FQuest  GetCompletedQuestBySlot(int Slot, bool IsDialogIn) const;

	/*Retrieves the talk task score set in the manager.*/
	UFUNCTION(BlueprintCallable, Category = "Quest Score")
		int  GetQuestTalkTaskScore() const;

	/*Retrieves total score of a quest task using GetTaskScore() function from the base quest task involved.*/
	UFUNCTION(BlueprintCallable, Category = "Quest Score")
		int  GetQuestTaskScore(EQuestSelectedType SelectType, int QuestSlotIn, int TaskSlotIn, bool IsDialogIn) const;

	/*Retrieves total score of a quest task using GetTaskScore() function from the base quest task involved.*/
	UFUNCTION()
		int  GetQuestTaskScoreFromObjectTrack(EQuestSelectedType SelectType, int QuestSlotIn, const FQuest& QuestIn, int TaskSlotIn, const TArray<FQuestObjectTrack>& ObjectsTrackIN) const;
	
	/**
	*	Calculates current score of a quest, by adding all completed tasks scores.
	*	Can be useful for generating rewards such as gold or experience or for determining a quest dificulty.
	*	If the quest is not fully completed, it will yield the current score.
	*/
	UFUNCTION(BlueprintCallable, Category = "Quest Score")
		int  GetQuestScore(EQuestSelectedType SelectType, int QuestSlotIn, bool IsDialogIn) const;
	 
	/**
	*	Calculates total score of a quest, considering every task regardless their state.
	*/
	UFUNCTION(BlueprintCallable, Category = "Quest Score")
		int  GetQuestTotalScore(EQuestSelectedType SelectType, int QuestSlotIn, bool IsDialogIn) const;

public:
	/*Retrieves Custom Payload data from a quest.*/
	UFUNCTION(BlueprintCallable, Category = "Quest Score")
		TArray <FString>  GetQuestTaskCustomPayloadData(EQuestSelectedType SelectType, int QuestSlotIn, int TaskSlotIn, bool IsDialogIn) const;

	/*Retrieves Custom Payload data from a quest for a specific index. If the index is invalid returns empty string.*/
	UFUNCTION(BlueprintCallable, Category = "Quest Score")
		FString  GetQuestTaskCustomPayloadDataByIndex(EQuestSelectedType SelectType, int QuestSlotIn, int TaskSlotIn, int CustomPayloadSlotIn, bool IsDialogIn) const;

	//---------------------------------------Units involved in quests
	/*Returns true if this NPC has a valid quest to offer to the player*/
	UFUNCTION(BlueprintCallable, Category = "Quest Info")
		bool  NPCGivesQuest(AActor* NPCIn) const;

	/*Finds all the quests that a NPC can give to the player.*/
	UFUNCTION(BlueprintCallable, Category = "Quest Talk Task")
		TArray <FQuestRep>  GetNPCAvailableQuests(AActor* NPCIn) const;

	/*Finds all the quests slots that are in progress and that the NPC has at least one talktext available for it.*/
	UFUNCTION(BlueprintCallable, Category = "Quest Talk Task")
		TArray <int>  GetNPCActiveQuestSlots(AActor* NPCIn) const;

	/*Finds all talk texts information for a NPC and for a particular quest that is in progress.*/
	UFUNCTION(BlueprintCallable, Category = "Quest Talk Task")
		TArray <FTalkTextInfo>  GetTalkTextsInfoForQuestSlot(AActor* NPCIN, int QuestSlotIn) const;

	/**
	*	Returns all Quests Slots of active  talk tasks that involve the NPC
	*	@Param GetFirstOneOnly if true will return only the first slot found.
	*/
	UFUNCTION(BlueprintCallable, Category = "Quest Info")
		TArray <int>  GetQuestsSlotsInvolvingNPCTalk(AActor* NPCIn, bool GetFirstOneOnly = false) const;

	/*Returns all Tasks IDSs of active talk tasks that involve the NPC*/
	UFUNCTION(BlueprintCallable, Category = "Quest Info")
		TArray <FQuestTaskID>  GetTaskIDsInvolvingNPCTalk(int QuestSlotIn, AActor* NPCIn) const;

protected:	
	//-----------------------------------------Nodes
	
	/*Checks if a node can activate. Does not check the activation condition of a task.*/
	UFUNCTION()
		bool NodeCanActivate (int QuestSlotIn, const FQuest& QuestIn, const FQuestTaskID& NodeIn, const TArray<FQuestTrack>& QuestsTrackIN) const;

	/*Checks if a node can Re-activate. Does not check the activation condition of a task.*/
	UFUNCTION()
		bool NodeCanReActivate(int QuestSlotIn, const FQuest& QuestIn, const FQuestTaskID& NodeIn, const TArray<FQuestTrack>& QuestsTrackIN) const;

	/**	
	*	Checks if a node has the requeriments of previous nodes in order to activate. Does not check the activation condition of a task.
	*	For a node to activate, it requires to have all required superior linked nodes completed and any number of optional nodes.
	*	If there is no Required Link, at least one optional is required.
	*	If the node has no superior links, it can activate.
	*/
	UFUNCTION()
		bool NodeHasLinksToActivate(int QuestSlotIn, const FQuest& QuestIn, const FQuestTaskID& NodeIn, const TArray<FQuestTrack>& QuestsTrackIN) const;

	/*
	*	Checks if a node can activate based on required previous nodes.
	*	Does not use track, instead uses a completed Nodes Array.
	*	Only works for talk tasks. Used to find pseudo talk start nodes.
	*/
	UFUNCTION()
		bool TalkNodeCanPseudoActivate(const FQuest& QuestIn, const FQuestTaskID& NodeIn, const TArray <FQuestTaskID>& PseudoCompletedNodes) const;

	/*Checks if a task meets activate condition. Calling this function does not require object spawn*/
	UFUNCTION()
		bool TaskActivateConditionCheck(const FQuest& QuestIn, int TaskSlot) const;

public:
	/*Gets state of node in track.*/
	UFUNCTION(BlueprintCallable, Category = "Quest Nodes")
		EQuestTaskState GetNodeStateInTrack(int QuestSlotIn, const FQuestTaskID& NodeIn, bool IsDialogIN) const;

protected:
	/*Gets state of node in track.*/
	UFUNCTION()
		EQuestTaskState GetNodeStateInTrackWithArrays(int QuestSlotIn, const FQuestTaskID& NodeIn, const TArray<FQuestTrack>& QuestsTrackIN) const;

	///*Finds all nodes disabled by this node that also disable this node in completion.*/
	//UFUNCTION(BlueprintCallable, Category = "Quest Nodes")
	//	TArray<FQuestTaskID> GetNodeMutuallyDisabledNodes(int QuestSlotIn, const FQuestTaskID& NodeIn)const;
	//

	/*Resets all variables in the track as if this node was never activated.*/
	UFUNCTION()
		bool ResetNodeTrack(int QuestSlotIn, const FQuestTaskID& NodeIn, TArray<FQuestTrack>& QuestsTrackIN);

	/*Updates quest track for a task with a new state.*/
	UFUNCTION()
		bool SetNodeStateInTrack(int QuestSlotIn, const FQuestTaskID& NodeIn, EQuestTaskState NewState, TArray<FQuestTrack>& QuestsTrackIN);

	/*Updates quest track for a task with a activation order*/
	UFUNCTION()
		bool SetNodeActivationOrderInTrack(int QuestSlotIn, const FQuestTaskID& NodeIn, int ActivationOrder, TArray<FQuestTrack>& QuestsTrackIN);

	/*Updates quest track for a task with a completion order*/
	UFUNCTION()
		bool SetNodeCompletionOrderInTrack(int QuestSlotIn, const FQuestTaskID& NodeIn, int CompletionOrder, TArray<FQuestTrack>& QuestsTrackIN);

	/*Sets a node to active. Changes state in track, adds to active tasks and handles activation order*/
	UFUNCTION()
		bool ActivateNodeID(int QuestSlotIn, bool IsDialogTaskIN, const FQuest& QuestIn, const FQuestTaskID& NodeIn, TArray<FQuestRep>& QuestsInventoryIN, TArray<FQuestTrack>& QuestsTrackIN, TArray<FQuestObjectTrack>& ObjectsTrackIN, TArray<FCompletedQuestTrack>& CompletedQuestsTrackIN, TArray<FQuestFailTrack>& QuestsFailTrackIN);

	/*Returns current activation order.*/
	UFUNCTION()
		int GetCurrentActivationOrder(int QuestSlotIn, TArray<FQuestTrack>& QuestsTrackIN) const;

	/*Returns current completion order.*/
	UFUNCTION()
		int GetCurrentCompletionOrder(int QuestSlotIn, TArray<FQuestTrack>& QuestsTrackIN) const;

	/**
	*	Gets the talk task with max completion Index.
	*	Returns -1 if cannot find a completed talk task.
	*/
	UFUNCTION()
		int GetLastCompletedTalkTask(int QuestSlotIn, TArray<FQuestTrack>& QuestsTrackIN) const;

	/*Return total amount of  tasks of a quest with a specific state. Takes into consideration normal and talk tasks.*/
	UFUNCTION()
		int GetTasksAmountByState(int QuestSlotIn, TArray<FQuestTrack>& QuestsTrackIN, EQuestTaskState StateIn = EQuestTaskState::Active) const;

	/*Finds all initial nodes. Initial nodes are those that do not have superior nodes.*/
	UFUNCTION()
		TArray <int>  GetStartingTasksSlots(EQuestTaskType TaskTypeIn, const FQuest& QuestIn, AActor* NPCIN, bool AnyNPC) const;
	   	 
	/*
	*	Gets all starting talk tasks, including those that are not starters but activate after a Starting ActivateCondition only task (PseudoStartTalkTasks)
	*/
	UFUNCTION()
		TArray <int>  GetStartingTalkTasksSlots(const FQuest& QuestIn, AActor* NPCIN, bool AnyNPC) const;

	/*
	*	Gets all talk tasks that are not starting, but are started after a Starting ActivateConditionOnly task is completed
	*	Propagates all ActivateConditionOnly tasks. Obtains those that will activate upon QuestAcepted.
	*/
	UFUNCTION()
		TArray <int>  GetPseudoStartingTalkTasksSlots(const FQuest& QuestIn, AActor* NPCIN, bool AnyNPC) const;
	
	/*
	*	Gets all the starting task that met the ActivateCondition and are only condition tasks
	*/
	UFUNCTION()
		TArray <int>  GetStartingTasksThatCanActivate(const FQuest& QuestIn, AActor* NPCIN, bool AnyNPC) const;
	   	 
public:
	/*Returns a array with all completed tasks sorted by activation order.*/
	UFUNCTION(BlueprintCallable, Category = "Quest Nodes")
		TArray<FQuestTaskID> GetCompletedTasksIDsByActivationOrder(int QuestSlotIn, EQuestSelectedType QuestTypeIn, EQuestTaskTypeSelection SelectionIn, bool IsDialogIn) const;

protected:
		TArray<FQuestTaskID> GetCompletedTasksIDsByActivationOrderWithArrays(int QuestSlotIn, EQuestSelectedType QuestTypeIn, EQuestTaskTypeSelection SelectionIn, const TArray<FQuestTrack>& QuestsTrackIN, const TArray<FCompletedQuestTrack>& CompletedQuestsTrackIN) const;

public:
	/*Returns a array with all completed tasks sorted by completion order.*/
	UFUNCTION(BlueprintCallable, Category = "Quest Nodes")
		TArray<FQuestTaskID> GetCompletedTasksIDsByCompletionOrder(int QuestSlotIn, EQuestSelectedType QuestTypeIn, EQuestTaskTypeSelection SelectionIn, bool IsDialogIn) const;

protected:
		TArray<FQuestTaskID> GetCompletedTasksIDsByCompletionOrderWithArrays(int QuestSlotIn, EQuestSelectedType QuestTypeIn, EQuestTaskTypeSelection SelectionIn, const TArray<FQuestTrack>& QuestsTrackIN, const TArray<FCompletedQuestTrack>& CompletedQuestsTrackIN) const;

	/**
	*	Given a array of inferior nodes of a completed objective, activates them if possible
	*	After using this function should check if quest fails. Its possible that after no node activated, the quest cannot continue and should fail.
	*/
	UFUNCTION()
		bool AttempToActivateInferiorNodes(int QuestSlotIn, bool IsDialogTaskIN, const FQuest& QuestIn, bool IsStartingNodeIn, const FQuestTaskID& SuperiorNode, const TArray<FQuestTaskID>& InferiorNodesIn, TArray<FQuestRep>& QuestsInventoryIN, TArray<FQuestTrack>& QuestsTrackIN, TArray<FQuestObjectTrack>& ObjectsTrackIN, TArray<FCompletedQuestTrack>& CompletedQuestsTrackIN, TArray<FQuestFailTrack>& QuestsFailTrackIN);
	
	/*Deactivates a node array.*/
	UFUNCTION()
		bool DeactivateNodes(int QuestSlotIn, const TArray<FQuestTaskID>& NodesIn, bool IsDialogIn, TArray<FQuestTrack>& QuestsTrackIN, TArray<FQuestObjectTrack>& ObjectsTrackIN);

	/*Deactivates a node. Returns true if the node was deactivated correctly. Ususally the operations fails if the node was already deactivated.*/
	UFUNCTION()
		bool DeactivateSingleNode(int QuestSlotIn, const FQuestTaskID& NodeIn, bool IsDialogIn, TArray<FQuestTrack>& QuestsTrackIN, TArray<FQuestObjectTrack>& ObjectsTrackIN);

	/**
	*	Handles node fail. 
	*	Only relevant for tasks.
	*	When a task fails changes state to fail and is removed from actives. Also the object is destroyed. All nodes disabled on complete, also fail.
	*/
	UFUNCTION()
		bool  FailNode(int QuestSlotIn, const FQuest& QuestIn, bool IsDialogIn, const FQuestTaskID& NodeIn, TArray<FQuestTrack>& QuestsTrackIN, TArray<FQuestObjectTrack>& ObjectsTrackIN);

	/*Handles node fail, state and dispatchers*/
	UFUNCTION()
		bool  SingleNodeFail(int QuestSlotIn, const FQuest& QuestIn, bool IsDialogIn, const FQuestTaskID& NodeIn, TArray<FQuestTrack>& QuestsTrackIN, TArray<FQuestObjectTrack>& ObjectsTrackIN);

	/*Fails a array of nodes.*/
	UFUNCTION()
		bool MultipleNodesFail(int QuestSlotIn, const FQuest& QuestIn, bool IsDialogIn, const TArray<FQuestTaskID>& NodesIn, TArray<FQuestTrack>& QuestsTrackIN, TArray<FQuestObjectTrack>& ObjectsTrackIN);

	/*Handles node lock, state and dispatchers. This is not normally called on the execution.*/
	UFUNCTION()
		bool  SingleNodeLock(int QuestSlotIn, const FQuest& QuestIn, const FQuestTaskID& NodeIn, bool IsDialogIn, TArray<FQuestTrack>& QuestsTrackIN, TArray<FQuestObjectTrack>& ObjectsTrackIN);
public:
	/**
	*	Handles node lock for all nodes that are containened in between two nodes.
	*	Includes both nodes provided
	*	If nodes are the same, then it blocks only that node.
	*/
	UFUNCTION()
		bool  LockNodesInBetweenNodes(int QuestSlotIn, const FQuestTaskID& StartNodeIn, const FQuestTaskID& EndNodeIn, bool IsDialogIN);

protected:
	/**
	*	Handles node lock for all nodes that are containened in between two nodes. 
	*	Includes both nodes provided
	*	If nodes are the same, then it blocks only that node.
	*/
	UFUNCTION()
		bool  LockNodesInBetweenNodesWithArrays(int QuestSlotIn, bool IsDialogIn, const FQuestTaskID& StartNodeIn, const FQuestTaskID& EndNodeIn, TArray<FQuestRep>& QuestsInventoryIN, TArray<FQuestTrack>& QuestsTrackIN, TArray<FQuestObjectTrack>& ObjectsTrackIN);

	/**
	*	From starting node to end node, it will propagate up and filter all nodes that are not locked.
	*	If there is no propagating connection between nodes, this will end when there is no more nodes.
	*	@Param IncludeDisabledNodesIn If this is set to true, all disabled nodes for the nodes involved will be obtained as well. This nodes will not contribute to the propagation of superior nodes.
	*/
	UFUNCTION()
		TArray<FQuestTaskID>  GetAllNotLockedNodesBetweenNodes(int QuestSlotIn, const FQuest& QuestIn, const FQuestTaskID& StartNodeIn, const FQuestTaskID& EndNodeIn, bool IncludeDisabledNodesIn, const TArray<FQuestTrack>& QuestsTrackIN);

	/*Gets all exclusive links to this tasks. Exclusive links are those only conected to this tasks.*/
	UFUNCTION()
		TArray<FQuestTaskID> GetTasksExclusivelyLinkedToNode(int QuestSlotIn, const FQuest& QuestIn, const FQuestTaskID& NodeIn, const TArray<FQuestTrack>& QuestsTrackIN)const;

	/**
	*	Determines if a node has a exclusive path possible.
	*	If the node has only one inferior node capable of activating or activated or preactivate then the node is exclusive.
	*	The node itself must be capable of activate or be activated or preactivated. If the node is completed or disabled then is not exclusive.
	*	All paths that are disabled or completed or enhances the possibilities of the node to be exclusive.
	*	This function is to be used withing GetTasksExclusivelyLinkedToNode.
	*	@Param NodeIn Node currently being analized.
	*	@Param BottomNodeIn Node that starts the operation. Last node in  the chain.
	*/
	UFUNCTION()
		bool IsNodeExclusive(int QuestSlotIn, const FQuest& QuestIn, const FQuestTaskID& NodeIn, const FQuestTaskID& BottomNodeIn, const TArray<FQuestTrack>& QuestsTrackIN)const;
	
	/*Handles the node completion, track update, inactivation and activation of connected nodes.*/
	UFUNCTION()
		bool CompleteNode(int QuestSlotIn, bool IsDialogTaskIN, const FQuest& QuestIn, const FQuestTaskID& NodeIn, TArray<FQuestRep>& QuestsInventoryIN, TArray<FQuestTrack>& QuestsTrackIN, TArray<FQuestObjectTrack>& ObjectsTrackIN, TArray<FCompletedQuestTrack>& CompletedQuestsTrackIN, TArray<FQuestFailTrack>& QuestsFailTrackIN);
	
	/**
	*	Corrects Quest Slot if needed, in case the execution changed it.
	*	If there is a task in the CompleteNode execution (Inferior nodes activation) that modifies the lenght of active array, sometimes can corrupt the QuestSlot.
	*	@Param QuestRepIn Must be the quest before all the execution of Inferior nodes activation happends. Need to store in local variable.
	*/
	UFUNCTION()
		int CorrectQuestSlotForNodeExecution(int QuestSlotIn, bool IsDialogTaskIN, const FQuestRep& QuestRepIn, const TArray<FQuestRep>& QuestsInventoryIN) const;

	/**
	*	Determines if the quest slot is still valid for the CompleteNode execution.
	*	If there is a task in the CompleteNode execution (Inferior nodes activation) that modifies the lenght of active array, sometimes can corrupt the QuestSlot. 
	*	@Param QuestRepIn Must be the quest before all the execution of Inferior nodes activation happends. Need to store in local variable.
	*/
	UFUNCTION()
		bool IsQuestSlotAfterNodeExecutionValid(int QuestSlotIn, bool IsDialogTaskIN, const FQuestRep& QuestRepIn, const TArray<FQuestRep>& QuestsInventoryIN) const;

	/**
	*	Find the correct quest slot of a given Quest.	
	*	@Param QuestRepIn Must be the quest before all the execution of Inferior nodes activation happends. Need to store in local variable.
	*/
	UFUNCTION()
		int GetValidQuestSlotAfterNodeExecution(const FQuestRep& QuestRepIn, const TArray<FQuestRep>& QuestsInventoryIN) const;

	//-----------------------------------------Tasks

	/**Updates amount of a task in the quest track*/
	UFUNCTION()
		bool  UpdateAmmountTaskTrack(int QuestSlotIn, bool IsDialogTaskIN, const FQuest& QuestIn, int TaskIN, int AmountIN, TArray<FQuestRep>& QuestsInventoryIN, TArray<FQuestTrack>& QuestsTrackIN, TArray<FQuestObjectTrack>& ObjectsTrackIN, TArray<FCompletedQuestTrack>& CompletedQuestsTrackIN, TArray<FQuestFailTrack>& QuestsFailTrackIN);

	/**Spawns and start a BaseQuestTask object*/
	UFUNCTION()
		bool  HandleBaseTaskSpawn(int QuestSlotIN, const FQuest& QuestIn, int TaskSlotIN, bool IsDialogTaskIN, TArray<FQuestObjectTrack>& ObjectsTrackIN, bool IsLoading = false);

	/**Executes the save logic for the task.*/
	UFUNCTION()
		bool  HandleBaseTaskSave(int QuestSlotIN, int TaskSlotIN, TArray<FQuestObjectTrack>& ObjectsTrackIN);

	/**Executes the save logic for all active tasks for all active quests.*/
	UFUNCTION()
		bool  HandleAllTasksSave(TArray<FQuestObjectTrack>& ObjectsTrackIN);

	/*Destroys a existing object for a task that is no longer active*/
	UFUNCTION()
		bool  HandleBaseTaskDestroy(int QuestSlotIN, int TaskSlotIN, TArray<FQuestObjectTrack>& ObjectsTrackIN);

	/*Destroys all existing objects for active tasks of a quest in particular. Called when a quest is removed from the inventory*/
	UFUNCTION()
		bool  DestroyAllRemainingBaseTasksObjectsForQuest(int QuestSlotIN, TArray<FQuestObjectTrack>& ObjectsTrackIN);

	/*Destroys all existing objects for all quests in the manager. Called when manager is destroyed*/
	UFUNCTION()
		bool  DestroyAllRemainingBaseTasksObjects(TArray<FQuestObjectTrack>& ObjectsTrackIN);

	/*Generates objects track for all quests. Used when loading a game, needs to regenerate all active slots objects and resize object track*/
	UFUNCTION()
		bool  GenerateEmptyObjectsTrack(TArray<FQuestRep>& QuestInventoryIN, TArray<FQuestObjectTrack>& ObjectsTrackIN);

	/*Generates a quest object track*/
	UFUNCTION()
		bool  GenerateEmptyObjectsTrackForQuest(const FQuest& QuestIN, TArray<FQuestObjectTrack>& ObjectsTrackIN);

	/**
	*	Updates a task object QuestSlots after a quest is removed.	
	*	Any higher slot than the removed one is lowered in 1.
	*/
	UFUNCTION()
		bool  ReduceQuestSlotForTaskObjectsAfterRemove(int QuestSlotIn, int TaskSlotIn, int RemovedQuestSlotIn, TArray<FQuestObjectTrack>& ObjectsTrackIN);

	/**
	*	Updates all task objects QuestSlots after a quest is removed.
	*	Any higher slot than the removed one is lowered in 1.
	*/
	UFUNCTION()
		bool  ReduceAllQuestSlotsObjectsAfterRemove(int RemovedQuestSlotIn, TArray<FQuestObjectTrack>& ObjectsTrackIN);

public:

	/*Update Current payload of a quest task. The payload is overwritten*/
	UFUNCTION(BlueprintCallable, Category = "Quest Task")
		bool SetTaskCurrentPayload(int QuestSlotIN, int TaskSlotIn, bool IsDialogTaskIn, const FQuestTaskPayload& NewPayloadIn);

	/*Get Current payload of a quest task.*/
	UFUNCTION(BlueprintCallable, Category = "Quest Task")
		FQuestTaskPayload GetTaskCurrentPayload(int QuestSlotIN, int TaskSlotIn, bool IsDialogTaskIn);

	/*Get Initial payload of a quest task. This payload is the one is set in the quest struct and is constant.*/
	UFUNCTION(BlueprintCallable, Category = "Quest Task")
		FQuestTaskPayload GetTaskInitialPayload(int QuestSlotIN, int TaskSlotIn, bool IsDialogTaskIn);

	/*Destroys all existing objects for all quests in the manager. Called when manager is destroyed*/
	UFUNCTION(BlueprintCallable, Category = "Quest Task")
		TArray <int>  GetActiveTasksOfClass(int QuestIN, bool IsDialogIn, const TSoftClassPtr<UBaseQuestTask>& TaskClassIn);
	
	/*Handles tasks end event of a BaseQuestTask*/
	UFUNCTION()
		bool  HandleTaskEnd(int QuestIN, int TaskIN, bool IsDialogTaskIN, bool ResultIN, int AmountIN);

	/*Handles tasks end event of a BaseQuestTask with customizable arrays*/
	UFUNCTION()
		bool  HandleTaskEndWithArrays(int QuestIN, int TaskIN, bool IsDialogTaskIN, bool ResultIN, int AmountIN,
			TArray<FQuestRep>& QuestsInventoryIN, TArray<FQuestTrack>& QuestsTrackIN, TArray<FQuestObjectTrack>& ObjectsTrackIN, TArray<FCompletedQuestTrack>& CompletedQuestsTrackIN, TArray<FQuestFailTrack>& QuestsFailTrackIN);

	/**
	*	Handles tasks end event of a ResetTrackToNode_QuestTask
	*	This is a bit outside of the normal end task. This will handle lock of nodes between the nodes selected.
	*/
	UFUNCTION()
		bool  HandleTaskEnd_ResetTrackToNode(int QuestIN, const FQuestTaskID& StartNodeIn, const FQuestTaskID& EndNodeIn, bool IsDialogTaskIN, bool IncludeDisabledNodesIn);

	/**
	*	Handles tasks end event of a ResetTrackToNode_QuestTask
	*	This is a bit outside of the normal end task. This will handle lock of nodes between the nodes selected.
	*/
	UFUNCTION()
		bool  HandleTaskEnd_ResetTrackToNodeWithArrays(int QuestIN, bool IsDialogTaskIN, const FQuestTaskID& StartNodeIn, const FQuestTaskID& EndNodeIn, bool IncludeDisabledNodesIn,
			TArray<FQuestRep>& QuestsInventoryIN, TArray<FQuestTrack>& QuestsTrackIN, TArray<FQuestObjectTrack>& ObjectsTrackIN, TArray<FCompletedQuestTrack>& CompletedQuestsTrackIN, TArray<FQuestFailTrack>& QuestsFailTrackIN);

	/*Returns the specified quest task*/
	UFUNCTION(BlueprintCallable, Category = "Quest Task")
		FQuestTask  GetTask(EQuestSelectedType SelectType, int QuestIN, int TaskIn, bool IsDialogIn) const;

	/*Returns the specified quest task track*/
	UFUNCTION(BlueprintCallable, Category = "Quest Task")
		FTaskTrack  GetTaskTrack(EQuestSelectedType SelectType, int QuestIN, int TaskIn, bool IsDialogIn) const;

	/*Gets the associated instance of the object for this task. Returns null if there is none.*/
	UFUNCTION(BlueprintCallable, Category = "Quest Task")
		UBaseQuestTask* GetTaskObject(int QuestIN, int TaskIn, bool IsDialogIn) const;

	//--------------------------------------------Talk Tasks
protected:

	/*Finds talk text info of talk task. Given the talk task name and text, it will find the slot info. This changes with different NPCS.*/
	UFUNCTION()
		int  FindTalkTextSlotOfTask(int QuestIn, int TalkTaskSlotIN, AActor* NPCIN, bool IsDialogIn, FTalkTextInfo& TalkInfoOut) const;

	/**	
	*	Get All completed talk tasks that are connected to any active one and from same NPC
	*	This function does the same than GetCompletedTalkTasksForNPC, using that one instead. 
	*	Solves the problem in a total different way.
	*/
	UFUNCTION()
		TArray <int>  FindReadedTalkTasks(int QuestSlotIn, const FQuest& QuestIn, const TArray <int>& ActiveTalkTasksIn, const FNPCQuestDefRep& NPCIN, const TArray<FQuestTrack>& QuestsTrackIN) const;

public:
	/*Returns the specified quest task*/
	UFUNCTION(BlueprintCallable, Category = "Quest Talk Task")
		FQuestTalkTask  GetTalkTask(EQuestSelectedType SelectType, int QuestIN, int TaskIn, bool IsDialogIn) const;

	/*Returns the specified quest talk task track*/
	UFUNCTION(BlueprintCallable, Category = "Quest Talk Task")
		FTalkTaskTrack  GetTalkTaskTrack(EQuestSelectedType SelectType, int QuestIN, int TaskIn, bool IsDialogIn) const;

	//----------------------------------------Dialogs

	/**
	*	Function called when you reach a NPC.
	*	Must be called on client.
	*	@PARAM CompleteFirstDialogIn You can choose if you want to commit instantly first dialog for this NPC.
		CompleteFirstDialogIn In Conversation mode for Example UI should set CompleteFirstDialogIn to true.
		In Single Sentence mode for Example UI should set CompleteFirstDialogIn to false.
	*/
	UFUNCTION(BlueprintCallable, Category = "Dialogs")
		bool  ReachedNPC(AActor* NPCIN, bool CompleteFirstDialogIn = true);

protected:
	/*Handles commiting first dialog when reaching NPC*/
	UFUNCTION()
		bool  CommitFirstNPCDialog(AActor* NPCIN);

public:
	/**
	*	Removes all the NPC dialogs from the dialog track. 
	*	This will cause to reset their active dialogs progress.
	*	May affect other NPCs dialogs referenced in this NPC aswell.
	*	The NPC will give their Dialogs again, as if you have never talked to them.
	*/
	UFUNCTION(BlueprintCallable, Category = "Dialogs")
		bool  RemoveNPCFromDialogTrack(AActor* NPCIN);

	/**
	*	Removes all the NPC dialogs from the dialog track.
	*	This will cause to reset their active dialogs progress.
	*	This does not affect completed dialogs.
	*	May affect other NPCs dialogs referenced in this NPC aswell.
	*	The NPC will give their Dialogs again, as if you have never talked to them.
	*/
	UFUNCTION(BlueprintCallable, Category = "Dialogs")
		bool  RemoveNPCDefFromDialogTrack(const FNPCQuestDefRep& NPCDefIn);

	/**
	*	Removes all the NPC dialogs from the dialog track and completed an failed track.
	*	This will cause to reset all their dialogs progress.
	*	May affect other NPCs dialogs referenced in this NPC aswell.
	*	The NPC will give their Dialogs again, as if you have never talked to them.
	*	If this is called from a task of a dialog of said NPC, that dialog will be ignored to avoid crashes. You should abandon the dialog after calling the task for this.
	*/
	UFUNCTION()
		void  ResetNPCDialogsFromTask(int QuestSlotIn, bool IsDialogIn, const FNPCQuestDefRep& NPCDefIn);

	/**
	*	Removes all the NPC dialogs from the dialog track and completed an failed track.
	*	This will cause to reset all their dialogs progress.
	*	May affect other NPCs dialogs referenced in this NPC aswell.
	*	The NPC will give their Dialogs again, as if you have never talked to them.
	*	Do not call from a Quest Task.
	*/
	UFUNCTION(BlueprintCallable, Category = "Dialogs")
		void  ResetNPCDialogs(AActor* NPCIN);

	/**
	*	Removes all the NPC dialogs from the dialog track and completed an failed track.
	*	This will cause to reset all their dialogs progress.
	*	May affect other NPCs dialogs referenced in this NPC aswell.
	*	The NPC will give their Dialogs again, as if you have never talked to them.
	*	Do not call from a Quest Task.
	*/
	UFUNCTION(BlueprintCallable, Category = "Dialogs")
		void  ResetNPCDialogsFromDef(const FNPCQuestDefRep& NPCDefIn);

protected:
	/*Resets status for a dialog array*/
	UFUNCTION()
		void  ResetNPCDialogsWithArray(const TArray<FQuestRep>& DialogsArrayIn);

	//------------------------------------------Talk
public:
	/*Finds all active talktasks that involves this npc.*/
	UFUNCTION()
		TArray <int> GetActiveTalkTasksForNPC(int QuestSlotIn, const FQuest& QuestIn, const FNPCQuestDefRep& NPCDefIn, const TArray<FQuestTrack>& QuestTrackIN) const;

	/*Finds all completed talktasks that involves this npc.*/
	UFUNCTION()
		TArray <int> GetCompletedTalkTasksForNPC(int QuestSlotIn, const FQuest& QuestIn, const FNPCQuestDefRep& NPCDefIn, const TArray<FQuestTrack>& QuestTrackIN) const;

	/*Finds all Pre active talk texts information for a NPC of a quest.*/
	UFUNCTION()
		TArray <FTalkTextInfo>  GetPreActiveTalkTextsOfQuest(int QuestSlotIn, bool IsDialogIN, const FQuest& QuestIn, const FNPCQuestDefRep& NPCDefIn, const TArray<FQuestTrack>& QuestTrackIN) const;

	/*Finds all AcceptingQuest talk texts information for an NPC.*/
	UFUNCTION()
		TArray <FTalkTextInfo>  GetAcceptQuestTalkTextsOfNPC(bool IsDialogIN, AActor* NPCIN, const TArray<FQuestRep>& NPCQuestsRepIN) const;

	/*Finds all Active and completed talk texts information for a NPC of a quest.*/
	UFUNCTION()
		TArray <FTalkTextInfo>  GetActiveTalkTextsOfQuest(int QuestSlotIn, bool IsDialogIN, const FQuest& QuestIn, const FNPCQuestDefRep& NPCDefIn, const TArray<FQuestTrack>& QuestTrackIN) const;

	/*Finds all talk texts information for a NPC and a given set off array.*/
	UFUNCTION(BlueprintCallable, Category = "Quest Talk Task")
		TArray <FTalkTextInfo>  GetTalkTextsInfoWithArrays(bool IsDialogIN, AActor* NPCIN, const FNPCQuestDefRep& NPCDefIN, const TArray<FQuestRep>& NPCQuestsRepIN, const TArray<FQuestRep>& QuestInventoryIN, const TArray<FQuestTrack>& QuestsTrackIN) const;

	/*Finds all talk texts information for a NPC.*/
	UFUNCTION(BlueprintCallable, Category = "Quest Talk Task")
		TArray <FTalkTextInfo>  GetTalkTextsInfo(AActor* NPCIN) const;

	/**
	*	Finds the TalkTextInfo for a particular talk task in a quest.
	*/
	UFUNCTION(BlueprintCallable, Category = "Quest Talk Task")
		FTalkTextInfo  GetTalkTextInfoForTalkTask(AActor* NPCIN, int QuestSlotIn, int TalkTaskIDIn, bool IsDialogIN) const;

	/*Determines if a talktext info is still valid.*/
	UFUNCTION(BlueprintCallable, Category = "Quest Talk Task")
		bool  IsTalkTextsInfoValid(AActor* NPCIN, const FTalkTextInfo& TalkTextInfoIn) const;

	/**	
	*	Determines if two talk texts are exactly equal.
	*	This is a check that is more restrictive that the normal "==".
	*	Determines if two talktexts are equal and with same state, type and quest slot. Normally this variables are skipped since they can represent still the same talk text in a quest.
	*/
	UFUNCTION(BlueprintCallable, Category = "Quest Talk Task")
		bool  AreTalkTextsExactlyEqual(AActor* NPCIN, const FTalkTextInfo& TalkTextInfoA, const FTalkTextInfo& TalkTextInfoB) const;
		
	/**
	*	Function called when talk task or dialog is selected by the client to be readed or completed. 
	*	This starts all the nodes checks.
	*	This is predicted, executing client and server logic.	
	*	In most cases NPC should be the CurrentNPC, stored in the quest manager, that is set on ReachedNPC. This is not directly provided for flexibility, as you still can use this function with any NPC.
	*/
	UFUNCTION(BlueprintCallable, Category = "Quest Talk Task")
		void  Pred_TalkToNPC(const FTalkTextInfo& TalkTextInfoIn, AActor* NPCIN);

	/*Gets all unique short desc from all active talk tasks for this quest*/
	UFUNCTION(BlueprintCallable, Category = "Quest Talk Task")
		TArray <FText>  GetAllActiveTalkTasksShortDesc(int QuestSlotIn, bool IsDialogIn) const;

protected:

	/*Determines if two talk tasks are connected by another talktasks of same NPC. This means that these talk tasks are part of the same conversation.*/
	UFUNCTION()
		bool  AreTalkTasksConnected(int QuestSlotIn, const FQuest& QuestIn, int TalkTaskA, int TalkTaskB) const;

public:
	/*Finds if a superior node has a override of the short description and uses that instead*/
	UFUNCTION(BlueprintCallable, Category = "Quest Talk Task")
		FText  GetTalkTasksInheritedShortDescription(int QuestSlotIn, int TalkTaskIn, bool IsDialogIn) const;

protected:

	/**
	*	Get all different conversations ID for a node array
	*	A conversation start when the activated node is the first of the conversation on the active track.
	*	Nodes that are not talk will be ignored.
	*/
	UFUNCTION()
		TArray <int>  GetAllConversationsInTalkNodeArray(int QuestSlotIn, const FQuest& QuestIn, const TArray <FQuestTaskID>& NodeArray) const;

	/**
	*	Called after a node is completed or a quest is accepted(no need to check on node activate)
	*	Must feed the previous active track of tasks. The means the track from before the node start to complete, when it was still active.
	*	Will compare with the new quest track and determine wich conversations started and wich conversations ended.
	*	A conversation start when the activated node is the first of the conversation on the active track.
	*	A conversation ends when there is no active nodes of that conversation active anymore.
	*/
	UFUNCTION()
		void  CheckConversationStartAndEnd(int QuestSlotIn, const FQuest& QuestIn, bool IsDialogIn, const TArray <FQuestTaskID>& PreviousActiveTrack, const TArray<FQuestTrack>& QuestTrackIN) const;


	//---------------------------Single Sentence mode
public:

	/**
	*	Given a talk text, determines all its nodes of the provided type and finds the talk tasks.
	*	Also gives
	*	@Param TalkText Talk text to use.
	*	@Param NPCIn Current NPC. Must be provided for quests that have not been accepted yet.
	*/

	UFUNCTION(BlueprintCallable, Category = "Quest Talk Task")
		TArray<FQuestTalkTask> GetTalkTextNodes(AActor* NPCIn, const FTalkTextInfo& TalkTextsIn, ENodeConnectorType TypeIn, TArray<int>& TalkTasksIDsOut, FQuest& QuestOut);

	/**
	*	This will filter talktext in order of priority for a given mode
	*	@Param ActiveTalkTextsIn All current talk texts for NPC.
	*	@Param	AllowedStates The states to filter. When there is not NPC active ones, i need to find the completed ones.
	*	@Param SSDataIn Correlated single sentence data for the active talktexts.
	*	@Param SSDataOut Correlated single sentence data for the filtered talktexts.
	*/
	UFUNCTION(BlueprintCallable, Category = "Single Sentence mode")
		TArray <FTalkTextInfo> GetSingleSentencePriorityTalkTextsForMode(AActor* NPCIn, const TArray <FTalkTextInfo>& ActiveTalkTextsIn, const ETalkTextMode& AllowedMode, const TArray<EQuestTaskState>& AllowedStates, const TArray <FSingleSentenceModeData>& SSDataIn, TArray <FSingleSentenceModeData>& SSDataOut);

	/**
	*	This will filter talktext in order of priority
	*	First are quest and then dialogs.
	*	@Param ActiveTalkTextsIn All current talk texts for NPC.
	*	@Param	AllowedStates The states to filter. When there is not NPC active ones, i need to find the completed ones.
	*	@Param SSDataIn Correlated single sentence data for the active talktexts.
	*	@Param SSDataOut Correlated single sentence data for the filtered talktexts.
	*/
	UFUNCTION(BlueprintCallable, Category = "Single Sentence mode")
		TArray <FTalkTextInfo> GetSingleSentencePriorityTalkTexts(AActor* NPCIn, const TArray <FTalkTextInfo>& ActiveTalkTextsIn, const TArray<EQuestTaskState>& AllowedStates, const TArray <FSingleSentenceModeData>& SSDataIn, TArray <FSingleSentenceModeData>& SSDataOut);

	/**
	*	From a group of talk texts, determines the lower quest slot.
	*	@Param ActiveTalkTextsIn All talk texts to filter
	*/
	UFUNCTION(BlueprintCallable, Category = "Single Sentence mode")
		int GetTalkTextsLowerQuestSlot(const TArray <FTalkTextInfo>& ActiveTalkTextsIn);

	/**
	*	This will get all texts that are for SSD, for player or NPC.
	*	@Param ActiveTalkTextsIn All talk texts active.
	*	@Param SSDataIn Correlated single sentence data for the active talktexts.
	*	@Param SSDataOut Correlated single sentence data for the filtered talktexts.
	*/
	UFUNCTION(BlueprintCallable, Category = "Single Sentence mode")
		TArray <FTalkTextInfo> GetTalkTextsFilteredForSSD(AActor* NPCIn, const TArray <FTalkTextInfo>& ActiveTalkTextsIn, const TArray<EQuestTaskState>& AllowedStates, const TArray <FSingleSentenceModeData>& SSDataIn, EDialogType FilterTypeIn, TArray <FSingleSentenceModeData>& SSDataOut);

	/**
	*	This will get the talk text that the NPC supposedly said last.
	*	@Param ActiveTalkTextsIn All Talk Texts for NPC.
	*	@Param SSDataOut Talk text SSdata.
	*/
	UFUNCTION(BlueprintCallable, Category = "Single Sentence mode")
		FTalkTextInfo GetNPCLastCommitedTalkTextForSSD(AActor* NPCIn, const TArray <FTalkTextInfo>& ActiveTalkTextsIn, const TArray <FSingleSentenceModeData>& SSDataIn, FSingleSentenceModeData& SSDataOut, bool& FoundLastCommitedText);
				
	//------------------------------------------Quest Track
protected:
	/*Load quest track. Reespawns all tasks objects for all quests. Called on manager load quests */
	UFUNCTION()
		void  LoadQuestsTrack(bool IsDialogIN, TArray<FQuestRep>& QuestInventoryIN, TArray<FQuestTrack>& QuestTrackIN, TArray<FQuestObjectTrack>& ObjectsTrackIN);

	/*Adds a quest to the quest track*/
	UFUNCTION()
		void  AddQuestTrack(const FQuest& QuestIN, TArray<FQuestTrack>& QuestTrackIN, TArray<FQuestObjectTrack>& ObjectsTrackIN);
	
	/*Adds a quest ID to the quest track active tasks.*/
	UFUNCTION()
		bool  AddTaskIDToActiveTrack(int QuestSlotIn, const FQuestTaskID& TaskIDIn, TArray<FQuestTrack>& QuestTrackIN);

	/*Removes a quest ID from the quest track active tasks.*/
	UFUNCTION()
		bool  RemoveTaskIDFromActiveTrack(int QuestSlotIn, const FQuestTaskID& TaskIDIn, TArray<FQuestTrack>& QuestTrackIN, TArray<FQuestObjectTrack>& ObjectsTrackIN);

	/**	
	*	Sets the NodeHandler if possible.
	*	This wont be possible if another node is handling it.
	*/
	UFUNCTION()
		bool  NodeStartHandlingOperation(int QuestSlotIn, const FQuestTaskID& TaskIDIn, TArray<FQuestTrack>& QuestTrackIN);

	/**
	*	Sets the NodeHandler to invalid if possible.
	*	This wont be possible if no node is handling it.
	*/
	UFUNCTION()
		bool  NodeStopHandlingOperation(int QuestSlotIn, const FQuestTaskID& TaskIDIn, TArray<FQuestTrack>& QuestTrackIN);

	/**
	*	Determines if any node (dont care wich one) is handling a operation.
	*/
	UFUNCTION()
		bool  IsAnyNodeHandlingOperation(int QuestSlotIn, TArray<FQuestTrack>& QuestTrackIN);

	/**
	*	Determines if a node is currently handling the quest operation.
	*/
	UFUNCTION()
		bool  IsNodeTheOperationHandler(int QuestSlotIn, const FQuestTaskID& TaskIDIn, TArray<FQuestTrack>& QuestTrackIN);

	/*Get all active tasks from provided track*/
	UFUNCTION()
		TArray <int>  GetActiveTasksSlotsFromTrackWithArrays(int QuestSlotIn, EQuestTaskType TaskTypeIn, const TArray<FQuestTrack>& QuestTrackIN) const;

public:
	/*Get all active tasks from Quest track*/
	UFUNCTION(BlueprintCallable, Category = "Quest Task Track")
		TArray <int>  GetActiveTasksSlotsFromTrack(int QuestSlotIn, EQuestTaskType TaskTypeIn) const;
	
	//-----------------------------------------Fail Track

protected:
	/**
	*	Handle quest fail track upon accept.
	*/
	UFUNCTION()
		void HandleQuestFailTrackOnAccept(const FQuestRep& QuestRepIn, bool IsDialogIn, TArray<FQuestFailTrack>& QuestsFailTrackIN);

	/**
	*	Add quest to fail track if is not already there.
	*/
	UFUNCTION()
		bool AddQuestToFailtrack(const FQuestRep& QuestRepIn, TArray<FQuestFailTrack>& QuestsFailTrackIN);
public:
	/**
	*	Remove quest to fail track if is already there.
	*/
	UFUNCTION()
		bool RemoveQuestFromFailTrack(bool IsDialogIN, const FQuestRep& QuestRepIn);

protected:
	/**
	*	Remove quest to fail track if is already there.
	*/
	UFUNCTION()
		bool RemoveQuestFromFailTrackWithArrays(const FQuestRep& QuestRepIn, TArray<FQuestFailTrack>& QuestsFailTrackIN);

protected:
	/**
	*	Get quest fail track index if is already there. 
	*	If cannot find, returns -1.
	*/
	UFUNCTION()
		int GetQuestFailtrackIndex(const FQuestRep& QuestRepIn, const TArray<FQuestFailTrack>& QuestsFailTrackIN) const;

public:
	/**
	*	Determines if a quest is in the fail quest track.
	*	Returns the fail track of the quest if is failed.
	*/
	UFUNCTION()
		bool IsQuestFailed(bool IsDialogIN, const FQuestRep& QuestRepIn) const;

protected:
	/**
	*	Determines if a quest is in the fail quest track.
	*	Returns the fail track of the quest if is failed.
	*/
	UFUNCTION()
		bool IsQuestFailedWithArrays(const FQuestRep& QuestRepIn, bool IsDialogIn, const TArray<FQuestFailTrack>& QuestsFailTrackIN) const;

public:
	/**
	*	Determines if a quest has a fail time that is equal or higher than the time provided
	*/
	UFUNCTION()
		bool HasQuestFailTimeSpan(bool IsDialogIN, const FQuestRep& QuestRepIn, const FTimespan& QuestFailTimeIn) const;

protected:
	/**
	*	Determines if a quest has a fail time that is equal or higher than the time provided
	*/
	UFUNCTION()
		bool HasQuestFailTimeSpanWithArrays(const FQuestRep& QuestRepIn, const FTimespan& QuestFailTimeIn, const TArray<FQuestFailTrack>& QuestsFailTrackIN) const;

	//-----------------------------------------Pre activation

protected:

	/*Adds a quest ID to the quest track PreActive tasks.*/
	UFUNCTION()
		bool  AddTaskIDToPreActiveTrack(int QuestSlotIn, const FQuestTaskID& TaskIDIn, TArray<FQuestTrack>& QuestTrackIN);

	/*Removes a quest ID from the quest track PreActive tasks.*/
	UFUNCTION()
		bool  RemoveTaskIDFromPreActiveTrack(int QuestSlotIn, const FQuestTaskID& TaskIDIn, TArray<FQuestTrack>& QuestTrackIN);

	/**
	*	Activates a preactive tasks for a determined talk text.
	*/
	UFUNCTION()
		bool  ActivateTalkTaskFromPreActiveTrack(int QuestSlotIn, bool IsDialogTaskIN, const FQuest& QuestIn, int TalkTaskSlotIn, TArray<FQuestRep>& QuestsInventoryIN, TArray<FQuestTrack>& QuestTrackIN, TArray<FQuestObjectTrack>& ObjectsTrackIN, TArray<FCompletedQuestTrack>& CompletedQuestsTrackIN, TArray<FQuestFailTrack>& QuestsFailTrackIN);

	/*Pre activates a task.*/
	UFUNCTION()
		bool  PreActivateTask(int QuestSlotIn, const FQuestTaskID& TaskIDIn, bool IsDialogIn, TArray<FQuestTrack>& QuestTrackIN);

	/**
	*	Preactivates all initial talk tasks from a quest.
	*/
	UFUNCTION()
		bool  PreActivateAllInitialTalkTasks(int QuestSlotIn, const FQuest& QuestIn, bool IsDialogIn, TArray<FQuestTrack>& QuestTrackIN);

	/*Get all Pre active tasks from track*/
	UFUNCTION()
		TArray <int>  GetPreActiveTasksSlotsFromTrack(int QuestSlotIn, EQuestTaskType TaskTypeIn, const TArray<FQuestTrack>& QuestTrackIN) const;

	/*Get all Pre active tasks from track from NPC*/
	UFUNCTION()
		TArray <int>  GetPreActiveTalkTasksSlotsFromTrack(int QuestSlotIn, const FQuest& QuestIn, const FNPCQuestDefRep& NPCDefIn, const TArray<FQuestTrack>& QuestTrackIN) const;

	//---------------------------------------Variable Get Internal from varying inventory
	
	/*Converts an array fo quest rep to quest.*/
	UFUNCTION()
		TArray <FQuest>  ConvertQuestsInventoryRep(const TArray<FQuestRep>& QuestInventoryIN) const;

public:
	//------------------------------------------Display
	/*Change the selected quest slot.*/
	UFUNCTION(BlueprintCallable, Category = "Quest Display")
		bool  SetSelectedQuest(int NewSelectedQuestIN);
	
	/*Change the selected completed quest slot.*/
	UFUNCTION(BlueprintCallable, Category = "Quest Display")
		bool  SetSelectedQuestCompleted(int NewSelectedQuestCompIN);
	
	//----------------------------------------Variable Gets---------------------------
	/*Gets all active quests.*/
	UFUNCTION(BlueprintCallable, Category = "Quest Display")
		TArray <FQuest>  GetQuestsInventory() const;

	/*Gets all active quests in rep form.*/
	UFUNCTION(BlueprintCallable, Category = "Quest Display")
		TArray <FQuestRep>  GetQuestsRepInventory() const;

	/*Gets all active dialogs in rep form.*/
	UFUNCTION(BlueprintCallable, Category = "Quest Display")
		TArray <FQuestRep>  GetDialogsRepInventory() const;

	/**	
	*	Gets a quest in a specific slot. 
	*	Can be a active or a completed quest.
	*/
	UFUNCTION(BlueprintCallable, Category = "Quest Display")
		FQuest  GetQuestsInSlot(EQuestSelectedType SelectType, int QuestSLot, bool IsDialogIn) const;

	/**
	*	Gets a quest track in a specific slot. 
	*	Can be a active or a completed quest.
	*/
	UFUNCTION(BlueprintCallable, Category = "Quest Display")
		FQuestTrack  GetQuestTrackInSlot(EQuestSelectedType SelectType, int QuestSLot, bool IsDialogIn) const;
		
	/*Gets a selected quest. Can be a active or a completed quest.*/
	UFUNCTION(BlueprintCallable, Category = "Quest Display")
		FQuest  GetSelectedQuests(EQuestSelectedType SelectType) const;

	/**
	*	Gets a selected quest track. 
	*	Can be a active or a completed quests.
	*/
	UFUNCTION(BlueprintCallable, Category = "Quest Display")
		FQuestTrack GetSelectedQuestsTrack(EQuestSelectedType SelectType) const;

	/*Gets a selected quest slot. Can be a active or a completed quest.*/
	UFUNCTION(BlueprintCallable, Category = "Quest Display")
		int GetSelectedQuestsSlot(EQuestSelectedType SelectType) const;

	/*Gets the entire completed quests track.*/
	UFUNCTION(BlueprintCallable, Category = "Quest Display")
		TArray <FCompletedQuestTrack> GetCompletedQuestsTrack() const;

	/*Gets the entire active quests track.*/
	UFUNCTION(BlueprintCallable, Category = "Quest Display")
		TArray<FQuestTrack> GetQuestTracks() const;

	/*Sets the active talktext.*/
	UFUNCTION(BlueprintCallable, Category = "Quest Display")
		bool  SetActiveTalkText(const FTalkTextInfo& TalkTextIn);

	/*Gets the active talktext.*/
	UFUNCTION(BlueprintCallable, Category = "Quest Display")
		FTalkTextInfo GetActiveTalkText() const;

	/*Finds a active talk text index.*/
	UFUNCTION(BlueprintCallable, Category = "Quest Display")
		int FindActiveTalkTextIndex(const TArray<FTalkTextInfo>& AllInfoIn) const;

	/**
	*	Gets the Current/Selected NPC. This is the npc that is currently talking to the player.
	*	This is set on NPC reached.
	*	WARNING - This variable will not be set to null as there is no way to know when you stop talking to a NPC from the quest manager.It will overriden on next NPC reached.
	*/
	UFUNCTION(BlueprintCallable, Category = "Quest Display")
		AActor* GetCurrentNPC() const;

};
