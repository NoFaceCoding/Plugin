//Copyright 2020 Marchetti S. Alfredo I. All Rights Reserved.


#include "Quest/QuestsFunctionLibrary.h"

#include "Quest/QuestManager.h"
#include "Task/QuestTaskInterface.h"
#include "TaskSelectReward/QuestTaskRewardInterface.h"
#include "TaskSelectReward/BaseQuestRewardSlot.h"

#include "Task/BaseQuestTask.h"

//GetSaveGames function includes
#include "Quest/QuestSystemSaveGame.h"

//Savegames
#include "Misc/Paths.h"
#include "HAL/PlatformFilemanager.h"
#include "Misc/LocalTimestampDirectoryVisitor.h"
#include "Kismet/GameplayStatics.h"

//Log
#include "QuestEditor.h"

bool UQuestsFunctionLibrary::EqualQuestTaskID(FQuestTaskID A, FQuestTaskID B)
{
	return (A == B);
}

bool UQuestsFunctionLibrary::EqualTalkMode(FTalkModeDef A, FTalkModeDef B)
{
	return (A == B);
}

bool UQuestsFunctionLibrary::EqualTalkTextInfo(FTalkTextInfo A, FTalkTextInfo B)
{
	return (A == B);
}

bool UQuestsFunctionLibrary::EqualQuestRep(FQuestRep A, FQuestRep B)
{
	return (A == B);
}

bool UQuestsFunctionLibrary::EqualDialogRep(FDialogRep A, FDialogRep B)
{
	return (A == B);
}

bool UQuestsFunctionLibrary::EqualNPCQuestDefRep(FNPCQuestDefRep A, FNPCQuestDefRep B)
{
	return (A == B);
}

bool UQuestsFunctionLibrary::EqualQuestNode(FQuestNode A, FQuestNode B)
{
	return (A == B);
}

bool UQuestsFunctionLibrary::NotEqualQuestTaskID(FQuestTaskID A, FQuestTaskID B)
{
	return (A != B);
}

bool UQuestsFunctionLibrary::NotEqualTalkMode(FTalkModeDef A, FTalkModeDef B)
{
	return (A != B);
}

bool UQuestsFunctionLibrary::NotEqualTalkTextInfo(FTalkTextInfo A, FTalkTextInfo B)
{
	return (A != B);
}

bool UQuestsFunctionLibrary::NotEqualQuestRep(FQuestRep A, FQuestRep B)
{
	return (A != B);
}

bool UQuestsFunctionLibrary::NotEqualDialogRep(FDialogRep A, FDialogRep B)
{
	return (A != B);
}

bool UQuestsFunctionLibrary::NotEqualNPCQuestDefRep(FNPCQuestDefRep A, FNPCQuestDefRep B)
{
	return (A != B);
}

bool UQuestsFunctionLibrary::NotEqualQuestNode(FQuestNode A, FQuestNode B)
{
	return (A != B);
}

FQuest UQuestsFunctionLibrary::QuestRepToQuest(const FQuestRep& QuestRepIN)
{
	//Finds DT for quest
	UDataTable* DT;
	DT = GetDTFromPath(QuestRepIN.Datatable);
	if (!DT) return FQuest();

	FQuest* QuestPointer = DT->FindRow<FQuest>(QuestRepIN.QuestDTName, FString(), false);
	FQuest QuestLocal = FQuest();
	if (QuestPointer != nullptr)
	{
		QuestLocal = *QuestPointer;
	}

	return QuestLocal;
}

FDialog UQuestsFunctionLibrary::DialogRepToDialog(const FDialogRep& DialogRepIN)
{
	//Finds DT for dialog
	UDataTable* DT;
	DT = GetDTFromPath(DialogRepIN.Datatable);
	if (!DT) return FDialog();

	FDialog* DialogPointer = DT->FindRow<FDialog>(DialogRepIN.DialogsDTName, FString(), false);
	FDialog DialogLocal = FDialog();
	
	if (DialogPointer != nullptr)
	{
		DialogLocal = *DialogPointer;
	}

	return DialogLocal;
}

UDataTable* UQuestsFunctionLibrary::GetDTFromReference(FString QuestDTName, FString DTFolderPath)
{
	UDataTable* DT = nullptr;

	//invalid path?
	if (DTFolderPath == "None" || DTFolderPath == FString()) return DT;

	FString DTPath = DTFolderPath + QuestDTName + "." + QuestDTName;

	//Find object
	DT = FindObject<UDataTable>(nullptr, *DTPath, false);

	//if is not found load it
	if (DT == nullptr) DT = LoadObject<UDataTable>(nullptr, *DTPath);
	return DT;
}

UDataTable* UQuestsFunctionLibrary::GetDTFromPath(TSoftObjectPtr <UDataTable> DTPathIN)
{
	//Find object
	UDataTable* DT = nullptr;

	//invalid path?
	//if (!DTPathIN.IsValid()) return DT;
	
	//Find object
	//DT = FindObject<UDataTable>(nullptr, *DTPathIN, false);

	//if is not found load it
	//if (DT == nullptr) 
	DT = LoadObject<UDataTable>(nullptr, *DTPathIN.ToString());
	return DT;
}

UDataTable* UQuestsFunctionLibrary::GetDTFromStringPath(FString DTPathIN)
{
	//Find object
	UDataTable* DT = nullptr;

	//invalid STRING path?
	if (DTPathIN == FString() || DTPathIN == "None") return DT;

	//Find object
	//DT = FindObject<UDataTable>(nullptr, *DTPathIN, false);

	//if is not found load it
	if (DT == nullptr) DT = LoadObject<UDataTable>(nullptr, *DTPathIN);

	return DT;
}

FNPCQuestDef UQuestsFunctionLibrary::NPCQuestDefRepToNPCQuestDef(const FNPCQuestDefRep& FNPCQuestDefIn)
{
	//Finds DT for NPC info (similar to questreptoquest function)
	UDataTable* DT;
	DT = GetDTFromPath(FNPCQuestDefIn.Datatable);
	if (!DT) return FNPCQuestDef();

	FNPCQuestDef* PointerLocal = DT->FindRow<FNPCQuestDef>(FNPCQuestDefIn.NPCDefDTName, FString(), false);
	FNPCQuestDef StructLocal = FNPCQuestDef();

	if (PointerLocal != nullptr)
	{
		StructLocal = *PointerLocal;
	}
	return StructLocal;
}

FString UQuestsFunctionLibrary::MakeStringReference(const FString& DTNameIn, const FString& DTPathIn)
{
	FString DTPath = DTPathIn + DTNameIn + "." + DTNameIn;
	return DTPath;
}

FString UQuestsFunctionLibrary::QuestRepToString(const FQuestRep& QuestRepIN)
{
	FString Separator = "**";
	FString StringCode = FString();

	//Append DTName 
	StringCode.Append(QuestRepIN.QuestDTName.ToString());
	StringCode.Append(Separator);
	//Append DTPath 
	StringCode.Append(QuestRepIN.Datatable.ToString());

	return StringCode;
}

FString UQuestsFunctionLibrary::DialogRepToString(const FDialogRep& DialogRepIN)
{
	FString Separator = "**";
	FString StringCode = FString();

	//Append DTName 
	StringCode.Append(DialogRepIN.DialogsDTName.ToString());
	StringCode.Append(Separator);
	//Append DTPath 
	StringCode.Append(DialogRepIN.Datatable.ToString());

	return StringCode;
}

FString UQuestsFunctionLibrary::NPCQuestDefRepToString(const FNPCQuestDefRep& FNPCQuestDefIn)
{
	FString Separator = "**";
	FString StringCode = FString();

	//Append DTName 
	StringCode.Append(FNPCQuestDefIn.NPCDefDTName.ToString());
	StringCode.Append(Separator);
	//Append DTPath  
	StringCode.Append(FNPCQuestDefIn.Datatable.ToString());

	return StringCode;
}

FString UQuestsFunctionLibrary::TaskIDToString(const FQuestTaskID& QuestTaskIDIn)
{
	FString Separator = "**";
	FString StringCode = FString();

	//Apend ID
	StringCode.AppendInt(QuestTaskIDIn.TaskID);
	StringCode.Append(Separator);

	//Append Type as integer		
	uint8 bytes = (uint8)QuestTaskIDIn.TaskType;
	StringCode.AppendInt(bytes);
	StringCode.Append(Separator);

	return StringCode;
}

FString UQuestsFunctionLibrary::QuestNodeToString(const FQuestNode& QuestNodeIn)
{
	//Needs a diferent separator
	FString Separator = ":::";
	FString StringCode = FString();

	//Append quest rep as string 
	StringCode.Append(QuestRepToString(QuestNodeIn.Quest));
	StringCode.Append(Separator);
	//Append TaskId as string
	StringCode.Append(TaskIDToString(QuestNodeIn.Node));

	return StringCode;
}

FQuestRep UQuestsFunctionLibrary::StringToQuestRep(const FString& StringIn)
{
	FQuestRep QuestLocal = FQuestRep();
	TArray <FString> ModificationsArray;
	StringIn.ParseIntoArray(ModificationsArray, TEXT("**"), false);

	//--------------Convert 0 to name
	if (!ModificationsArray.IsValidIndex(0)) return FQuestRep();
	QuestLocal.QuestDTName = FName(*ModificationsArray[0]);
	//--------------Convert 1 to path
	if (!ModificationsArray.IsValidIndex(1)) return FQuestRep();
	QuestLocal.Datatable = ModificationsArray[1];
	
	return QuestLocal;
}

FDialogRep UQuestsFunctionLibrary::StringToDialogRep(const FString& StringIn)
{
	FDialogRep DialogLocal = FDialogRep();
	TArray <FString> ModificationsArray;
	StringIn.ParseIntoArray(ModificationsArray, TEXT("**"), false);

	//--------------Convert 0 to name
	if (!ModificationsArray.IsValidIndex(0)) return FDialogRep();
	DialogLocal.DialogsDTName = FName(*ModificationsArray[0]);
	//--------------Convert 1 to path
	if (!ModificationsArray.IsValidIndex(1)) return FDialogRep();
	DialogLocal.Datatable = ModificationsArray[1];

	return DialogLocal;
}

FNPCQuestDefRep UQuestsFunctionLibrary::StringToNPCQuestDefRep(const FString& StringIn)
{
	//Invalid string?
	if (StringIn == "") return FNPCQuestDefRep();

	FNPCQuestDefRep NPCLocal = FNPCQuestDefRep();
	TArray <FString> ModificationsArray;
	StringIn.ParseIntoArray(ModificationsArray, TEXT("**"), false);

	//--------------Convert 0 to name
	if (!ModificationsArray.IsValidIndex(0)) return FNPCQuestDefRep();
	NPCLocal.NPCDefDTName = FName(*ModificationsArray[0]);
	//--------------Convert 1 to path
	if (!ModificationsArray.IsValidIndex(1)) return FNPCQuestDefRep();
	NPCLocal.Datatable = ModificationsArray[1];

	return NPCLocal;
}

FQuestTaskID UQuestsFunctionLibrary::StringToTaskID(const FString& StringIn)
{
	FQuestTaskID IDLocal = FQuestTaskID();
	TArray <FString> ModificationsArray;
	StringIn.ParseIntoArray(ModificationsArray, TEXT("**"), false);

	//---------------Convert 0 to ID
	if (!ModificationsArray.IsValidIndex(0)) return FQuestTaskID();
	IDLocal.TaskID = FCString::Atoi(*ModificationsArray[0]);
	
	//--------------Convert 1 to Type
	if (!ModificationsArray.IsValidIndex(1)) return FQuestTaskID();
	int IclassInt = FCString::Atoi(*ModificationsArray[1]);
	//convert int to enum
	IDLocal.TaskType = (EQuestTaskType)IclassInt;

	return IDLocal;
}

FQuestNode UQuestsFunctionLibrary::StringToQuestNode(const FString& StringIn)
{	
	FQuestNode QuestEndLocal = FQuestNode();
	TArray <FString> ModificationsArray;
	//Use correct separator
	StringIn.ParseIntoArray(ModificationsArray, TEXT(":::"), false);

	//--------------Convert 0 to questrep
	if (!ModificationsArray.IsValidIndex(0)) return FQuestNode();
	QuestEndLocal.Quest = StringToQuestRep(ModificationsArray[0]);
	//--------------Convert 1 to taskid
	if (!ModificationsArray.IsValidIndex(1)) return FQuestNode();
	QuestEndLocal.Node = StringToTaskID(ModificationsArray[1]);

	return QuestEndLocal;
}

TArray<FQuestTaskLink> UQuestsFunctionLibrary::RemoveTaskIDFromLinkArray(const FQuestTaskID& TaskIDIn, const TArray<FQuestTaskLink>& LinksArrayIn)
{
	TArray<FQuestTaskLink> LinksLocal = TArray<FQuestTaskLink>();

	for (int i = 0; i < LinksArrayIn.Num(); i++)
	{
		//If node is not the one to remove add to local array
		if (LinksArrayIn[i].Node != TaskIDIn)
		{
			LinksLocal.Add(LinksArrayIn[i]);
		}
	}	
	
	return LinksLocal;
}

TArray<FQuestTaskID> UQuestsFunctionLibrary::ConvertLinkArrayToNodeArray(const TArray<FQuestTaskLink> LinkArrayIn)
{
	TArray<FQuestTaskID> LocalNodes = TArray<FQuestTaskID>();

	for (int i = 0; i < LinkArrayIn.Num(); i++)
	{
		//Add the node to array, discarding link type
		LocalNodes.Add(LinkArrayIn[i].Node);
	}

	return LocalNodes;
}

TArray<FQuestTaskID> UQuestsFunctionLibrary::GetNodesOfTypeInArray(EQuestTaskType TaskTypeToMatch, const TArray <FQuestTaskID> NodesArrayIn)
{
	TArray <FQuestTaskID> NodesArrayOut;
	for (int i = 0; i < NodesArrayIn.Num(); i++)
	{
		//Matches type?
		if (NodesArrayIn[i].TaskType == TaskTypeToMatch)
		{
			//Add to output
			NodesArrayOut.Add(NodesArrayIn[i]);
		}
	}	
	
	return NodesArrayOut;
}

float UQuestsFunctionLibrary::GetActiveTaskTimer(int QuestSlotIn, int QuesTaskIn, bool IsDialogIn, UQuestManager* QuestManagerRef)
{
	float timerint = -1.0f;

	if (!QuestManagerRef) return timerint;

	UBaseQuestTask* ObjectTask = QuestManagerRef->GetTaskObject(QuestSlotIn, QuesTaskIn, IsDialogIn);

	//Object null returns null
	if (ObjectTask == nullptr) return timerint;

	//If does not implements interface 
	if (!ObjectTask->GetClass()->ImplementsInterface(UQuestTaskInterface::StaticClass())) return timerint;

	//calls interface
	timerint = IQuestTaskInterface::Execute_GetTaskTimer(ObjectTask);

	return timerint;
}

FString UQuestsFunctionLibrary::GetTaskShortDescription(const FQuestTask& TaskIn)
{
	FString StringOut = FString();

	//Obtain class to load
	TSoftClassPtr<UBaseQuestTask> TaskClassInterna = TaskIn.TaskClass;

	//Class null returns null
	if (TaskClassInterna == nullptr) return StringOut;

	//Load class from soft ref
	TSubclassOf <UBaseQuestTask> LoadedClass;
	LoadedClass = TaskClassInterna.LoadSynchronous();
	const UBaseQuestTask* DefaultTask = TaskClassInterna->GetDefaultObject<UBaseQuestTask>();

	//If does not implements interface 
	if (!DefaultTask->GetClass()->ImplementsInterface(UQuestTaskInterface::StaticClass())) return StringOut;

	//calls interface
	StringOut = IQuestTaskInterface::Execute_GetTaskShortDescription(DefaultTask, TaskIn.Payload, TaskIn.CustomPayloadData);

	return StringOut;
}

FString UQuestsFunctionLibrary::GetTaskName(const FQuestTask& TaskIn)
{
	FString StringOut = FString();

	//Obtain class to load
	TSoftClassPtr<UBaseQuestTask> TaskClassInterna = TaskIn.TaskClass;

	//Class null returns null
	if (TaskClassInterna == nullptr) return StringOut;

	//Load class from soft ref
	TSubclassOf <UBaseQuestTask> LoadedClass;
	LoadedClass = TaskClassInterna.LoadSynchronous();
	const UBaseQuestTask* DefaultTask = TaskClassInterna->GetDefaultObject<UBaseQuestTask>();

	//If does not implements interface 
	if (!DefaultTask->GetClass()->ImplementsInterface(UQuestTaskInterface::StaticClass())) return StringOut;

	//calls interface
	StringOut = IQuestTaskInterface::Execute_GetTaskName(DefaultTask);

	return StringOut;
}

FLinearColor UQuestsFunctionLibrary::GetTaskUIColor(const FQuestTask& TaskIn)
{
	FLinearColor ColorOut = FLinearColor();

	//Obtain class to load
	TSoftClassPtr<UBaseQuestTask> TaskClassInterna = TaskIn.TaskClass;

	//Class null returns null
	if (TaskClassInterna == nullptr) return ColorOut;

	//Load class from soft ref
	TSubclassOf <UBaseQuestTask> LoadedClass;
	LoadedClass = TaskClassInterna.LoadSynchronous();
	const UBaseQuestTask* DefaultTask = TaskClassInterna->GetDefaultObject<UBaseQuestTask>();

	//If does not implements interface 
	if (!DefaultTask->GetClass()->ImplementsInterface(UQuestTaskInterface::StaticClass())) return ColorOut;

	//calls interface
	ColorOut = IQuestTaskInterface::Execute_GetTaskColor(DefaultTask);

	return ColorOut;
}

bool UQuestsFunctionLibrary::GetTaskShouldAnnounce(const FQuestTask& TaskIn)
{
	bool AnnounceOut = false;

	//Obtain class to load
	TSoftClassPtr<UBaseQuestTask> TaskClassInterna = TaskIn.TaskClass;

	//Class null returns null
	if (TaskClassInterna == nullptr) return AnnounceOut;

	//Load class from soft ref
	TSubclassOf <UBaseQuestTask> LoadedClass;
	LoadedClass = TaskClassInterna.LoadSynchronous();
	const UBaseQuestTask* DefaultTask = TaskClassInterna->GetDefaultObject<UBaseQuestTask>();

	//If does not implements interface 
	if (!DefaultTask->GetClass()->ImplementsInterface(UQuestTaskInterface::StaticClass())) return AnnounceOut;

	//calls interface
	AnnounceOut = IQuestTaskInterface::Execute_TaskShouldAnnounce(DefaultTask);

	return AnnounceOut;
}

bool UQuestsFunctionLibrary::GetTaskShouldShowInUI(const FQuestTask& TaskIn)
{
	bool AnnounceOut = false;

	//Obtain class to load
	TSoftClassPtr<UBaseQuestTask> TaskClassInterna = TaskIn.TaskClass;

	//Class null returns null
	if (!TaskClassInterna) return AnnounceOut;

	//Load class from soft ref
	TSubclassOf <UBaseQuestTask> LoadedClass;
	LoadedClass = TaskClassInterna.LoadSynchronous();
	const UBaseQuestTask* DefaultTask = TaskClassInterna->GetDefaultObject<UBaseQuestTask>();

	if(!DefaultTask) return AnnounceOut;

	//If does not implements interface 
	if (!DefaultTask->GetClass()->ImplementsInterface(UQuestTaskInterface::StaticClass())) return AnnounceOut;

	//calls interface
	AnnounceOut = IQuestTaskInterface::Execute_TaskShouldShowInUI(DefaultTask);

	return AnnounceOut;
}

bool UQuestsFunctionLibrary::GetNodeShouldShowInUI(EQuestSelectedType SelectType, int QuestSlotIn, FQuestTaskID NodeIn, bool IsDialogIn, UQuestManager* QuestManagerRef)
{
	if (!QuestManagerRef) return false;
	//Talk Tasks always show
	if (NodeIn.TaskType == EQuestTaskType::TalkTask) return true;

	FQuestTask TaskInterna = QuestManagerRef->GetTask(SelectType, QuestSlotIn, NodeIn.TaskID, IsDialogIn);
	return GetTaskShouldShowInUI(TaskInterna);
}

bool UQuestsFunctionLibrary::GetTaskShowsOnlyOnceInSummary(const FQuestTask& TaskIn, FString& StringIDOut)
{
	bool AnnounceOut = false;

	//Obtain class to load
	TSoftClassPtr<UBaseQuestTask> TaskClassInterna = TaskIn.TaskClass;

	//Class null returns null
	if (TaskClassInterna == nullptr) return AnnounceOut;

	//Load class from soft ref
	TSubclassOf <UBaseQuestTask> LoadedClass;
	LoadedClass = TaskClassInterna.LoadSynchronous();
	const UBaseQuestTask* DefaultTask = TaskClassInterna->GetDefaultObject<UBaseQuestTask>();

	//If does not implements interface 
	if (!DefaultTask->GetClass()->ImplementsInterface(UQuestTaskRewardInterface::StaticClass())) return AnnounceOut;

	//calls interface
	FString IDLocal;
	AnnounceOut = IQuestTaskRewardInterface::Execute_TaskShowsOnlyOnceInSummary(DefaultTask, TaskIn.Payload, IDLocal);

	//Update out vars
	StringIDOut = IDLocal;
	return AnnounceOut;
}

bool UQuestsFunctionLibrary::GetTaskRewardGiver(const FQuestTask& TaskIn, FNPCQuestDefRep& RewardGiverOut)
{
	bool HasRewardGiver = false;

	//Obtain class to load
	TSoftClassPtr<UBaseQuestTask> TaskClassInterna = TaskIn.TaskClass;

	//Class null returns null
	if (TaskClassInterna == nullptr) return HasRewardGiver;

	//Load class from soft ref
	TSubclassOf <UBaseQuestTask> LoadedClass;
	LoadedClass = TaskClassInterna.LoadSynchronous();
	const UBaseQuestTask* DefaultTask = TaskClassInterna->GetDefaultObject<UBaseQuestTask>();

	//If does not implements interface 
	if (!DefaultTask->GetClass()->ImplementsInterface(UQuestTaskRewardInterface::StaticClass())) return HasRewardGiver;

	//calls interface
	FNPCQuestDefRep RewardGiverLocal;
	HasRewardGiver = IQuestTaskRewardInterface::Execute_TaskRewardGiver(DefaultTask, TaskIn.Payload, RewardGiverLocal);

	//Update out vars
	RewardGiverOut = RewardGiverLocal;
	return HasRewardGiver;
}

bool UQuestsFunctionLibrary::GetTaskRewardLocation(const FQuestTask& TaskIn, FString& LocationNameOut)
{
	bool HasRewardGiver = false;

	//Obtain class to load
	TSoftClassPtr<UBaseQuestTask> TaskClassInterna = TaskIn.TaskClass;

	//Class null returns null
	if (TaskClassInterna == nullptr) return HasRewardGiver;

	//Load class from soft ref
	TSubclassOf <UBaseQuestTask> LoadedClass;
	LoadedClass = TaskClassInterna.LoadSynchronous();
	const UBaseQuestTask* DefaultTask = TaskClassInterna->GetDefaultObject<UBaseQuestTask>();

	//If does not implements interface 
	if (!DefaultTask->GetClass()->ImplementsInterface(UQuestTaskRewardInterface::StaticClass())) return HasRewardGiver;

	//calls interface
	FString LocationNameLocal;
	HasRewardGiver = IQuestTaskRewardInterface::Execute_TaskRewardLocation(DefaultTask, TaskIn.Payload, LocationNameLocal);

	//Update out vars
	LocationNameOut = LocationNameLocal;
	return HasRewardGiver;
}

TSubclassOf<UBaseQuestRewardSlot> UQuestsFunctionLibrary::GetTaskRewardSlot(const FQuestTask& TaskIn)
{
	//Obtain class to load
	TSoftClassPtr<UBaseQuestTask> TaskClassInterna = TaskIn.TaskClass;

	//Class null returns null
	if (TaskClassInterna == nullptr) return nullptr;

	//Load class from soft ref
	TSubclassOf <UBaseQuestTask> LoadedClass;
	LoadedClass = TaskClassInterna.LoadSynchronous();
	const UBaseQuestTask* DefaultTask = TaskClassInterna->GetDefaultObject<UBaseQuestTask>();

	//If does not implements interface 
	if (!DefaultTask->GetClass()->ImplementsInterface(UQuestTaskRewardInterface::StaticClass())) return nullptr;

	//calls interface
	TSubclassOf<UBaseQuestRewardSlot> RewardSlot = IQuestTaskRewardInterface::Execute_TaskRewardSlot(DefaultTask);

	return RewardSlot;
}

FGameplayTagContainer UQuestsFunctionLibrary::GetTaskRewardTags(const FQuestTask& TaskIn)
{
	//Obtain class to load
	TSoftClassPtr<UBaseQuestTask> TaskClassInterna = TaskIn.TaskClass;

	//Class null returns null
	if (TaskClassInterna == nullptr) return FGameplayTagContainer();

	//Load class from soft ref
	TSubclassOf <UBaseQuestTask> LoadedClass;
	LoadedClass = TaskClassInterna.LoadSynchronous();
	const UBaseQuestTask* DefaultTask = TaskClassInterna->GetDefaultObject<UBaseQuestTask>();

	//If does not implements interface 
	if (!DefaultTask->GetClass()->ImplementsInterface(UQuestTaskRewardInterface::StaticClass())) return FGameplayTagContainer();

	//calls interface
	FGameplayTagContainer RewardTags = IQuestTaskRewardInterface::Execute_TaskRewardTags(DefaultTask);

	return RewardTags;
}

bool UQuestsFunctionLibrary::TaskIsActivateConditionOnly(const FQuest& QuestIn, int TaskSlot)
{
	//Is task valid?
	FQuestTaskID TaskIDLocal = FQuestTaskID();
	TaskIDLocal.TaskType = EQuestTaskType::Task;
	TaskIDLocal.TaskID = TaskSlot;
	if (!IsTaskIDValidInQuest(QuestIn, TaskIDLocal)) return false;
	
	//--------------------------------Activate condition for tasks
	if (!QuestIn.Tasks[TaskSlot].TaskClass.IsValid())
	{
		QuestIn.Tasks[TaskSlot].TaskClass.LoadSynchronous();
	}

	TSubclassOf <UBaseQuestTask> TaskClass = QuestIn.Tasks[TaskSlot].TaskClass.Get();

	if (TaskClass)
	{
		const UBaseQuestTask* TaskCDO = TaskClass.GetDefaultObject();
		//Get Status of task, is ACO ?
		return TaskCDO->GetTaskConditionOnlyStatus();
	}
	//invalid task return false
	return false;
}

TArray<FQuestTaskID> UQuestsFunctionLibrary::GetAllTalkTasksOfConversation(const FQuest& QuestIn, int TalkTaskSampleIn, int& ConversationIDOut)
{
	//Propagates up and down. Use Inferior and superior nodes. I get only talk tasks from same npc
	//------------------------------------------Superior nodes from active texts. First loop 

	TArray <FQuestTaskID> SuperiorNodesA = TArray <FQuestTaskID>(); //all superior initial nodes 
	TArray <FQuestTaskID> ReadedNodes = TArray <FQuestTaskID>();
	TArray <FQuestTaskID> SuperiorNodesB = TArray <FQuestTaskID>();
	TArray <FQuestTaskID> AnalizedNodes = TArray <FQuestTaskID>();
	//---------------------------------------All initial nodes 	

	//Add all nodes, superior and inferior of task 
	FQuestTaskID TaskTalkID;
	TaskTalkID.TaskID = TalkTaskSampleIn;
	TaskTalkID.TaskType = EQuestTaskType::TalkTask;

	//valid tasks
	if (!IsTaskIDValidInQuest(QuestIn, TaskTalkID)) return TArray<FQuestTaskID>();

	FQuestTalkTask TaskLocal = QuestIn.TalkTasks[TalkTaskSampleIn];

	//Start with all inferior and sup nodes of task
	SuperiorNodesA.Append(ConvertLinkArrayToNodeArray(TaskLocal.SuperiorNodes));
	SuperiorNodesA.Append(ConvertLinkArrayToNodeArray(TaskLocal.InferiorNodes));

	//Add sample ID to readed nodes
	ReadedNodes.Add(TaskTalkID);
	
	//Starting node is also added to analized, is the first one.
	AnalizedNodes.Add(TaskTalkID);

	bool WhileEnable = true;

	while (WhileEnable)
	{
		//Check all A nodes
		//If are talk nodes and same npc, add to array out.
		for (int i = 0; i < SuperiorNodesA.Num(); i++)
		{
			//already analized dont do anything
			if (!AnalizedNodes.Contains(SuperiorNodesA[i]))
			{
				//talk node?
				if (SuperiorNodesA[i].TaskType == EQuestTaskType::TalkTask)
				{
					//Is valid? Should be since i created the nodes array in the same function. But sometimes can crash?
					if (QuestIn.TalkTasks.IsValidIndex(SuperiorNodesA[i].TaskID))
					{
						//same NPC
						if (QuestIn.TalkTasks[SuperiorNodesA[i].TaskID].TalkNPC == TaskLocal.TalkNPC)
						{
							//Add to array of readed
							ReadedNodes.AddUnique(SuperiorNodesA[i]);
							//Add to Array B all superior nodes and inferior nodes
							//This might add duplicated nodes. This will need a filter later
							SuperiorNodesB.Append(GetNodeSuperiorNodesInQuest(QuestIn, SuperiorNodesA[i]));
							SuperiorNodesB.Append(GetNodeInferiorNodesInQuest(QuestIn, SuperiorNodesA[i]));
						}
					}
					else
					{
						//If talktas invalid, show error. This should never happend
						UE_LOG(QuestEditorLog, Error, TEXT("UQuestsFunctionLibrary::GetAllTalkTasksOfConversation - Quest: %s has no talktask with ID: %i."), *QuestIn.QuestName.ToString(), SuperiorNodesA[i].TaskID);
					}
				}
				//If is not talk task, but is a condition only, it will "Skip it".
				else
				{
					if (TaskIsActivateConditionOnly(QuestIn, SuperiorNodesA[i].TaskID))
					{
						//Add to Array B all superior nodes and inferior nodes
						//This might add duplicated nodes. This will need a filter later
						SuperiorNodesB.Append(GetNodeSuperiorNodesInQuest(QuestIn, SuperiorNodesA[i]));
						SuperiorNodesB.Append(GetNodeInferiorNodesInQuest(QuestIn, SuperiorNodesA[i]));
					}
				}					
			}
			//If node fails or not doesnt matter, its added to analizaed array to avoid doble check
			AnalizedNodes.AddUnique(SuperiorNodesA[i]);
		}

		//if B is empty cancel loop
		if (SuperiorNodesB.Num() == 0) WhileEnable = false;
		//Prepare for next loop
		else
		{
			SuperiorNodesA = SuperiorNodesB;
			SuperiorNodesB.Empty();
		}
	}

	//Find Conversation ID
	int ConversationIDLocal = 1000;

	for (int i = 0; i < ReadedNodes.Num(); i++)
	{
		//Save the lowest ID for the  conversation ID
		if (ConversationIDLocal > ReadedNodes[i].TaskID) ConversationIDLocal = ReadedNodes[i].TaskID;
	}

	//Update Conversation ID Out
	ConversationIDOut = ConversationIDLocal;

	return ReadedNodes;
}

TMap<FQuestTaskID, int> UQuestsFunctionLibrary::GetQuestConversationsMap(const FQuest& QuestIn)
{
	TMap<FQuestTaskID, int> ConversationsMap;

	//------------------------------------------Superior nodes from active texts. First loop 
	FQuestTaskID TaskIDLocal;
	TaskIDLocal.TaskType = EQuestTaskType::TalkTask;

	//To check. If i use the map to check contained and save some lines i get crashes on editor. This way is safer
	TArray <FQuestTaskID> AnalizedNodes = TArray <FQuestTaskID>();

	TArray <FQuestTaskID> LocalConversationOfNode = TArray <FQuestTaskID>();

	int ConversationIDLocal = 0;

	for (int i = 0; i < QuestIn.TalkTasks.Num(); i++)
	{
		//Is not analized
		TaskIDLocal.TaskID = i;
		if (!AnalizedNodes.Contains(TaskIDLocal))
		{
			//Get all nodes for this conversation
			LocalConversationOfNode = GetAllTalkTasksOfConversation(QuestIn, i, ConversationIDLocal);

			//Add map entry for this conversation.
			for (int j = 0; j < LocalConversationOfNode.Num(); j++)
			{
				//add each node with ther conversation ID
				ConversationsMap.Add(LocalConversationOfNode[j], ConversationIDLocal);
			}

			//Add nodes to analized array
			AnalizedNodes.Append(LocalConversationOfNode);
		}
		//If the conversations map has all nodes then there is no point in keep looping
		if (AnalizedNodes.Num() == QuestIn.TalkTasks.Num()) break;
	}

	return ConversationsMap;
}

bool UQuestsFunctionLibrary::IsTaskIDValidInQuest(const FQuest& QuestIn, const FQuestTaskID& TaskIDIn)
{
	//Invalid task ?
	switch (TaskIDIn.TaskType)
	{
	case EQuestTaskType::Task: return (QuestIn.Tasks.IsValidIndex(TaskIDIn.TaskID));
		break;
	case EQuestTaskType::TalkTask: return (QuestIn.TalkTasks.IsValidIndex(TaskIDIn.TaskID));
		break;
	default:
		break;
	}

	return false;
}

TArray<FQuestTaskID> UQuestsFunctionLibrary::GetNodeInferiorNodesInQuest(const FQuest& QuestIn, const FQuestTaskID& NodeIn)
{
	TArray<FQuestTaskID> NodesLocal = TArray<FQuestTaskID>();

	switch (NodeIn.TaskType)
	{
	case EQuestTaskType::Task:
	{
		NodesLocal = ConvertLinkArrayToNodeArray(QuestIn.Tasks[NodeIn.TaskID].InferiorNodes);
	}
	break;
	case EQuestTaskType::TalkTask:
	{
		NodesLocal = ConvertLinkArrayToNodeArray(QuestIn.TalkTasks[NodeIn.TaskID].InferiorNodes);
	}
	break;
	default:
		break;
	}
	return NodesLocal;
}

TArray<FQuestTaskID> UQuestsFunctionLibrary::GetNodeSuperiorNodesInQuest(const FQuest& QuestIn, const FQuestTaskID& NodeIn)
{
	TArray<FQuestTaskID> NodesLocal = TArray<FQuestTaskID>();

	switch (NodeIn.TaskType)
	{
	case EQuestTaskType::Task:
	{
		NodesLocal = ConvertLinkArrayToNodeArray(QuestIn.Tasks[NodeIn.TaskID].SuperiorNodes);
	}
	break;
	case EQuestTaskType::TalkTask:
	{
		NodesLocal = ConvertLinkArrayToNodeArray(QuestIn.TalkTasks[NodeIn.TaskID].SuperiorNodes);
	}
	break;
	default:
		break;
	}
	return NodesLocal;
}

TArray<FQuestTaskID> UQuestsFunctionLibrary::GetNodeDisabledNodesInQuest(const FQuest& QuestIn, const FQuestTaskID& NodeIn)
{
	if (!IsTaskIDValidInQuest(QuestIn, NodeIn)) return TArray <FQuestTaskID>();

	TArray <FQuestTaskID> TaskIDs;

	switch (NodeIn.TaskType)
	{
	case EQuestTaskType::Task: TaskIDs = QuestIn.Tasks[NodeIn.TaskID].DisabledNodesOnComplete;
		break;
	case EQuestTaskType::TalkTask:TaskIDs = QuestIn.TalkTasks[NodeIn.TaskID].DisabledNodesOnComplete;
		break;
	default:
		break;
	}
	return TaskIDs;
}

TArray<FQuestTaskLink> UQuestsFunctionLibrary::GetNodeInferiorLinksInQuest(const FQuest& QuestIn, const FQuestTaskID& NodeIn)
{
	TArray<FQuestTaskLink> NodesLocal = TArray<FQuestTaskLink>();

	switch (NodeIn.TaskType)
	{
	case EQuestTaskType::Task:
	{
		NodesLocal = QuestIn.Tasks[NodeIn.TaskID].InferiorNodes;
	}
	break;
	case EQuestTaskType::TalkTask:
	{
		NodesLocal = QuestIn.TalkTasks[NodeIn.TaskID].InferiorNodes;
	}
	break;
	default:
		break;
	}
	return NodesLocal;
}

TArray<FQuestTaskLink> UQuestsFunctionLibrary::GetNodeSuperiorLinksInQuest(const FQuest& QuestIn, const FQuestTaskID& NodeIn)
{
	TArray<FQuestTaskLink> NodesLocal = TArray<FQuestTaskLink>();

	switch (NodeIn.TaskType)
	{
	case EQuestTaskType::Task:
	{
		NodesLocal = QuestIn.Tasks[NodeIn.TaskID].SuperiorNodes;
	}
	break;
	case EQuestTaskType::TalkTask:
	{
		NodesLocal = QuestIn.TalkTasks[NodeIn.TaskID].SuperiorNodes;
	}
	break;
	default:
		break;
	}
	return NodesLocal;
}

bool UQuestsFunctionLibrary::AreAllSuperiorNodesOfNodeConditions(const FQuest& QuestIn, const FQuestTaskID& NodeIn)
{
	//Are all superior nodes of node conditional nodes?
	TArray<FQuestTaskID> SuperiorNodesLocal = GetNodeSuperiorNodesInQuest(QuestIn, NodeIn);

	bool result = true;

	for (int i = 0; i < SuperiorNodesLocal.Num(); i++)
	{
		//If it has a superior talk task the node is not a pseudo start
		if (SuperiorNodesLocal[i].TaskType == EQuestTaskType::TalkTask)
		{
			result = false;
			break;
		}
		//If is a task
		else if (SuperiorNodesLocal[i].TaskType == EQuestTaskType::Task)
		{
			//If not a activate condition only, then the node is not pseudo start
			if (!TaskIsActivateConditionOnly(QuestIn, SuperiorNodesLocal[i].TaskID))
			{
				result = false;
				break;
			}
		}
	}

	return result;
}

bool UQuestsFunctionLibrary::AreAllRequiredSuperiorNodesOfNodeConditions(const FQuest& QuestIn, const FQuestTaskID& NodeIn)
{
	//Are all superior REQUIRED nodes of node conditional nodes?
	TArray<FQuestTaskLink> SuperiorNodesLocal = GetNodeSuperiorLinksInQuest(QuestIn, NodeIn);

	bool result = true;

	for (int i = 0; i < SuperiorNodesLocal.Num(); i++)
	{
		//Node is required?
		if (SuperiorNodesLocal[i].LinkType == ENodeLinkType::Required)
		{
			//If it has a superior talk task the node is not a pseudo start
			if (SuperiorNodesLocal[i].Node.TaskType == EQuestTaskType::TalkTask)
			{
				result = false;
				break;
			}
			//If is a task
			else if (SuperiorNodesLocal[i].Node.TaskType == EQuestTaskType::Task)
			{
				//If not a activate condition only, then the node is not pseudo start
				if (!TaskIsActivateConditionOnly(QuestIn, SuperiorNodesLocal[i].Node.TaskID))
				{
					result = false;
					break;
				}
			}
		}		
	}

	return result;
}

bool UQuestsFunctionLibrary::IsNodeStartNode(const FQuest& QuestIn, const FQuestTaskID& NodeIn)
{
	//Invalid just return
	if (!IsTaskIDValidInQuest(QuestIn, NodeIn))
	{
		return false;
	}		

	//If has no superior nodes, then is start node
	TArray<FQuestTaskID> SuperiorNodesLocal = GetNodeSuperiorNodesInQuest(QuestIn, NodeIn);
	if (SuperiorNodesLocal.Num() == 0)
	{
		return true;
	}

	//This node cannot be a pseudo?
	if (!AreAllSuperiorNodesOfNodeConditions(QuestIn, NodeIn))
	{
		return false;
	}

	//If all superior nodes are start (CONDITION) nodes then this node is pseudo
	//assume true
	bool Result = true;
	TArray<FQuestTaskID> CurrentSuperiorNodes = TArray<FQuestTaskID>();

	for (int i = 0; i < SuperiorNodesLocal.Num(); i++)
	{
		//If has no superior nodes is start. If one has a req node then this is not a pseudo.
		CurrentSuperiorNodes = GetNodeSuperiorNodesInQuest(QuestIn, SuperiorNodesLocal[i]);

		//This is kinda a weak check, if u put conditions in serial manner. Should propagate UP instead untill it finds a no conditional node.
		if (CurrentSuperiorNodes.Num() != 0)
		{
			//Stop looping, already confirmed is not pseudo.
			Result = false;
			break;
		}
	}

	return Result;
}

TArray<FString> UQuestsFunctionLibrary::GetAllQuestSaveGameNames()
{	
	//--------------------------------Path for savegames is fixed
	FString FolderPathLocal = FPaths::ProjectDir() + "Saved/SaveGames/";

	//-------------------------------Get all files in directory
	TArray<FString> directoriesToSkip;
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	FLocalTimestampDirectoryVisitor Visitor(PlatformFile, directoriesToSkip, directoriesToSkip, false);
	PlatformFile.IterateDirectory(*FolderPathLocal, Visitor);
	TArray<FString> files = TArray<FString>();
	FString filePath = FString();
	FString fileName = FString();

	for (TMap<FString, FDateTime>::TIterator TimestampIt(Visitor.FileTimes); TimestampIt; ++TimestampIt)
	{
		filePath = TimestampIt.Key();
		fileName = FPaths::GetBaseFilename(filePath);
		//If has QuestData in its name, is a player save game.
		if (fileName.Contains("QuestData"))
		{
			files.Add(fileName);
		}			
	}
	
	return files;
}

TArray <UQuestSystemSaveGame*> UQuestsFunctionLibrary::GetAllQuestSaveGameObjects()
{
	//--------------------------------Path for savegames is fixed
	FString FolderPathLocal = FPaths::ProjectDir() + "Saved/SaveGames/";

	//-------------------------------Get all files in directory
	TArray<FString> directoriesToSkip;
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	FLocalTimestampDirectoryVisitor Visitor(PlatformFile, directoriesToSkip, directoriesToSkip, false);
	PlatformFile.IterateDirectory(*FolderPathLocal, Visitor);
	TArray<FString> files = TArray<FString>();
	FString filePath = FString();
	FString fileName = FString();

	for (TMap<FString, FDateTime>::TIterator TimestampIt(Visitor.FileTimes); TimestampIt; ++TimestampIt)
	{
		filePath = TimestampIt.Key();
		fileName = FPaths::GetBaseFilename(filePath);
		files.Add(fileName);		
	}

	//--------------------------------filter all questdata objects
	TArray <UQuestSystemSaveGame*> AllSaveFiles = TArray <UQuestSystemSaveGame*>();
	
	UQuestSystemSaveGame* LoadedGame = nullptr;

	for (int i = 0; i < files.Num(); i++)
	{
		//Save game exists?
		if (UGameplayStatics::DoesSaveGameExist(files[i], 0))
		{
			LoadedGame = Cast<UQuestSystemSaveGame>(UGameplayStatics::LoadGameFromSlot(files[i], 0));
			if (LoadedGame)
			{
				AllSaveFiles.Add(LoadedGame);
			}
		}
	}

	return AllSaveFiles;
}

bool UQuestsFunctionLibrary::DeleteQuestSaveGame(const FString& SaveNameIn)
{
	//----Build Save Name
	FString LocalSaveName = SaveNameIn + "QuestData";

	return UGameplayStatics::DeleteGameInSlot(LocalSaveName, 0);
}

void UQuestsFunctionLibrary::DeleteAllQuestSaveGames()
{
	//--------------------------------Path for savegames is fixed
	FString FolderPathLocal = FPaths::ProjectDir() + "Saved/SaveGames/";

	//-------------------------------Get all files in directory
	TArray<FString> directoriesToSkip;
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	FLocalTimestampDirectoryVisitor Visitor(PlatformFile, directoriesToSkip, directoriesToSkip, false);
	PlatformFile.IterateDirectory(*FolderPathLocal, Visitor);
	TArray<FString> files = TArray<FString>();
	FString filePath = FString();
	FString fileName = FString();

	for (TMap<FString, FDateTime>::TIterator TimestampIt(Visitor.FileTimes); TimestampIt; ++TimestampIt)
	{
		filePath = TimestampIt.Key();
		fileName = FPaths::GetBaseFilename(filePath);
		//If has QuestData in its name, is a player save game.
		if (fileName.Contains("QuestData"))
		{
			//Delete save game object
			UGameplayStatics::DeleteGameInSlot(fileName, 0);
		}
	}

	return;
}

TArray<FTalkTextInfo> UQuestsFunctionLibrary::FilterTalkTextsForSingleSentence(const TArray<FTalkTextInfo>& ActiveTalkTextsIn, const TArray<FSingleSentenceModeData>& SSDataIn, const ETalkTextMode& AllowedMode, const TArray<EQuestTaskState>& AllowedStates, const TArray<ETalkTextType>& AllowedTypes, const TArray<EDialogType>& AllowedSSTypes, TArray <FSingleSentenceModeData>& SSDataOut)
{
	//Check each talk text
	//If nums are different there is something wrong.
	if (ActiveTalkTextsIn.Num() != SSDataIn.Num())
	{
		SSDataOut.Empty();
		return TArray<FTalkTextInfo>();
	}
	

	TArray<FTalkTextInfo> FilteredTalkTexts = TArray<FTalkTextInfo>();
	TArray <FSingleSentenceModeData> FilteredSSData = TArray <FSingleSentenceModeData>();

	for (int i = 0; i < ActiveTalkTextsIn.Num(); i++)
	{
		//Is of same Talk Text mode?
		if (ActiveTalkTextsIn[i].Mode == AllowedMode)
		{
			//Requires SStype?
			if (AllowedSSTypes.Num() > 0)
			{
				//This talktext type is invalid? Means is not contained. Then I can break
				//This is contained in the parallel data array.
				if (!AllowedSSTypes.Contains(SSDataIn[i].Type))
				{
					//Not contained, i can skip this one.
					continue;
				}
			}

			//Requires states?
			if (AllowedStates.Num() > 0)
			{
				//This talktext state is invalid? Means is not contained. Then I can break
				if (!AllowedStates.Contains(ActiveTalkTextsIn[i].State))
				{
					//Not contained, i can skip this one.
					continue;
				}
			}

			//Require Types?
			if (AllowedStates.Num() > 0)
			{
				//This talktext type is invalid? Means is not contained. Then I can break
				if (!AllowedTypes.Contains(ActiveTalkTextsIn[i].Type))
				{
					//Not contained, i can skip this one.
					continue;
				}
			}

			//If reaches this part, it means that has all checks. Add to array and also keep the SSD correlated.
			FilteredTalkTexts.Add(ActiveTalkTextsIn[i]);
			FilteredSSData.Add(SSDataIn[i]);
		}		
	}

	//OutVars
	SSDataOut = FilteredSSData;
	return FilteredTalkTexts;
}

bool UQuestsFunctionLibrary::AreAllTalkTextsOfType(const TArray<FSingleSentenceModeData>& SSDataIn, EDialogType AllowedSSType)
{
	bool succes = true;
	for (int i = 0; i < SSDataIn.Num(); i++)
	{
		if (SSDataIn[i].Type != AllowedSSType)
		{
			succes = false;
			break;
		}
	}			
	
	return succes;
}
