//Copyright 2020 Marchetti S. Alfredo I. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Quest/QuestTypes.h"
#include "QuestEditor/QuestTypesDev.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "QuestEditorDevFunctionLibrary.generated.h"

class UBaseQuestEditor;
class UBaseQuestTask;
class UQuestEditorSettings;

/**
 * 
 */
UCLASS()
class QUESTEDITORDEV_API UQuestEditorDevFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	//----------------------------------------------------------------------------------------
	//-----------------------------------------Settings
	//----------------------------------------------------------------------------------------

	/** Direct access to the plugin settings */
	UFUNCTION()
		static UQuestEditorSettings* GetQuestEditorSettings();

	/** TasksBackgroundColor getter from settings */
	UFUNCTION(BlueprintCallable, Category = "Quest Settings")
		static FLinearColor GetSettings_TaskBGColor();

	/** TasksBackgroundColor getter from settings */
	UFUNCTION(BlueprintCallable, Category = "Quest Settings")
		static FLinearColor GetSettings_TalkTaskBGColor();

	/** Get the detail panel width size in the settings. */
	UFUNCTION(BlueprintCallable, Category = "Quest Settings")
		static FVector2D GetSettings_DetailsPanelSize();

	/** Get arrow sensibility settings */
	UFUNCTION(BlueprintCallable, Category = "Quest Settings")
		static float GetSettings_ArrowsSensitivity();

	/** Get zoom sensibility settings */
	UFUNCTION(BlueprintCallable, Category = "Quest Settings")
		static float GetSettings_ZoomSensitivity();

	/** Get editor previous version from settings */
	UFUNCTION()
		static int GetSettings_PreviousVersion();

	/** Get editor arrow size from settings */
	UFUNCTION(BlueprintCallable, Category = "Quest Settings")
		static float GetSettings_ArrowSize();

	/** Get editor line thickness from settings */
	UFUNCTION(BlueprintCallable, Category = "Quest Settings")
		static float GetSettings_LineThickness();


	//----------------------------------------------------------------------------------------
	//-----------------------------------------Keybinds
	//----------------------------------------------------------------------------------------
	
	/**
	*	Gets the input associated to the action by default for the quest editor
	*/
	UFUNCTION(BlueprintCallable, Category = "Quest Editor Key Binds")
		static FInputChord GetDefaultKeybindForAction(EQuestKeyBindAction ActionIn);

	/**
	*	Get the default keybinds array for the quest editor.
	*/
	UFUNCTION(BlueprintCallable, Category = "Quest Editor Key Binds")
		static TArray <FQuestEditorKeyBind> GetDefaultQuestEditorKeybinds();
		
	/**
	*	Given the key, returns the action assigned to the key.
	*	If the key is not found in the overrides, it will be searchead in the default keys.
	*	Returns true if the key is found. If not there is not action assigned to this key.
	*/
	UFUNCTION(BlueprintCallable, Category = "Quest Editor Key Binds")
		static bool GetActionForKey(FInputChord InputIn, EQuestKeyBindAction& ActionOut);
	
	/**
	*	Determines if the action can be performed in KB with the current modes.
	*/
	UFUNCTION(BlueprintCallable, Category = "Quest Editor Key Binds")
		static bool CanPerformKBAction(EQuestKeyBindAction ActionIn, bool IsHierarchyIn);

	//----------------------------------------------------------------------------------------
	//------------------------------------------General functions
	//----------------------------------------------------------------------------------------
	/*
	*	Gets all NPCs names that can start a quest via talking.
	*/
	UFUNCTION(BlueprintCallable, Category = "Quest")
		static TArray <FString>  GetNPCsThatCanStartQuest(const FQuestRep& QuestRepIn);

	/**
	*	Gets First Quest editor reference found.
	*	This will find all quest editor references in memory and provide the first one.
	*	Avoid using this a lot. Try to call on begin play and save the reference once found for future use.
	*/
	UFUNCTION(meta = (WorldContext = "WorldContextObject"),BlueprintCallable, Category = "QuestEditor")
		static UBaseQuestEditor* FindQuestEditorReference(UObject* WorldContextObject);

	/**
	*	Finds datatables of a particular struct in the specified folder.
	*	Can be heavy on performance, specially if folder is too big. 
	*	Avoid calling this function.
	*/
	UFUNCTION(BlueprintCallable, Category = "QuestEditor-Datatables")
		static TArray<UDataTable*> FindAllDatatablesOfStruct(const UScriptStruct* DTStructIn, FString FolderPath = "/Game");

	/**
	*	Gets quest editor info of a task base on the task reference. 
	*	Datatables array (for task info) must be provided to avoid extra calls to get datatable functions, for better performance.
	*	If class invalid or not found in DTs, Slots will return -1 and return value is false.
	*/
	UFUNCTION(BlueprintCallable, Category = "QuestEditor")
		static bool GetTaskQuestEditorInfo(TSoftClassPtr<UBaseQuestTask> TaskClassIn, TArray <UDataTable*> TaskInfoDTsIn, int& SlotTaskOut, int& SlotDTOut, FQuestTaskEditorInfo& TaskInfoOut);

	/*Determines if a DT is of struct type*/
	UFUNCTION(BlueprintCallable, Category = "QuestEditor-Datatables")
		static bool IsDTofStructType(UDataTable* DT, const UScriptStruct* DTStructIn);

	/*Prints in the Quest Dev Log. Only prints Log messages */
	UFUNCTION(BlueprintCallable, Category = "QuestEditor-Datatables")
		static void PrintQuestEditorDevLog(const FString& StringIn);

	/**
	*	Gets the quest information from a datatable reference.
	*	This is expensive since it will iterate through all DTs untill finds one with same name.
	*	UNRELIABLE - If many quests share same name, this will not work properly.
	*/
	UFUNCTION(BlueprintCallable, Category = "Quest structs")
		static FQuestRep  FindQuestInDatatables(const FString& QuestNameIN, TArray <UDataTable*> DTArrayIn);

	/**
	*	Gets the quest information from a datatable reference.
	*	This is expensive since it will iterate through all DTs untill finds one with same name.
	*	UNRELIABLE - If many dialogs share same name, this will not work properly.
	*/
	UFUNCTION(BlueprintCallable, Category = "Quest structs")
		static FDialogRep  FindDialogInDatatables(const FString& DialogNameIN, TArray <UDataTable*> DTArrayIn);

	/**
	*	Gets the NPC information from a datatable reference.
	*	This is expensive since it will iterate through all DTs untill finds one with same name.
	*	UNRELIABLE - If many NPCs share same name, this will not work properly.
	*/
	UFUNCTION(BlueprintCallable, Category = "Quest structs")
		static FNPCQuestDefRep  FindNPCInDatatables(const FString& NPCNameIN, TArray <UDataTable*> DTArrayIn);

	/**
	*	Determines if the quest is saved in a datatable by name.
	*/
	UFUNCTION(BlueprintCallable, Category = "Quest structs")
		static bool  IsQuestInDatatables(const FString& QuestNameIn, TArray <UDataTable*> DTArrayIn);


	//----------------------------------------------------------------------------------------
	//--------------------------------------------EDITOR UI
	//----------------------------------------------------------------------------------------

	/*Determine if the widget is fully visible.*/
	UFUNCTION(BlueprintCallable, Category = "Widget Visiblity")
	static bool IsWidgetFullyVisible(UWidget* WidgetIn);

	/*Gets a quest end name for identifying in editor.*/
	UFUNCTION(BlueprintCallable, Category = "QuestEditor")
		static FString GetQuestNodeVisualString(FQuestNode QuestNodeIn, bool UseNPCDialog);

	/*Gets a quest end name for identifying in editor.*/
	UFUNCTION(BlueprintCallable, Category = "QuestEditor")
		static FString GetTaskIDVisualString(const FQuest& QuestIn, const FQuestTaskID& TaskIDIn, bool UseNPCDialog);


	/*Gets a quest end name variables for identifying in editor.*/
	UFUNCTION(BlueprintCallable, Category = "QuestEditor")
		static void GetQuestNodeSeparatedStrings(FQuestNode QuestEndIn, bool UseNPCDialog, FString& QuestStringOut, FString& TaskStringOut, FString& TaskTypeStringOut);

	/*Gets a quest node ID name variables for identifying in editor.*/
	UFUNCTION(BlueprintCallable, Category = "QuestEditor")
		static void GetTaskIDSeparatedStrings(const FQuest& QuestIn, const FQuestTaskID& TaskIDIn, bool UseNPCDialog, FString& QuestStringOut, FString& TaskStringOut, FString& TaskTypeStringOut);

	/**
	*	If the string exceds the len, returns all characters possibles untill reaching that len
	*	If the string is truncated, "..." is added at the end.
	*/
	UFUNCTION(BlueprintCallable, Category = "QuestEditor")
		static FString TruncateStringToLen(FString StringIn, int MaxLen);

	/**
	*	Given a tasks array, separates by type.
	*/
	UFUNCTION(BlueprintCallable, Category = "QuestEditor")
		static void SeparateTasksByType(const TArray <FQuestTaskID>& AllTasksIn, TArray <FQuestTaskID>&TasksOut, TArray <FQuestTaskID>& TalkTasksOut);
	
	/**
	*	Get the selection type based on the selected nodes
	*/
	UFUNCTION(BlueprintCallable, Category = "QuestEditor")
		static ENodeSelectionType GetSelectionType(const TArray <FQuestTaskID>& AllTasksIn, const TArray <FQuestTaskID>& AllTalkTasksIn);

	/**
	*	Get a task that represents all the tasks.
	*	Used for multinode UI.
	*/
	UFUNCTION(BlueprintCallable, Category = "QuestEditor")
		static FQuestTask GetRepresentingTask(const TArray <FQuestTask>& AllTasksIn);

	/**
	*	Get a talktask that represents all the talktasks.
	*	Used for multinode UI.
	*/
	UFUNCTION(BlueprintCallable, Category = "QuestEditor")
		static FQuestTalkTask GetRepresentingTalkTask(const TArray <FQuestTalkTask>& AllTalkTasksIn);

	/**
	*	Get talktasks from quests based on ID arrays.
	*/
	UFUNCTION(BlueprintCallable, Category = "QuestEditor")
		static TArray <FQuestTalkTask> GetTalkTasksFromIDArray(const FQuest& QuestIn, const TArray <FQuestTaskID>& TalkTasksIn);

	/**
	*	Get tasks from quests based on ID arrays.
	*/
	UFUNCTION(BlueprintCallable, Category = "QuestEditor")
		static TArray <FQuestTask> GetTasksFromIDArray(const FQuest& QuestIn, const TArray <FQuestTaskID>& TasksIn);

	//----------------------------------------------------------------------------------------
	//-----------------------------------------------Datatables
	//----------------------------------------------------------------------------------------
	/*Saves all dialogs to a quest datatable.*/
	UFUNCTION(BlueprintCallable, Category = "QuestEditor")
		static bool ConvertDialogsToQuest(TSoftObjectPtr <UDataTable> DialogDatatableIn, TSoftObjectPtr <UDataTable> QuestDatatableIn);

	/*Saves the quest to a DT. If it already existes overwrittes it. If not the quest is added*/
	UFUNCTION(BlueprintCallable, Category = "QuestEditor")
		static bool SaveQuestRowToDT(const FQuest& QuestIn, TSoftObjectPtr <UDataTable> DatatableIn);

	/*Saves the dialog to a DT from a quest struct. If it already existes overwrittes it. If not the quest is added*/
	UFUNCTION(BlueprintCallable, Category = "QuestEditor")
		static bool SaveDialogRowToDT(const FQuest& QuestIn, TSoftObjectPtr <UDataTable> DatatableIn);

	/*Saves the NPCInfo to a DT. If it already existes overwrittes it. If not the npc is added*/
	UFUNCTION(BlueprintCallable, Category = "QuestEditor")
		static bool SaveNPCRowToDT(const FNPCQuestDef& NPCInfoIn, TSoftObjectPtr <UDataTable> DatatableIn);

	/*Adds the quest to the npc DT info inventory*/
	UFUNCTION(BlueprintCallable, Category = "QuestEditor")
		static bool AddQuestToNPC(const FQuestRep& QuestRepIn, FName NPCNameIn, TSoftObjectPtr <UDataTable> DatatableIn);

	/*Adds the dialog to the npc DT info inventory.*/
	UFUNCTION(BlueprintCallable, Category = "QuestEditor")
		static bool AddDialogsToNPC(const FQuestRep& QuestRepIn, FName NPCNameIn, TSoftObjectPtr <UDataTable> DatatableIn);

private:
	/*Adds New NPC info row*/
	UFUNCTION(BlueprintCallable, Category = "QuestEditor")
		static bool AddNPCInfoRowToDT(const FNPCQuestDef& NPCInfoIn, FName InRowName, bool UseAutoRowName, TSoftObjectPtr <UDataTable> DatatableIn);

	/*Adds a new quest row to a quest DT*/
	UFUNCTION()
		static bool AddQuestRowToDT(const FQuest& QuestIn, FName InRowName, TSoftObjectPtr <UDataTable> DatatableIn, bool UseAutoRowName = true);

	/*Adds a new dialog row to a dialog DT*/
	UFUNCTION()
		static bool AddDialogRowToDT(const FQuest& QuestIn, FName InRowName, TSoftObjectPtr <UDataTable> DatatableIn, bool UseAutoRowName = true);

	//-----------------------------------------------------------------------------------------
	//---------------------------------------------Localization
	//-----------------------------------------------------------------------------------------

	/*Handles the quest Unique ID before adding to datatable.*/
	UFUNCTION()
		static FQuest HandleQuestUniqueID(const FQuest& QuestIn, TSoftObjectPtr <UDataTable> DatatableIn);

	/*Handles the dialog Unique ID before adding to datatable. Dialog is in quest form.*/
	UFUNCTION()
		static FQuest HandleDialogUniqueID(const FQuest& QuestIn, TSoftObjectPtr <UDataTable> DatatableIn);

	/*Generates localization keys for the quest*/
	UFUNCTION()
		static FQuest GenerateQuestLocalizationKeys(const FQuest& QuestIn, TSoftObjectPtr <UDataTable> DatatableIn);

	/*Generates localization keys for the dialog*/
	UFUNCTION()
		static FDialog GenerateDialogLocalizationKeys(const FDialog& DialogIn,TSoftObjectPtr <UDataTable> DatatableIn);

	/*Gets the talktasks or task max unique ID*/
	UFUNCTION()
		static int GetTaskMaxUniqueID(const FQuest& QuestIn, EQuestTaskType TaskType);

	/*Get max Quest Unique ID in Datatable*/
	UFUNCTION()
		static int GetQuestMaxUniqueIDInDT(TSoftObjectPtr <UDataTable> DatatableIn);

	/*Get max dialog Unique ID in Datatable*/
	UFUNCTION()
		static int GetDialogMaxUniqueIDInDT(TSoftObjectPtr <UDataTable> DatatableIn);

	/**
	*	If the UniqueID is repetead more than once in the datatable is considered invalid
	*	This can happend if the user modifies datatables manually instead of using the Quest Editor.
	*/
	UFUNCTION()
		static bool IsQuestUniqueIDValid(TSoftObjectPtr <UDataTable> DatatableIn, const FQuest& QuestIn);

	/**
	*	If the UniqueID is repetead more than once in the datatable is considered invalid
	*	This can happend if the user modifies datatables manually instead of using the Quest Editor.
	*/
	UFUNCTION()
		static bool IsDialogUniqueIDValid(TSoftObjectPtr <UDataTable> DatatableIn, const FQuest& QuestIn);

	/*Gets the quest and sets all unique IDs that are not set in tasks.*/
	UFUNCTION(BlueprintCallable, Category = "QuestEditor-IDs")
		static void SetAllUniqueIDsInQuest(UPARAM(ref)FQuest& QuestIn);

	/*Determines if there is at least a task that needs a new ID*/
	UFUNCTION()
		static bool AreQuestTasksMissingIDs(const FQuest& QuestIn);

	//-----------------------------------------------------------------------------------------
	//---------------------------------------------Nodes
	//-----------------------------------------------------------------------------------------
public:
	/*Creates a disable node conection between tasks*/
	UFUNCTION(BlueprintCallable, Category = "QuestEditor-Node")
		static bool CreateDisableLinkBetweenNodes(UPARAM(ref)FQuest& QuestIn, FQuestTaskID DisablerNodeIn, FQuestTaskID DisabledNodeIn);

	/*Destroys a existing disable node conection between tasks*/
	UFUNCTION(BlueprintCallable, Category = "QuestEditor-Node")
		static bool DestroyDisableLinkBetweenNodes(UPARAM(ref)FQuest& QuestIn, FQuestTaskID DisablerNodeIn, FQuestTaskID DisabledNodeIn);

	/*Get all disabled nodes of a particular node*/
	UFUNCTION(BlueprintCallable, Category = "QuestEditor-Node")
		static TArray <FQuestTaskID> GetDisabledNodesOfNode(const FQuest& QuestIn, FQuestTaskID Node);
private:
	/*Gets all nodes that disable this one*/
	UFUNCTION(BlueprintCallable, Category = "QuestEditor-Node")
		static TArray <FQuestTaskID> GetDisablerNodesOfNode(const FQuest& QuestIn, FQuestTaskID Node);

	/*Remove a task Id from quest.*/
	UFUNCTION(BlueprintCallable, Category = "QuestEditor-Node")
		static bool RemoveTaskByID(UPARAM(ref)FQuest& QuestIn, FQuestTaskID TaskID);

	/*Removes many tasks Id from quest.*/
	UFUNCTION(BlueprintCallable, Category = "QuestEditor-Node")
		static bool RemoveMultipleTasksByID(UPARAM(ref)FQuest& QuestIn, const TArray <FQuestTaskID> TaskIDs, bool UpdateConversationIDs = false);

	/*Sorts a array of IDS in decreasing fashion.*/
	UFUNCTION(BlueprintCallable, Category = "QuestEditor-Node")
		static TArray <FQuestTaskID> SortIDArrayDecreasing(const TArray <FQuestTaskID> TaskIDs);

	/**
	*	Clear all connections nodes.
	*	WARNING - Only for testing and debugging.
	*/
	UFUNCTION(BlueprintCallable, Category = "QuestEditor-Node")
		static bool ClearAllConnections(UPARAM(ref)FQuest& QuestIn);

public:
	/*Add a task of type to quest*/
	UFUNCTION(BlueprintCallable, Category = "QuestEditor-Node")
		static int AddTaskOfType(UPARAM(ref)FQuest& QuestIn, EQuestTaskType TaskType);

	/*Breaks a node conection between tasks*/
	UFUNCTION(BlueprintCallable, Category = "QuestEditor-Node")
		static bool BreakNodesConnection(UPARAM(ref)FQuest& QuestIn, FQuestTaskID SuperiorNode, FQuestTaskID InferiorNode, bool UpdateConversationIDs = false);

	/**	
	*	Breaks all node conections of a tasks
	*	Does not remove disable connections
	*/
	UFUNCTION(BlueprintCallable, Category = "QuestEditor-Node")
		static bool BreakAllConnectionOfNode(UPARAM(ref)FQuest& QuestIn, FQuestTaskID Node);

	/**
	*	Breaks all node conections of a tasks in a particular connector.
	*	Returns all modified nodes.
	*/
	UFUNCTION(BlueprintCallable, Category = "QuestEditor-Node")
		static TArray <FQuestTaskID> BreakAllConnectionOfNodeToConnector(UPARAM(ref)FQuest& QuestIn, FQuestTaskID NodeIn, ENodeConnectorType ConnectorIn);

	/*Creates a node conection between tasks*/
	UFUNCTION(BlueprintCallable, Category = "QuestEditor-Node")
		static bool CreateNewLinkBetweenNodes(UPARAM(ref)FQuest& QuestIn, FQuestTaskID SuperiorNode, FQuestTaskID InferiorNode, ENodeLinkType LinkType = ENodeLinkType::Required, bool UpdateConversationIDs = false);

	/*Determines if a node conection can be made.*/
	UFUNCTION(BlueprintCallable, Category = "QuestEditor-Node")
		static bool CanLinkNodes(FQuest QuestIn, FQuestTaskID SuperiorNode, FQuestTaskID InferiorNode);

	/*Determines if a disable node conection can be made.*/
	UFUNCTION(BlueprintCallable, Category = "QuestEditor-Node")
		static bool CanLinkWithDisableNode(FQuest QuestIn, FQuestTaskID DisablerNode, FQuestTaskID DisabledNode);

private:
	/*Determines if a node conection already exist between two nodes.*/
	UFUNCTION(BlueprintCallable, Category = "QuestEditor-Node")
		static bool AreNodesLinked(FQuest QuestIn, FQuestTaskID SuperiorNode, FQuestTaskID InferiorNode);
public:
	/**
	*	Changes the link type of two nodes.
	*	Because there are two link types, it toggles them. No need to specify new value.
	*/
	UFUNCTION(BlueprintCallable, Category = "QuestEditor-Node")
		static bool ChangeLinkTypeBetweenNodes(UPARAM(ref)FQuest& QuestIn, FQuestTaskID SuperiorNode, FQuestTaskID InferiorNode);

private:
	/*Finds a node in a array*/
	UFUNCTION(BlueprintCallable, Category = "QuestEditor-Node")
		static int FindNodeIDInNodeArray(TArray <FQuestTaskID> NodeIDArray, FQuestTaskID Node);

	/**
	*	Returns all links of a node with superior and inferior nodes.
	*	Does not return disabled nodes links.
	*	Returns pairs of nodes. Start and ends of node link.
	*/
	UFUNCTION(BlueprintCallable, Category = "QuestEditor-Node")
		static bool GetNodeConnections(const FQuest& QuestIn, FQuestTaskID Node, TArray <FQuestTaskID>& SuperiorNodesOut, TArray <FQuestTaskID>& InferiorNodesOut);

	/**
	*	Returns all links of a node with disabled nodes and disabler nodes.
	*	Returns pairs of nodes. Start and ends of node link.
	*/
	UFUNCTION(BlueprintCallable, Category = "QuestEditor-Node")
		static bool GetNodeDisableConnections(const FQuest& QuestIn, FQuestTaskID Node, TArray <FQuestTaskID>& DisablerNodesOut, TArray <FQuestTaskID>& DisabledNodesOut);

	/**
	*	Returns all links of a quest for all superior and inferior nodes.
	*	Returns pairs of nodes. Start and ends of node link.
	*/
	UFUNCTION(BlueprintCallable, Category = "QuestEditor-Node")
		static bool GetAllNodesConnections(const FQuest& QuestIn, TArray <FQuestTaskID>& SuperiorNodesOut, TArray <FQuestTaskID>& InferiorNodesOut, TArray<ENodeLinkType>& LinkTypesOut);
public:
	/**
	*	Returns all connections between the selected nodes.
	*	Returns pairs of nodes. Start and ends of node link.
	*/
	UFUNCTION(BlueprintCallable, Category = "QuestEditor-Node")
		static bool GetAllSelectedNodesConnections(const FQuest& QuestIn, const TArray<FQuestTaskID>& SelectedNodes, TArray <FQuestTaskID>& SuperiorNodesOut, TArray <FQuestTaskID>& InferiorNodesOut, TArray<ENodeLinkType>& LinkTypesOut);
private:
	/**
	*	Returns all disabled links for a quest
	*	Returns pairs of nodes. Start and ends of node link.
	*/
	UFUNCTION(BlueprintCallable, Category = "QuestEditor-Node")
		static bool GetAllNodesDisablesConnections(const FQuest& QuestIn, TArray <FQuestTaskID>& DisablerNodesOut, TArray <FQuestTaskID>& DisabledNodesOut);

public:
	/**
	*	Determines if a node can start a swap connection
	*	Returns pairs of nodes. Start and ends of node link.
	*/
	UFUNCTION(BlueprintCallable, Category = "QuestEditor-Node")
		static bool CanNodeStartSwapConnection(const FQuest& QuestIn, FQuestTaskID NodeIn, ENodeConnectorType ConnectorIn);
	
	/**
	*	Swaps all connections of NodeA with NodeB.
	*	Returns a array of messages to be printed for destroyed connections
	*/
	UFUNCTION(BlueprintCallable, Category = "QuestEditor-Node")
		static TArray <FString> SwapNodeConnections(UPARAM(ref)FQuest& QuestIn, const FQuestTaskID& NodeA, const FQuestTaskID& NodeB, ENodeConnectorType ConnectorIn, bool UpdateConversationIDs = false);

	/**
	*	Updates conversation IDs in talk tasks for a quest.
	*/
	UFUNCTION(BlueprintCallable, Category = "QuestEditor-Node")
		static void  UpdateAllConversationsIDs(UPARAM(ref)FQuest& QuestIn);

	//-----------------------------------------------------------------------------------------
	//---------------------------------------------Nodes alignment and editor functionality
	//-----------------------------------------------------------------------------------------
	
	/**
	*	Gets the position of the node referenced to its center and not to its top left has they usually are.
	*/
	UFUNCTION(BlueprintCallable, Category = "QuestEditor-NodeAlignment")
		static FVector2D GetNodeCenterPosition(const FQuest& QuestIn, const FQuestTaskID SelectedNodeIn, const FVector2D& NodeSizeIn);

	/**
	*	Gets a node center position and converts it to a Quest editor position (points to top left of node)
	*/
	UFUNCTION(BlueprintCallable, Category = "QuestEditor-NodeAlignment")
		static FVector2D ConvertCenterPositionToQEPosition(const FVector2D& NodeSizeIn, const FVector2D& NodeCenterPositionIn);

	/**
	*	Gets the lowest or highest X or Y value of all nodes positions calculated from center of widget
	*/
	UFUNCTION(BlueprintCallable, Category = "QuestEditor-NodeAlignment")
		static int GetPositionValue(const FQuest& QuestIn, const TArray <FQuestTaskID>& SelectedNodesIn, const TArray <FVector2D>& NodesWidgetSizeIn, bool UseY , bool CheckLower, FQuestTaskID& NodeOut);

	/**
	*	Gets the node quest editor position in quest
	*/
	UFUNCTION(BlueprintCallable, Category = "QuestEditor-NodeAlignment")
		static FVector2D GetNodePosition(const FQuest& QuestIn, const FQuestTaskID& NodeIn);

	/**
	*	Gets all nodes that dont have superior nodes
	*/
	UFUNCTION(BlueprintCallable, Category = "QuestEditor-NodeAlignment")
		static TArray <FQuestTaskID> GetQuestStartNodes(const FQuest& QuestIn);
	
	/**
	*	Gets all nodes that dont have inferior nodes.
	*/
	UFUNCTION(BlueprintCallable, Category = "QuestEditor-NodeAlignment")
		static TArray <FQuestTaskID> GetQuestEndNodes(const FQuest& QuestIn);

	/**
	*	Sorts a array of IDS in decreasing fashion.
	*	Can use X or Y component.
	*/
	UFUNCTION(BlueprintCallable, Category = "QuestEditor-Node")
		static TArray <FQuestTaskID> SortNodesByEditorPositiong(const FQuest& QuestIn, const TArray <FQuestTaskID>& Nodes, bool UseY, bool UseReverseOrder);

	/**
	*	Compares 2 arrays of nodes and determines if they contain the same nodes.
	*/
	UFUNCTION(BlueprintCallable, Category = "QuestEditor-Node")
		static bool AreNodeArraysEqual(const TArray <FQuestTaskID>& NodesA, const TArray <FQuestTaskID>& NodesB);
	
	/**
	*	Finds all nodes of quest that have the same task type as the node selected.
	*/
	UFUNCTION(BlueprintCallable, Category = "QuestEditor-Node")
		static TArray <FQuestTaskID> GetAllNodesOfSameTaskType(const FQuest& QuestIn, const FQuestTaskID NodesIn);
	   
	/**
	*	Determines the node grid tier based on size and position and grid dimensions.
	*/
	UFUNCTION(BlueprintCallable, Category = "QuestEditor-Node")
		static FVector2D GetNodeGridTier(const FQuest& QuestIn, const FQuestTaskID NodeIn, const FVector2D& NodeWidgetSizeIn, const FVector2D& GridSizeIn);


	//-----------------------------------------------------------------------------------------
	//---------------------------------------------NPC
	//-----------------------------------------------------------------------------------------
private:

	/*
	*	Gets Start nodes that are not ActivateConditionOnly.
	*/
	UFUNCTION(BlueprintCallable, Category = "QuestEditor-Hierarchy")
		static TArray <FQuestTaskID>  GetQuestNonConditionalStartNodes(const FQuest& QuestIn);

	/**
	*	Gets all pseudo start nodes of quest.
	*/
	UFUNCTION(BlueprintCallable, Category = "QuestEditor-NPC")
		static TArray <FQuestTaskID>  GetQuestPseudoStartNodes(const FQuest& QuestIn);

	/**
	*	Gets all NPCs that can start this quest.
	*/
	UFUNCTION(BlueprintCallable, Category = "QuestEditor-NPC")
		static TArray <FNPCQuestDefRep>  GetQuestStartNPCs(const FQuest& QuestIn);

	/**
	*	Gets all start task nodes that are activate cond only
	*	This is used to start the Get Pseudo start talk tasks process 
	*	This is not a good check, only checks name of npc. Most of the times should be correct
	*	If you have 2 NPCs with same name in same or different DT might be wrong.
	*/
	UFUNCTION(BlueprintCallable, Category = "QuestEditor-NPC")
		static TArray <int>  GetInitialPseudoTasksOfQuest(const FQuest& QuestIn);

	/**	
	*	Gets all start talk nodes for NPC by name.
	*	This is not a good check, only checks name of npc. Most of the times should be correct
	*	If you have 2 NPCs with same name in same or different DT might be wrong.
	*/
	UFUNCTION(BlueprintCallable, Category = "QuestEditor-NPC")
		static TArray <int>  GetStartingTalkTaskForNPC(const FQuest& QuestIn, const FString& NPCNameIn);

	/**
	*	Gets all pseudo start talk nodes for NPC by name.
	*	Does not care if the activate condition returns true, just asumes it does.
	*	This is not a good check, only checks name of npc. Most of the times should be correct
	*	If you have 2 NPCs with same name in same or different DT might be wrong.
	*/
	UFUNCTION(BlueprintCallable, Category = "QuestEditor-NPC")
		static TArray <int>  GetStartingPseudoTalkTaskForNPC(const FQuest& QuestIn, const FString& NPCNameIn);

	/**	
	*	Determines if the NPC of this name can give this quest if assigned to him.
	*	This is not a good check, only checks name of npc. Most of the times should be correct
	*	If you have 2 NPCs with same name in same or different DT might be wrong.
	*	If the quest has a direct start, then this will be false.
	*/
	UFUNCTION(BlueprintCallable, Category = "QuestEditor-NPC")
		static bool NPCCanGiveQuest(const FQuest& QuestIn, const FString& NPCNameIn);

	/**
	*	Get All gets that this NPC can start.
	*	If you have 2 NPCs with same name in same or different DT might be wrong.
	*/
	UFUNCTION(BlueprintCallable, Category = "QuestEditor-NPC")
		static TArray <FQuestRep> GetAllQuestsInDTForNPC(TSoftObjectPtr <UDataTable> Datatable, const FString& NPCNameIn);

	//-----------------------------------------------------------------------------------------
	//----------------------------------------------DEBUG AND BLUETILITY PLUGIN
	//-----------------------------------------------------------------------------------------

	/**Gets the game world.*/
	UFUNCTION(BlueprintCallable, Category = "QuestEditor-World")
		static UWorld* GetQuestsGameWorld();

	//-----------------------------------------------------------------------------------------
	//----------------------------------------------QUEST CHAINS HIERARCHY
	//-----------------------------------------------------------------------------------------

	/*
	*	Gets all quests that are required for a quest to start.
	*	Partial requeriments are considered. If a quest is only required in one start node is considered to be superior quest.
	*/
	UFUNCTION(BlueprintCallable, Category = "QuestEditor-Hierarchy")
		static TArray <FQuestRep>  GetAllSuperiorQuestsOfQuest(const FQuest& QuestIn);

	/*
	*	Gets all quests that require this quest as a starting condition.
	*	Partial requeriments are considered. If a quest is only required in one start node is considered to be inferior quest.
	*	Might be heavy on performance since it searches through all quest DTs
	*	Unreliable. Needs to find quest in datatables, wich means that if names overlap this can be innacurated.
	*/
	UFUNCTION(BlueprintCallable, Category = "QuestEditor-Hierarchy")
		static TArray <FQuestRep>  GetAllInferiorQuestsOfQuestRep(const FQuestRep& QuestRepIn, TArray <UDataTable*> DatatablesIn);

	/*
	*	Gets the link type between two quests.
	*	If all start nodes require the quest to start and the link between condition and pseudo start is required, then the link is required, otherwise is optional.
	*	Assumes that quests ARE linked. If quests are not link this function wont behave correctly. Designed to use always with connected quests.
	*/
	UFUNCTION(BlueprintCallable, Category = "QuestEditor-Hierarchy")
		static ENodeLinkType  GetLinkTypeBetweenConnectedQuests(const FQuestRep& SuperiorQuestIn, const FQuestRep& InferiorQuestIn);
	
	/*
	*	Given a group of node links that are all start nodes and connected to a single Pseudo start, determines if this pack of nodes is connected to the Superior Quest
	*/
	UFUNCTION(BlueprintCallable, Category = "QuestEditor-Hierarchy")
		static bool  AreGroupOfNodesConectedToQuest(const FQuestRep& SuperiorQuestIn, const FQuestRep& InferiorQuestIn, const TArray<FQuestTaskLink>& NodeLinksIn, ENodeLinkType& LinkTypeOut);

	/*
	*	Detects if a task of the inferior quest is a requeriment to have completed the superior quest in order to be accepted.
	*	@Param QuestOut The Quest Involved in the conditional task.
	*/
	UFUNCTION(BlueprintCallable, Category = "QuestEditor-Hierarchy")
		static bool  ConditionalTaskIsLinkedToQuest(const FQuestRep& SuperiorQuestIn, const FQuestRep& InferiorQuestIn, const FQuestTaskID& NodeIn, FQuestRep& QuestOut);

	/*
	*	Converts a quest rep to a task that can go inside a quest for hierarchy three.
	*/
	UFUNCTION(BlueprintCallable, Category = "QuestEditor-Hierarchy")
		static FQuestTask  ConvertQuestRepToHierarchyTask(const FQuestRep& QuestIn);


	/*
	*	Creates a task that represents a direct start in the hierarchy.
	*/
	UFUNCTION(BlueprintCallable, Category = "QuestEditor-Hierarchy")
		static FQuestTask  CreateDirectStartHierarchyTask();

	/*
	*	For a hierarchy quest, adds the superior nodes of a node to the tasks array and links them.
	*	Adjusts positions and links automatically for presenting in the editor based on the node tier.
	*	Returns task IDs off added quests.
	*	@Param HierarchyQuestIn Partial Hierarchy Quest.
	*	@Param QuestRepNodeIn Task ID of the node that represents a quest in the hierarchy.
	*	@Param SuperiorNodeTier Tier of the node, 0 represents the first Quest and it goes up for each superior node layer added.
	*	@Param PropagateUp If true propagates for superior quests, if not for inferior quests.
	*	@Param DatatablesIn Quest datatables of the system. Used for propagating down.
	*/
	UFUNCTION(BlueprintCallable, Category = "QuestEditor-Hierarchy")
		static TArray <FQuestTaskID>  AddHierarchyOfNodeToQuest(UPARAM(ref)FQuest& HierarchyQuestIn, const FQuestTaskID& QuestRepNodeIn, int SuperiorNodeTier, bool PropagateUp, TArray <UDataTable*> DatatablesIn);
	
	/*
	*	For a hierarchy quest, adds the inferior nodes and the quest itself to the QuestHierarchy.
	*	Adjusts positions and links automatically for presenting in the editor.
	*	Also sets additional variables of quest.
	*	@Param HierarchyQuestIn Initial Hierarchy Quest. Must be Zeroed Quest.
	*	@Param QuestRepIn Initial quest in the hierarchy.
	*	@Param SuperiorNodeTier Tier of the node, 0 represents the first Quest and it goes up for each superior node layer added.
	*/
	UFUNCTION(BlueprintCallable, Category = "QuestEditor-Hierarchy")
		static void  AddInitialHierarchyOfQuest(UPARAM(ref)FQuest& HierarchyQuestIn, const FQuestRep& QuestRepIn, TArray <UDataTable*> DatatablesIn);
	
	/*
	*	For a hierarchy quest, adds the superior or inferior nodes to the QuestHierarchy, propagating.
	*	Adjusts positions and links automatically for presenting in the editor.
	*	Also sets additional variables of quest.
	*	@Param HierarchyQuestIn Hierarchy Quest.
	*	@Param QuestRepIn Initial quest in the hierarchy.
	*	@Param MaxSuperiorTier Max propagation tier. Can be used to avoid performance issues. If 0 will propagate infinitely.
	*/
	UFUNCTION(BlueprintCallable, Category = "QuestEditor-Hierarchy")
		static void  PropagateQuestsInHierarchy(UPARAM(ref)FQuest& HierarchyQuestIn, const FQuestRep& QuestRepIn, int MaxSuperiorTier, bool PropagateUp, TArray <UDataTable*> DatatablesIn);

	/*
	*	Creates a quest that represents the chain hierarchy.
	*	Adjusts positions and links automatically for presenting in the editor.
	*	The amount of quests to show in hierarchy can be adjusted. 
	*	Calculating inferior quests is significantly heavier than superior quests.
	*/
	UFUNCTION(BlueprintCallable, Category = "QuestEditor-Hierarchy")
		static FQuest  GetQuestHierarchy(const FQuestRep& QuestRepIn, TArray <UDataTable*> DatatablesIn, int MaxSuperiorTier, int MaxInferiorTier);	

	/**
		Gets total nodes in a tier.
	*	Returns 0 if tier does not exist.
	*/
	UFUNCTION(BlueprintCallable, Category = "QuestEditor-Hierarchy")
		static int  GetAmountOfNodesInHierarchyTier(const FQuest& QuestIn, int YTierIn);

	/** 
	*	Gets the maximun amount of nodes in a Y tier, along X.
	*	Checks all tiers.
	*/
	UFUNCTION(BlueprintCallable, Category = "QuestEditor-Hierarchy")
		static int  GetMaxAmountOfNodesInHierarchyTiers(const FQuest& QuestIn, int& TierOut);

	/**
	*	Gets the maximun tier in Y.
	*	Checks all tiers.
	*/
	UFUNCTION(BlueprintCallable, Category = "QuestEditor-Hierarchy")
		static int  GetMaxHierarchyYTier(const FQuest& QuestIn);

	/**
	*	Gets the minimun tier in Y.
	*	Checks all tiers.
	*/
	UFUNCTION(BlueprintCallable, Category = "QuestEditor-Hierarchy")
		static int  GetMinHierarchyYTier(const FQuest& QuestIn);

	/**
	*	Gets total nodes in a tier.
	*	Uses payload vector to obtain tiers.
	*/
	UFUNCTION(BlueprintCallable, Category = "QuestEditor-Hierarchy")
		static void  SetNodePositionByTierInHierarchy(UPARAM(ref)FQuest& HierarchyQuestIn, FQuestTaskID NodeIn, int MaxTaskInAnyTier);

	/**
	*	Try to adjust node positions based on the position of the inferior or superior nodes.
	*	@Param UseInferiorNodes If true, will use inferior nodes, if false is gonna use superior nodes.
	*/
	UFUNCTION(BlueprintCallable, Category = "QuestEditor-Hierarchy")
		static void  AdjustAllNodePositionsInTierByConnectedNodesInHierarchy(UPARAM(ref)FQuest& HierarchyQuestIn, int TierIn, bool UseInferiorNodes);

	/**
	*	Gets all inferior nodes of a node that have immediate lower tier to the node in the hierarchy.
	*/
	UFUNCTION(BlueprintCallable, Category = "QuestEditor-Hierarchy")
		static TArray <int> GetInferiorNodesOfNodeWithImmediateHierarchyTier(const FQuest& HierarchyQuestIn, int NodeID);

	/**
	*	Gets all inferior nodes of a node that have immediate lower tier to the node in the hierarchy.
	*/
	UFUNCTION(BlueprintCallable, Category = "QuestEditor-Hierarchy")
		static TArray <int> GetSuperiorNodesOfNodeWithImmediateHierarchyTier(const FQuest& HierarchyQuestIn, int NodeID);

	/**
	*	Gets all nodes IDs of a tier in hierarchy.
	*/
	UFUNCTION(BlueprintCallable, Category = "QuestEditor-Hierarchy")
		static TArray <int> GetNodesIDsByTierInHierarchy(const FQuest& HierarchyQuestIn, int TierIn);

	/**
	*	Gets the average position in X of nodes
	*/
	UFUNCTION(BlueprintCallable, Category = "QuestEditor-Hierarchy")
		static float GetNodesAverageXPositionInHierarchy(const FQuest& HierarchyQuestIn, TArray <int> NodesIn);

	/**
	*	Detects if are nodes overlapping in position X in the tier.
	*	Returns the two nodes that are overlapping position that were found first.
	*/
	UFUNCTION(BlueprintCallable, Category = "QuestEditor-Hierarchy")
		static TArray <int>  GetNodesInTierOverlappingPosition(const FQuest& HierarchyQuestIn, int TierIn);

	/**
	*	Moves the nodes untill there is no overlap in X for a particular tier.
	*/
	UFUNCTION(BlueprintCallable, Category = "QuestEditor-Hierarchy")
		static void  AdjustNodePairPositionsToAvoidOverlap(UPARAM(ref)FQuest& HierarchyQuestIn, TArray <int> NodePairIn);

	/**
	*	Moves the nodes untill there is no overlap in X for a particular tier.
	*/
	UFUNCTION(BlueprintCallable, Category = "QuestEditor-Hierarchy")
		static void  AdjustNodePositionsToAvoidOverlapInTier(UPARAM(ref)FQuest& HierarchyQuestIn, int TierIn);

	/**
	*	Moves the nodes untill there is no overlap in X for all tiers of quest.
	*/
	UFUNCTION(BlueprintCallable, Category = "QuestEditor-Hierarchy")
		static void  AdjustAllTierPositionsToAvoidOverlap(UPARAM(ref)FQuest& HierarchyQuestIn);

	/**
	*	Gets the offset when adjusting overlapping nodes in X.
	*/
	UFUNCTION(BlueprintCallable, Category = "QuestEditor-Hierarchy")
		static int  GetHierarchyAdjustOffset();

	/**
	*	Determine if a node is already ocupping a position.
	*/
	UFUNCTION(BlueprintCallable, Category = "QuestEditor-Hierarchy")
		static bool  IsNodeNearPositionInHierarchy(const FQuest& HierarchyQuestIn, int PositionXIn, int TierIn);

	/**
	*	Sets all nodes of quest based on their tiers.
	*/
	UFUNCTION(BlueprintCallable, Category = "QuestEditor-Hierarchy")
		static void  SetAllNodePositionsInHierarchy(UPARAM(ref)FQuest& HierarchyQuestIn);

	/**
	*	Filters all nodes and merges them in one if they appear more than once.
	*/
	UFUNCTION(BlueprintCallable, Category = "QuestEditor-Hierarchy")
		static void  FilterRepeatedNodesInHierarchy(UPARAM(ref)FQuest& HierarchyQuestIn);

	/**
	*	Adjusts all tiers, collpasing them if there are any tier missing in X.
	*/
	UFUNCTION(BlueprintCallable, Category = "QuestEditor-Hierarchy")
		static void  CollapseXTiers(UPARAM(ref)FQuest& HierarchyQuestIn);
	
	/**
	*	Detects if a quest should have a direct start node in hierarchy.
	*	Requires at least one no conditional starts and at least 1 conditional start
	*/
	UFUNCTION(BlueprintCallable, Category = "QuestEditor-Hierarchy")
		static bool  QuestRequiresDirectStartNode(const FQuest& QuestIn);


};
