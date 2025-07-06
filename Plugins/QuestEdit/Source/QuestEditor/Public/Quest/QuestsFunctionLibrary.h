//Copyright 2020 Marchetti S. Alfredo I. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UObject/UnrealType.h"
#include "Quest/QuestTypes.h"
#include "QuestsFunctionLibrary.generated.h"

class UQuestManager;
class UQuestSystemSaveGame;
class UBaseQuestRewardSlot;

/**
 * 
 */
UCLASS()
class QUESTEDITOR_API UQuestsFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
		

public:

	//------------------------------------------Quests and dialogs structs
	/*Gets the quest information from a datatable reference*/
	UFUNCTION(BlueprintCallable, Category = "Quest structs")
		static FQuest  QuestRepToQuest(const FQuestRep& QuestRepIN);

	/*Gets the dialog information from a datatable reference*/
	UFUNCTION(BlueprintCallable, Category = "Quest structs")
		static FDialog  DialogRepToDialog(const FDialogRep& DialogRepIN);

	/*Gets the NPC information from a datatable reference*/
	UFUNCTION(BlueprintCallable, Category = "Quest structs")
		static FNPCQuestDef  NPCQuestDefRepToNPCQuestDef(const FNPCQuestDefRep& FNPCQuestDefIn);

	/*Gets DT name and DT folder path and creates a full Path reference.*/
	UFUNCTION(BlueprintCallable, Category = "Quest structs")
		static FString  MakeStringReference(const FString& DTNameIn, const FString& DTPathIn);	


	
	//---------------------------------------------Conversions
	/*Converts a quest rep struct to string*/
	UFUNCTION(BlueprintCallable, Category = "Quest structs")
		static FString  QuestRepToString(const FQuestRep& QuestRepIN);

	/*Converts a Dialog rep struct to string*/
	UFUNCTION(BlueprintCallable, Category = "Quest structs")
		static FString  DialogRepToString(const FDialogRep& DialogRepIN);

	/*Converts a FNPCQuestDefRep struct to string*/
	UFUNCTION(BlueprintCallable, Category = "Quest structs")
		static FString  NPCQuestDefRepToString(const FNPCQuestDefRep& FNPCQuestDefIn);

	/*Converts a Quest task ID struct to string*/
	UFUNCTION(BlueprintCallable, Category = "Quest structs")
		static FString  TaskIDToString(const FQuestTaskID& QuestTaskIDIn);

	/*Converts a Quest node struct to string*/
	UFUNCTION(BlueprintCallable, Category = "Quest structs")
		static FString  QuestNodeToString(const FQuestNode& QuestNodeIn);

	/*Converts a string to quest rep struc*/
	UFUNCTION(BlueprintCallable, Category = "Quest structs")
		static FQuestRep  StringToQuestRep(const FString& StringIn);

	/*Converts a string to Dialog rep struct*/
	UFUNCTION(BlueprintCallable, Category = "Quest structs")
		static FDialogRep StringToDialogRep(const FString& StringIn);

	/*Converts a string to FNPCQuestDefRep struct*/
	UFUNCTION(BlueprintCallable, Category = "Quest structs")
		static FNPCQuestDefRep  StringToNPCQuestDefRep(const FString& StringIn);

	/*Converts a string to TaskID*/
	UFUNCTION(BlueprintCallable, Category = "Quest structs")
		static FQuestTaskID StringToTaskID(const FString& StringIn);

	/*Converts a string to QuestNode*/
	UFUNCTION(BlueprintCallable, Category = "Quest structs")
		static FQuestNode StringToQuestNode(const FString& StringIn);

	/**	
	*	Removes a TaskID from a Links Array and returns the new array.
	*	Does not care of link type.
	*/
	UFUNCTION(BlueprintCallable, Category = "Quest structs")
		static  TArray <FQuestTaskLink> RemoveTaskIDFromLinkArray(const FQuestTaskID& TaskIDIn, const TArray <FQuestTaskLink>& LinksArrayIn);

	/**
	*	Given a link array, returns the node array.
	*	Discards link type.
	*/
	UFUNCTION(BlueprintCallable, Category = "Quest structs")
		static  TArray <FQuestTaskID> ConvertLinkArrayToNodeArray(const TArray <FQuestTaskLink> LinkArrayIn);

	/**
	*	Filter Array by type. Gets a new array with only the nodes that matches the selected type.
	*/
	UFUNCTION(BlueprintCallable, Category = "Quest structs")
		static  TArray <FQuestTaskID> GetNodesOfTypeInArray(EQuestTaskType TaskTypeToMatch, const TArray <FQuestTaskID> NodesArrayIn);
	

	//--------------------------------------Exposed equal-not equal
	UFUNCTION(BlueprintPure, Category = "Quest Operation", Meta = (DisplayName = "Equal Quest Task", CompactNodeTitle = "==", Keywords = "== equal"))
		static bool EqualQuestTaskID(FQuestTaskID A, FQuestTaskID B);
	
	UFUNCTION(BlueprintPure, Category = "Quest Operation", Meta = (DisplayName = "Equal Talk Mode", CompactNodeTitle = "==", Keywords = "== equal"))
		static bool EqualTalkMode(FTalkModeDef A, FTalkModeDef B);

	UFUNCTION(BlueprintPure, Category = "Quest Operation", Meta = (DisplayName = "Equal Talk Text Info", CompactNodeTitle = "==", Keywords = "== equal"))
		static bool EqualTalkTextInfo(FTalkTextInfo A, FTalkTextInfo B);

	UFUNCTION(BlueprintPure, Category = "Quest Operation", Meta = (DisplayName = "Equal QuestRep", CompactNodeTitle = "==", Keywords = "== equal"))
		static bool EqualQuestRep(FQuestRep A, FQuestRep B);

	UFUNCTION(BlueprintPure, Category = "Quest Operation", Meta = (DisplayName = "Equal DialogRep", CompactNodeTitle = "==", Keywords = "== equal"))
		static bool EqualDialogRep(FDialogRep A, FDialogRep B);

	UFUNCTION(BlueprintPure, Category = "Quest Operation", Meta = (DisplayName = "Equal NPCQuestDefRep", CompactNodeTitle = "==", Keywords = "== equal"))
		static bool EqualNPCQuestDefRep(FNPCQuestDefRep A, FNPCQuestDefRep B);

	UFUNCTION(BlueprintPure, Category = "Quest Operation", Meta = (DisplayName = "Equal QuestNode", CompactNodeTitle = "==", Keywords = "== equal"))
		static bool EqualQuestNode(FQuestNode A, FQuestNode B);

	UFUNCTION(BlueprintPure, Category = "Quest Operation", Meta = (DisplayName = "NotEqual Quest Task", CompactNodeTitle = "!=", Keywords = "!= Not equal"))
		static bool NotEqualQuestTaskID(FQuestTaskID A, FQuestTaskID B);

	UFUNCTION(BlueprintPure, Category = "Quest Operation", Meta = (DisplayName = "NotEqual Talk Mode", CompactNodeTitle = "!=", Keywords = "!= Not equal"))
		static bool NotEqualTalkMode(FTalkModeDef A, FTalkModeDef B);

	UFUNCTION(BlueprintPure, Category = "Quest Operation", Meta = (DisplayName = "NotEqual Talk Text Info", CompactNodeTitle = "!=", Keywords = "!= Not equal"))
		static bool NotEqualTalkTextInfo(FTalkTextInfo A, FTalkTextInfo B);

	UFUNCTION(BlueprintPure, Category = "Quest Operation", Meta = (DisplayName = "NotEqual QuestRep", CompactNodeTitle = "!=", Keywords = "!= Not equal"))
		static bool NotEqualQuestRep(FQuestRep A, FQuestRep B);

	UFUNCTION(BlueprintPure, Category = "Quest Operation", Meta = (DisplayName = "NotEqual DialogRep", CompactNodeTitle = "!=", Keywords = "!= Not equal"))
		static bool NotEqualDialogRep(FDialogRep A, FDialogRep B);

	UFUNCTION(BlueprintPure, Category = "Quest Operation", Meta = (DisplayName = "NotEqual NPCQuestDefRep", CompactNodeTitle = "!=", Keywords = "!= Not equal"))
		static bool NotEqualNPCQuestDefRep(FNPCQuestDefRep A, FNPCQuestDefRep B);
	   
	UFUNCTION(BlueprintPure, Category = "Quest Operation", Meta = (DisplayName = "NotEqual QuestNode", CompactNodeTitle = "!=", Keywords = "!= Not equal"))
		static bool NotEqualQuestNode(FQuestNode A, FQuestNode B);


	//--------------------------------------Quest dt
	/*Gets a datatable from a datatable name and folder path.*/
	UFUNCTION(BlueprintCallable, Category = "Quest DT")
		static UDataTable* GetDTFromReference(FString QuestDTName, FString DTFolderPath);

	/*Gets a datatable from a datatable TSoftObjectPtr path.*/
	UFUNCTION(BlueprintCallable, Category = "Quest DT")
		static UDataTable* GetDTFromPath(TSoftObjectPtr <UDataTable> DTPathIN);

	/*Gets a datatable from a datatable string full path.*/
	UFUNCTION(BlueprintCallable, Category = "Quest DT")
		static UDataTable* GetDTFromStringPath(FString DTPathIN);
	
	//--------------------------------------Quest Interface

	/*Gets the current value of the timer. Returns -1 is there is no active timer.*/
	UFUNCTION(BlueprintCallable, Category = "Quest Task Interface")
		static float  GetActiveTaskTimer(int QuestSlotIn, int QuesTaskIn, bool IsDialogIn, UQuestManager* QuestManagerRef);

	/*Gets task ID short description from task object*/
	UFUNCTION(BlueprintCallable, Category = "Quest Task Interface")
		static FString GetTaskShortDescription(const FQuestTask& TaskIn);

	/*Gets task ID name from task object*/
	UFUNCTION(BlueprintCallable, Category = "Quest Task Interface")
		static FString GetTaskName(const FQuestTask& TaskIn);

	/*Gets the task ID color for ui.*/
	UFUNCTION(BlueprintCallable, Category = "Quest Task Interface")
		static FLinearColor GetTaskUIColor(const FQuestTask& TaskIn);

	/*Gets the task announce config for ui.*/
	UFUNCTION(BlueprintCallable, Category = "Quest Task Interface")
		static bool GetTaskShouldAnnounce(const FQuestTask& TaskIn);

	/*Gets the task UI visib config for ui.*/
	UFUNCTION(BlueprintCallable, Category = "Quest Task Interface")
		static bool GetTaskShouldShowInUI(const FQuestTask& TaskIn);

	/*Gets the task node UI visib config for ui.*/
	UFUNCTION(BlueprintCallable, Category = "Quest Task Interface")
		static bool GetNodeShouldShowInUI(EQuestSelectedType SelectType, int QuestSlotIn, FQuestTaskID NodeIn, bool IsDialogIn, UQuestManager* QuestManagerRef);

	/**
	*	Determines if the task should show only once in summary even if there are many of it active at the same time.
	*	Used in choose reward.
	*	The string ID helps identify the task type. Should be unique per each task. For Example: ItemReward or SpellReward.
	*/
	UFUNCTION(BlueprintCallable, Category = "Quest Task Interface")
		static bool GetTaskShowsOnlyOnceInSummary(const FQuestTask& TaskIn, FString& StringIDOut);

	/**
	*	Determines if the task has a reward giver and if found, returns the definition.
	*	By default the reward tasks use stringA of payload for the NPCDef.
	*/
	UFUNCTION(BlueprintCallable, Category = "Quest Task Interface")
		static bool GetTaskRewardGiver(const FQuestTask& TaskIn, FNPCQuestDefRep& RewardGiverOut);

	/**
	*	Determines if the task has a reward location and if found, returns the location name.
	*	By default the reward tasks use stringA of payload for the Location.
	*/
	UFUNCTION(BlueprintCallable, Category = "Quest Task Interface")
		static bool GetTaskRewardLocation(const FQuestTask& TaskIn, FString& LocationNameOut);

	/**
	*	Determines the task reward slot used in UI.
	*/
	UFUNCTION(BlueprintCallable, Category = "Quest Task Interface")
		static 	TSubclassOf<UBaseQuestRewardSlot> GetTaskRewardSlot(const FQuestTask& TaskIn);

	/**
	*	Determines the task reward additional tags.
	*/
	UFUNCTION(BlueprintCallable, Category = "Quest Task Interface")
		static 	FGameplayTagContainer GetTaskRewardTags(const FQuestTask& TaskIn);
	


	/*Convert structure to Payload Data String.*/
	UFUNCTION(BlueprintCallable, Category = "Quest Custom Payload", CustomThunk, meta = (CustomStructureParam = "StructIn"))
		static FString StructToPayloadData(TFieldPath<FProperty> StructIn);
	DECLARE_FUNCTION(execStructToPayloadData)
	{
		//Grab struct				
		Stack.MostRecentProperty = nullptr;
		Stack.StepCompiledIn<FStructProperty>(NULL);
		void* StructAddr = Stack.MostRecentPropertyAddress;
		FStructProperty* StructProperty = CastField<FStructProperty>(Stack.MostRecentProperty);
	
		//Get scripstruct
		UScriptStruct* MyScriptStruct = StructProperty->Struct;

		//Convert to string
		FString StringToStore;
		MyScriptStruct->ExportText(StringToStore, StructAddr, nullptr, nullptr, (PPF_ExportsNotFullyQualified | PPF_Copy | PPF_Delimited | PPF_IncludeTransient), nullptr);
		
		P_FINISH;
		P_NATIVE_BEGIN;
		*(FString*)RESULT_PARAM = StringToStore;
		P_NATIVE_END;
	}

	/** 
	*	Fills a structure with a Payload Data String
	*	Make sure the StructIn variable is cleared before, to avoid carrying data over, specially in loops.
	*/
	UFUNCTION(BlueprintCallable, Category = "Quest Custom Payload", CustomThunk, meta = (CustomStructureParam = "StructIn"))
		static void PayloadDataToStruct(TFieldPath<FProperty> StructIn, const FString& PayloadDataIn);
	DECLARE_FUNCTION(execPayloadDataToStruct)
	{

		// Grab Struct In
		Stack.MostRecentProperty = nullptr;
		Stack.StepCompiledIn<FStructProperty>(NULL);
		void* StructAddr = Stack.MostRecentPropertyAddress;
		FStructProperty* StructProperty = CastField<FStructProperty>(Stack.MostRecentProperty);
		UScriptStruct* MyScriptStruct = StructProperty->Struct;
		
		//Grab String in
		P_GET_PROPERTY(FStrProperty, PayloadDataIn);
		
		P_FINISH;
		if (PayloadDataIn == FString()) return;
				
		P_NATIVE_BEGIN;
		MyScriptStruct->ImportText(*PayloadDataIn, StructAddr, nullptr, (PPF_ExportsNotFullyQualified | PPF_Copy | PPF_Delimited | PPF_IncludeTransient), nullptr, "CustomStructure", false);
		P_NATIVE_END;	
	}

	//---------------------------------Tasks (not exposed)

	/**
	*	Checks if a task is ActivateConditionOnly.
	*	Calling this function does not require object spawn.
	*	(not exposed to bp)
	*/
	UFUNCTION()
		static bool TaskIsActivateConditionOnly(const FQuest& QuestIn, int TaskSlot);

	/**
	*	Finds all talktasks of a conversation that includes the task sample. The sample task can be any task of the conversation.
	*/
	UFUNCTION(BlueprintCallable, Category = "Quest")
		static TArray<FQuestTaskID> GetAllTalkTasksOfConversation(const FQuest& QuestIn, int TalkTaskSampleIn, int& ConversationIDOut);

	/**
	*	Gets all nodes and their conversation ID of a quest
	*	@Param ReelevantNodesIn Nodes that matters, if they are all found the operation can be cancelled to save time.
	*/
	UFUNCTION(BlueprintCallable, Category = "Quest")
		static TMap <FQuestTaskID, int> GetQuestConversationsMap(const FQuest& QuestIn);

	/*Determines if a Task ID is for the Quest struct provided. */
	UFUNCTION(BlueprintCallable, Category = "Quest")
		static bool IsTaskIDValidInQuest(const FQuest& QuestIn, const FQuestTaskID& TaskIDIn);

	/*Gets the inferior nodes of a particular node for a quest*/
	UFUNCTION()
		static TArray<FQuestTaskID> GetNodeInferiorNodesInQuest(const FQuest& QuestIn, const FQuestTaskID& NodeIn);

	/*Gets the superior nodes of a particular node for a quest*/
	UFUNCTION()
		static TArray<FQuestTaskID> GetNodeSuperiorNodesInQuest(const FQuest& QuestIn, const FQuestTaskID& NodeIn);

	/*Gets the Disabled nodes of a particular node for a quest*/
	UFUNCTION()
		static TArray<FQuestTaskID> GetNodeDisabledNodesInQuest(const FQuest& QuestIn, const FQuestTaskID& NodeIn);

	/*Gets the inferior Links of a particular node for a quest*/
	UFUNCTION()
		static TArray<FQuestTaskLink> GetNodeInferiorLinksInQuest(const FQuest& QuestIn, const FQuestTaskID& NodeIn);

	/*Gets the superior Links of a particular node for a quest*/
	UFUNCTION()
		static TArray<FQuestTaskLink> GetNodeSuperiorLinksInQuest(const FQuest& QuestIn, const FQuestTaskID& NodeIn);

	/*Determines if all superior nodes of a task are conditionals nodes. */
	UFUNCTION()
		static bool AreAllSuperiorNodesOfNodeConditions(const FQuest& QuestIn, const FQuestTaskID& NodeIn);

	/*Determines if all REQUIRED superior nodes of a task are conditionals nodes, optional nodes are not considered. */
	UFUNCTION()
		static bool AreAllRequiredSuperiorNodesOfNodeConditions(const FQuest& QuestIn, const FQuestTaskID& NodeIn);

	/**
	*	If the node has no superior nodes is start node.
	*	If all superior nodes of a node are conditionals nodes and also start nodes, the node is start node (Pseudo). 
	*/
	UFUNCTION()
		static bool IsNodeStartNode(const FQuest& QuestIn, const FQuestTaskID& NodeIn);

	//--------------------------------------Quest Framework

	/*Gets all save game names for quests in the game save folder.*/
	UFUNCTION(BlueprintCallable, Category = "Quest Framework")
		static TArray <FString> GetAllQuestSaveGameNames();

	/*Gets all save game objects for quests in the game save folder.*/
	UFUNCTION(BlueprintCallable, Category = "Quest Framework")
		static TArray <UQuestSystemSaveGame*> GetAllQuestSaveGameObjects();

	/**
	*	Destroys a save game object of the Quest System of a player.
	*	This action is permanent.
	*	@Param SaveNameIn Player Save Name
	*/
	UFUNCTION(BlueprintCallable, Category = "Quest Framework")
		static bool DeleteQuestSaveGame(const FString& SaveNameIn);

	/**
	*	Destroys all save games objects of the Quest System for players.
	*	This action is permanent.
	*/
	UFUNCTION(BlueprintCallable, Category = "Quest Framework")
		static void DeleteAllQuestSaveGames();





	//--------------------------------------Single sentence mode
	/**
	*	Gets all talk texts of the array with the especified filters.
	*	Used in Single sentence mode.
	*	@Param ActiveTalkTextsIn Current active talk texts for this NPC. Provided as input to avoid extra calls.
	*	@Param SSDataIn Correlated array for talk texts data. Provided as input to avoid extra calls.
	* 	@Param AllowedMode The talk text mode allowed, can be quest or dialog.
	*	@Param AllowedStates All the states that the talktexts can have. If no state is specified means that this filter will not apply or that any is valid.
	*	@Param AllowedTypes All the types that the talktexts can have. If no state is specified means that this filter will not apply or that any is valid.
	*	@Param AllowedSSTypes All the Single sentence types that the talktexts can have. If no state is specified means that this filter will not apply or that any is valid.
	*/

	UFUNCTION(BlueprintCallable, Category = "Single Sentence Mode")
		static TArray <FTalkTextInfo> FilterTalkTextsForSingleSentence(const TArray <FTalkTextInfo>& ActiveTalkTextsIn, const TArray <FSingleSentenceModeData>& SSDataIn, const ETalkTextMode& AllowedMode, const TArray<EQuestTaskState>& AllowedStates, const TArray<ETalkTextType>& AllowedTypes, const TArray<EDialogType>& AllowedSSTypes, TArray <FSingleSentenceModeData>& SSDataOut);

	/**
	*	Given an array of talk texts SSdata determines if All matches the type.
	*	@Param SSDataIn This is the custom payload data of all inferior nodes of talk text.
	*/

	UFUNCTION(BlueprintCallable, Category = "Single Sentence Mode")
		static bool AreAllTalkTextsOfType(const TArray <FSingleSentenceModeData>& SSDataIn, EDialogType AllowedSSType);




};
