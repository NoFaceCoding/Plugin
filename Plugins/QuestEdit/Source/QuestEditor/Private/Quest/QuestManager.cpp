//Copyright 2020 Marchetti S. Alfredo I. All Rights Reserved.

#include "Quest/QuestManager.h"
#include "Npc/NPCManager.h"
#include "Runtime/Engine/Classes/GameFramework/Actor.h"
#include "Quest/QuestsFunctionLibrary.h"
#include "Runtime/Engine/Public/Net/UnrealNetwork.h"
#include "Task/BaseQuestTask.h"
#include "Quest/QuestSystemSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "QuestEditor.h"
#include "QuestStats.h"
#include "UObject/Field.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "AssetRegistry/AssetData.h"

UQuestManager::UQuestManager() : Super()
{
	SetIsReplicatedByDefault(true);
	SetAutoActivate(true);
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	CurrentNPC = nullptr;
	// ...
}

void UQuestManager::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	//Repnotifys
	DOREPLIFETIME_CONDITION_NOTIFY(UQuestManager, QuestInventory, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UQuestManager, CompletedQuestsTrack, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UQuestManager, QuestsTrack, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UQuestManager, QuestsFailTrack, COND_OwnerOnly, REPNOTIFY_Always);
}

//--------------------------------Functions Repnotify
void UQuestManager::OnRep_QuestInventory()
{
	if (GetNetMode() != NM_DedicatedServer)
	{
		QuestRepInventoryUpdated.Broadcast();
	}		
}

void UQuestManager::OnRep_CompletedQuestsTrack()
{
	if (GetNetMode() != NM_DedicatedServer) CompletedQuestsTrackRepUpdated.Broadcast();
}

void UQuestManager::OnRep_QuestsTrack()
{
	if (GetNetMode() != NM_DedicatedServer) QuestTrackRepUpdated.Broadcast();
}

void UQuestManager::OnRep_QuestsFailTrack()
{
	if (GetNetMode() != NM_DedicatedServer) QuestFailTrackRepUpdated.Broadcast();
}

//--------------------------------ROC ROS EVENTS
void UQuestManager::ROS_StartQuestManager_Implementation(const TArray<FQuestRep>& QuestInventoryIN, const TArray<FCompletedQuestTrack>& QuestCompletedIN, const TArray<FQuestFailTrack>& QuestFailTrackIN, const TArray<FQuestTrack>& QuestTrackIN,
	const TArray<FQuestRep>& DialogInventoryIN, const TArray<FCompletedQuestTrack>& DialogCompletedIN, const TArray<FQuestFailTrack>& DialogFailTrackIN, const TArray<FQuestTrack>& DialogTrackIN)
{
	StartQuestManagerVariables(QuestInventoryIN, QuestCompletedIN, QuestTrackIN, QuestFailTrackIN, DialogInventoryIN, DialogCompletedIN, DialogFailTrackIN, DialogTrackIN);
	return;
}

bool UQuestManager::ROS_StartQuestManager_Validate(const TArray<FQuestRep>& QuestInventoryIN, const TArray<FCompletedQuestTrack>& QuestCompletedIN, const TArray<FQuestFailTrack>& QuestFailTrackIN, const TArray<FQuestTrack>& QuestTrackIN, 
	const TArray<FQuestRep>& DialogInventoryIN, const TArray<FCompletedQuestTrack>& DialogCompletedIN, const TArray<FQuestFailTrack>& DialogFailTrackIN, const TArray<FQuestTrack>& DialogTrackIN)
{
	return true;
}

void UQuestManager::Client_StartQuestManager_Implementation(const TArray<FQuestRep>& QuestInventoryIN,  const TArray<FCompletedQuestTrack>& QuestCompletedIN, const TArray<FQuestTrack>& QuestTrackIN, const TArray<FQuestFailTrack>& QuestFailTrackIN,
	const TArray<FQuestRep>& DialogInventoryIN, const TArray<FCompletedQuestTrack>& DialogCompletedIN, const TArray<FQuestFailTrack>& DialogFailTrackIN, const TArray<FQuestTrack>& DialogTrackIN)
{
	StartQuestManagerVariables(QuestInventoryIN, QuestCompletedIN, QuestTrackIN, QuestFailTrackIN, DialogInventoryIN, DialogCompletedIN, DialogFailTrackIN, DialogTrackIN);
	return;
}

void UQuestManager::ROS_LoadFromSaveGame_Implementation(const FString& PlayerSaveNameIN)
{
	//Load game on server
	QM_LoadFromSaveGame(PlayerSaveNameIN);
	return;
}

bool UQuestManager::ROS_LoadFromSaveGame_Validate(const FString& PlayerSaveNameIN)
{
	return true;
}

void UQuestManager::ROS_CompleteQuestTalkTask_Implementation(int QuestSlotIn, int TaskIDIn, AActor* NPC)
{
	CompleteQuestTalkTask(QuestSlotIn, TaskIDIn, false, NPC); //This is only called on server for quests, dialogs call function directly
}

bool UQuestManager::ROS_CompleteQuestTalkTask_Validate(int QuestSlotIn, int TaskIDIn, AActor* NPC)
{
	return true;
}

void UQuestManager::ROS_AbandonQuest_Implementation(int QuestSelected)
{
	AbandonQuest(false, QuestSelected);  //The ROS is only for quests, dialogs are client
}

bool UQuestManager::ROS_AbandonQuest_Validate(int QuestSelected)
{
	return true;
}

void UQuestManager::ROS_AcceptQuestDirectly_Implementation(const FQuestRep& QuestRepIn, bool IsDialogIn)
{
	AcceptQuestDirectly(QuestRepIn, IsDialogIn);
}

bool UQuestManager::ROS_AcceptQuestDirectly_Validate(const FQuestRep& QuestRepIn, bool IsDialogIn)
{
	return true;
}

void UQuestManager::Client_AcceptQuestFromStartActor_Implementation(const FQuestRep& QuestRepIn)
{
	Pred_AcceptQuestDirectly(QuestRepIn, false); //No dialogs allowed
}

void UQuestManager::ROS_AcceptQuestFromTalk_Implementation(AActor* NPC, int QuestSlotIn, int TalkTaskIDIn)
{
	AcceptQuestFromTalk(NPC, QuestSlotIn, TalkTaskIDIn, false); //This is only called for quests, dialogs are not server side
}

bool UQuestManager::ROS_AcceptQuestFromTalk_Validate(AActor* NPC, int QuestSlotIn, int TalkTaskIDIn)
{
	return true;
}

void UQuestManager::ROS_NPCReached_Implementation(AActor* NPC)
{
	//Calls dispatcher on server
	OnNPCReached.Broadcast(NPC);
}

bool UQuestManager::ROS_NPCReached_Validate(AActor* NPC)
{
	return true;
}

void UQuestManager::ROS_RemoveCompletedQuestSlot_Implementation(int QuestSlotIn)
{
	RemoveCompletedQuestSlot(QuestSlotIn, false);
}

bool UQuestManager::ROS_RemoveCompletedQuestSlot_Validate(int QuestSlotIn)
{
	return true;
}

void UQuestManager::ROS_RemoveCompletedQuestRep_Implementation(const FQuestRep& QuestRepIn)
{
	RemoveCompletedQuestRep(QuestRepIn, false);
}

bool UQuestManager::ROS_RemoveCompletedQuestRep_Validate(const FQuestRep& QuestRepIn)
{
	return true;
}

void UQuestManager::ROS_PlayerSelectReward_Implementation(const FQuestNode& QuestNodeIn, bool IsDialogIn)
{
	PlayerSelectReward(QuestNodeIn, IsDialogIn);
}

bool UQuestManager::ROS_PlayerSelectReward_Validate(const FQuestNode& QuestNodeIn, bool IsDialogIn)
{
	return true;
}

//--------------------FUNCTIONS----------------------------------------

void UQuestManager::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	//If save is handled independently, save game
	if (HandleSaveAutomatically)
	{
		//Listen save
		if (GetNetMode() == NM_ListenServer || GetNetMode() == NM_Standalone)
		{
			//End play on editor dont do anything
			//if it triggers before destroy overrides with nulls
			//Listen
			if (EndPlayReason == EEndPlayReason::Destroyed)
			{
				//Listen save all variables with overwritting.
				HandleAllTasksSave(LocalObjectsDialogsTrack);		//Dialog
				DestroyAllRemainingBaseTasksObjects(LocalObjectsDialogsTrack);
				HandleAllTasksSave(LocalObjectsQuestsTrack);		//Quest
				DestroyAllRemainingBaseTasksObjects(LocalObjectsQuestsTrack);

				QM_SaveToSaveGame(PlayerSaveName);
			}
		}
		//Server needs to wait client save to avoid desynch.
		//Client save first and tells server to save then to ensure synch
		else if (GetNetMode() == NM_Client)
		{
			//Client save dialogs without overwritte
			QM_SaveNoRepToSaveGame(PlayerSaveName);
		}
		else if (GetNetMode() == NM_DedicatedServer)
		{
			//dedicated save rep data without overwritte
			HandleAllTasksSave(LocalObjectsQuestsTrack);         //Quest
			DestroyAllRemainingBaseTasksObjects(LocalObjectsQuestsTrack);
			QM_SaveRepToSaveGame(PlayerSaveName);
		}
	}	

	//EndPlay
	Super::EndPlay(EndPlayReason);
}

void UQuestManager::BeginPlay()
{	
	Super::BeginPlay();
	//If load is handled independently, load game
	if (HandleLoadAutomatically)
	{
		//Listen loading
		if (GetNetMode() == NM_ListenServer || GetNetMode() == NM_Standalone)
		{
			//Listen just load save
			QM_LoadFromSaveGame(PlayerSaveName);
		}		
		//Server needs to wait client loading to avoid desynch.
		//Client loads first and tells server to load then to ensure synch
		else if (GetNetMode() == NM_Client)
		{
			//Client load game
			QM_LoadFromSaveGame(PlayerSaveName);
			//Calls server to load game also
			ROS_LoadFromSaveGame(PlayerSaveName);
		}		
	}
	//Handle DT loading for players
	if (GetNetMode() != NM_DedicatedServer) HandleQuestDTLoading();
}

void UQuestManager::HandleQuestDTLoading()
{
	if (PreLoadedDatatablePaths.Num() == 0) return;

	// Load the asset registry module
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked< FAssetRegistryModule >(FName("AssetRegistry"));
	IAssetRegistry& AssetRegistry = AssetRegistryModule.Get();

	FString FolderPathLocal = FString();
	TArray <FString> ContentPaths;

	FTopLevelAssetPath BaseClassName = UDataTable::StaticClass()->GetClassPathName();
	
	TSet<FTopLevelAssetPath> DerivedNames;
	TArray<FTopLevelAssetPath> BaseNames;
	TSet<FTopLevelAssetPath> Excluded;
	FARFilter Filter;
	TArray< FAssetData > AssetList;

	//For all paths, try to load datatbles in this folder.
	for (int i = 0; i < PreLoadedDatatablePaths.Num(); i++)
	{
		//--------------clear loop
		ContentPaths.Empty();
		DerivedNames.Empty();
		BaseNames.Empty();
		Excluded.Empty();
		AssetList.Empty();
		Filter.Clear();
		//--------------------------------Path
		FolderPathLocal = PreLoadedDatatablePaths[i];
		ContentPaths.Add(FolderPathLocal);

		AssetRegistry.ScanPathsSynchronous(ContentPaths);

		// Use the asset registry to get the set of all class names deriving from Base	
		BaseNames.Add(BaseClassName);
		AssetRegistry.GetDerivedClassNames(BaseNames, Excluded, DerivedNames);

		Filter.bRecursiveClasses = true;
		if (!FolderPathLocal.IsEmpty())
		{
			Filter.PackagePaths.Add(*FolderPathLocal);
		}
		Filter.bRecursivePaths = true;
		AssetRegistry.GetAssets(Filter, AssetList);

		//Iterate over retrieved blueprint assets
		for (auto const& Asset : AssetList)
		{
			//Path
			FString StringLocal = Asset.ToSoftObjectPath().ToString();
			//Get DT
			UDataTable* DTLocal = UQuestsFunctionLibrary::GetDTFromStringPath(StringLocal);
			//Is valid DT?
			if (DTLocal)
			{
				//Quest?
				if (DTLocal->GetRowStruct() == FQuest::StaticStruct())
				{
					//Add to array
					PreLoadedDatatables.Add(DTLocal);
					UE_LOG(QuestEditorLog, Log, TEXT("UQuestManager::HandleQuestDTLoading - Loaded Quest datatable %s"), *DTLocal->GetFName().ToString());
				}
				//NPC
				else if (DTLocal->GetRowStruct() == FNPCQuestDef::StaticStruct())
				{
					//Add to array
					PreLoadedDatatables.Add(DTLocal);
					UE_LOG(QuestEditorLog, Log, TEXT("UQuestManager::HandleQuestDTLoading - Loaded NPCDef datatable %s"), *DTLocal->GetFName().ToString());
				}
			}
		}
	}

	UE_LOG(QuestEditorLog, Log, TEXT("UQuestManager::HandleQuestDTLoading - All datatables loaded."));
}

void UQuestManager::StartQuestManagerVariables(const TArray<FQuestRep>& QuestInventoryIN, const TArray<FCompletedQuestTrack>& QuestCompletedIN, const TArray<FQuestTrack>& QuestTrackIN, const TArray<FQuestFailTrack>& QuestFailTrackIN, 
	const TArray<FQuestRep>& DialogInventoryIN, const TArray<FCompletedQuestTrack>& DialogCompletedIN, const TArray<FQuestFailTrack>& DialogFailTrackIN, const TArray<FQuestTrack>& DialogTrackIN)
{
	//dialogs only reelevant on client.
	DialogsInventory = DialogInventoryIN;
	CompletedDialogsTrack = DialogCompletedIN;
	DialogsFailTrack = DialogFailTrackIN;
	DialogsTrack = DialogTrackIN;	
	LoadQuestsTrack(true, DialogsInventory, DialogsTrack, LocalObjectsDialogsTrack);

	//Quest
	QuestInventory = QuestInventoryIN;
	CompletedQuestsTrack = QuestCompletedIN;
	QuestsFailTrack = QuestFailTrackIN;
	QuestsTrack = QuestTrackIN;
	LoadQuestsTrack(false, QuestInventory, QuestsTrack, LocalObjectsQuestsTrack);
}

void UQuestManager::EndQuestManager(TArray<FQuestRep>& QuestInventoryOUT, TArray<FCompletedQuestTrack>& QuestCompletedOUT, TArray<FQuestTrack>& QuestTrackOUT, TArray<FQuestFailTrack>& QuestFailTrackOUT,
	TArray<FQuestRep>& DialogInventoryOUT, TArray<FCompletedQuestTrack>& DialogCompletedOUT, TArray<FQuestTrack>& DialogTrackOUT, TArray<FQuestFailTrack>& DialogFailTrackOUT)
{
	//For quest
	HandleAllTasksSave(LocalObjectsQuestsTrack);
	DestroyAllRemainingBaseTasksObjects(LocalObjectsQuestsTrack);

	//Dialog   
	HandleAllTasksSave(LocalObjectsDialogsTrack);
	DestroyAllRemainingBaseTasksObjects(LocalObjectsDialogsTrack);

	//Quest Out Var
	QuestInventoryOUT = QuestInventory;
	QuestCompletedOUT = CompletedQuestsTrack;
	QuestTrackOUT = QuestsTrack;
	QuestFailTrackOUT = QuestsFailTrack;

	//Dialog Out Var
	DialogInventoryOUT = DialogsInventory;
	DialogCompletedOUT = CompletedDialogsTrack;
	DialogTrackOUT = DialogsTrack;
	DialogFailTrackOUT = DialogsFailTrack;
}

void UQuestManager::QM_SetSaveGameName(FString PlayerSaveNameIN)
{
	PlayerSaveName = PlayerSaveNameIN;
}

void UQuestManager::SaveQuestData(FString PlayerSaveNameIN)
{
	//--------------Main save function.
	//Handles saving for any netmode
	//Listen save
	if (GetNetMode() == NM_ListenServer || GetNetMode() == NM_Standalone)
	{
		//Listen save all variables with overwritting.
		HandleAllTasksSave(LocalObjectsDialogsTrack); //dialog data
		HandleAllTasksSave(LocalObjectsQuestsTrack); //Quest data
		QM_SaveToSaveGame(PlayerSaveNameIN);
	}
	//Server needs to wait client loading to avoid desynch.
	//Client saves first and tells server to save then to ensure synch
	else if (GetNetMode() == NM_Client)
	{
		//Client save dialogs without overwritte
		HandleAllTasksSave(LocalObjectsDialogsTrack); //dialog data
		QM_SaveNoRepToSaveGame(PlayerSaveNameIN);
	}
	else if (GetNetMode() == NM_DedicatedServer)
	{
		//dedicated save rep data without overwritte
		HandleAllTasksSave(LocalObjectsQuestsTrack); //Quest data
		QM_SaveRepToSaveGame(PlayerSaveNameIN);
	}
}

void UQuestManager::QM_LoadFromSaveGame(FString PlayerSaveNameIN)
{
	//Save name
	FString SaveName = PlayerSaveNameIN + "QuestData";

	if (UGameplayStatics::DoesSaveGameExist(SaveName, 0))
	{
		//-----------------Synch Load
		// Retrieve and cast the USaveGame object to UMySaveGame.
		if (UQuestSystemSaveGame* LoadedGame = Cast<UQuestSystemSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveName, 0)))
		{
			if (LoadedGame != nullptr)
			{
				StartQuestManagerVariables(LoadedGame->QuestInventorySaved, LoadedGame->CompletedQuestsTrackSaved, LoadedGame->QuestsTrackSaved, LoadedGame->QuestsFailTrackSaved, 
					LoadedGame->DialogsInventorySaved, LoadedGame->CompletedDialogsTrackSaved, LoadedGame->DialogsFailTrackSaved, LoadedGame->DialogsTrackSaved);
				QM_QuestDataLoaded.Broadcast();
			}
		}
	}
}

void UQuestManager::QM_SaveToSaveGame(FString PlayerSaveNameIN)
{
	//Save name
	FString SaveName = PlayerSaveNameIN + "QuestData";

	//-----------------Synch Save
	if (UQuestSystemSaveGame* SaveGameInstance = Cast<UQuestSystemSaveGame>(UGameplayStatics::CreateSaveGameObject(UQuestSystemSaveGame::StaticClass())))
	{					   
		// Set data on the savegame object.
		SaveGameInstance->QuestInventorySaved = QuestInventory;
		SaveGameInstance->QuestsTrackSaved = QuestsTrack;
		SaveGameInstance->QuestsFailTrackSaved = QuestsFailTrack;
		SaveGameInstance->CompletedQuestsTrackSaved = CompletedQuestsTrack;

		SaveGameInstance->DialogsInventorySaved = DialogsInventory;
		SaveGameInstance->DialogsTrackSaved = DialogsTrack;
		SaveGameInstance->DialogsFailTrackSaved = DialogsFailTrack;
		SaveGameInstance->CompletedDialogsTrackSaved = CompletedDialogsTrack;
		
		// Start sync save process.
		UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveName, 0);
		QM_QuestDataSaved.Broadcast();
	}	
}

void UQuestManager::QM_SaveNoRepToSaveGame(FString PlayerSaveNameIN)
{
	//Save name
	FString SaveName = PlayerSaveNameIN + "QuestData";

	UQuestSystemSaveGame* SaveGameInstance = nullptr;
	if (UGameplayStatics::DoesSaveGameExist(SaveName, 0))
	{
		//Load existing savegame object
		SaveGameInstance = Cast<UQuestSystemSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveName, 0));
		if (SaveGameInstance)
		{
			//Set data on the savegame object.
			SaveGameInstance->DialogsInventorySaved = DialogsInventory;
			SaveGameInstance->DialogsTrackSaved = DialogsTrack;
			SaveGameInstance->DialogsFailTrackSaved = DialogsFailTrack;
			SaveGameInstance->CompletedDialogsTrackSaved = CompletedDialogsTrack;

			//Start sync save process.
			UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveName, 0);
			QM_QuestDataSaved.Broadcast();
		}
	}	
	else
	{
		//Create new save object
		SaveGameInstance = Cast<UQuestSystemSaveGame>(UGameplayStatics::CreateSaveGameObject(UQuestSystemSaveGame::StaticClass()));
		if (SaveGameInstance)
		{
			// Set data on the savegame object.
			SaveGameInstance->DialogsInventorySaved = DialogsInventory;
			SaveGameInstance->DialogsTrackSaved = DialogsTrack;
			SaveGameInstance->DialogsFailTrackSaved = DialogsFailTrack;
			SaveGameInstance->CompletedDialogsTrackSaved = CompletedDialogsTrack;

			// Start sync save process.
			UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveName, 0);
			QM_QuestDataSaved.Broadcast();
		}
	}	
}

void UQuestManager::QM_SaveRepToSaveGame(FString PlayerSaveNameIN)
{
	//Save name
	FString SaveName = PlayerSaveNameIN + "QuestData";

	UQuestSystemSaveGame* SaveGameInstance = nullptr;
	if (UGameplayStatics::DoesSaveGameExist(SaveName, 0))
	{
		//Load existing savegame object
		SaveGameInstance = Cast<UQuestSystemSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveName, 0));
		if (SaveGameInstance)
		{
			//Set data on the savegame object.
			SaveGameInstance->QuestInventorySaved = QuestInventory;
			SaveGameInstance->QuestsTrackSaved = QuestsTrack;
			SaveGameInstance->QuestsFailTrackSaved = QuestsFailTrack;
			SaveGameInstance->CompletedQuestsTrackSaved = CompletedQuestsTrack;

			// Start sync save process.
			UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveName, 0);
			QM_QuestDataSaved.Broadcast();
		}
	}
	else
	{
		//Create new save object
		SaveGameInstance = Cast<UQuestSystemSaveGame>(UGameplayStatics::CreateSaveGameObject(UQuestSystemSaveGame::StaticClass()));
		if (SaveGameInstance)
		{
			//Set data on the savegame object.
			SaveGameInstance->QuestInventorySaved = QuestInventory;
			SaveGameInstance->QuestsTrackSaved = QuestsTrack;
			SaveGameInstance->QuestsFailTrackSaved = QuestsFailTrack;
			SaveGameInstance->CompletedQuestsTrackSaved = CompletedQuestsTrack;

			// Start sync save process.
			UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveName, 0);
			QM_QuestDataSaved.Broadcast();
		}
	}
}

void UQuestManager::RewardTaskActivated(int QuestSlotIn, int TaskSlotIn, bool IsDialogIn)
{
	//for now just encapsulates a dispatcher
	OnPlayerRewardActivated.Broadcast(QuestSlotIn, TaskSlotIn, IsDialogIn);
}

void UQuestManager::PlayerSelectReward(const FQuestNode& QuestNodeIn, bool IsDialogIn)
{
	//encapsulate a dispatcher.
	//The reward selections cannot be done with slots, cuz of corrections that result in multiple choosing. The QuestRep is used to avoid any issue.
	OnPlayerSelectedReward.Broadcast(QuestNodeIn, IsDialogIn);
}

void UQuestManager::PlayerHandledRewardSuccesfully(const FQuestNode& QuestNodeIn, bool IsDialogIn)
{
	//if is not task leave
	if (QuestNodeIn.Node.TaskType != EQuestTaskType::Task) return;

	FQuest QuestLocal = UQuestsFunctionLibrary::QuestRepToQuest(QuestNodeIn.Quest);

	//invalid leave
	if (!QuestLocal.Tasks.IsValidIndex(QuestNodeIn.Node.TaskID)) return;
	
	//Get Task 
	FQuestTask LocalTask = QuestLocal.Tasks[QuestNodeIn.Node.TaskID];

	//Notify
	OnRewardTaskCompleted.Broadcast(LocalTask, IsDialogIn);
}

void UQuestManager::Pred_PlayerSelectReward(int QuestSlotIn, int TaskSlotIn, bool IsDialogIn)
{
	//Make task
	FQuestTaskID LocalTaskID = FQuestTaskID();
	LocalTaskID.TaskID = TaskSlotIn;
	LocalTaskID.TaskType = EQuestTaskType::Task;

	//Make node
	FQuestNode LocalNode = FQuestNode();

	if (IsDialogIn)
	{
		//Is valid quest?
		if (!DialogsInventory.IsValidIndex(QuestSlotIn)) return;

		//Set Node
		LocalNode.Quest = DialogsInventory[QuestSlotIn];
		LocalNode.Node = LocalTaskID;
	}
	else
	{
		//Is valid quest?
		if (!QuestInventory.IsValidIndex(QuestSlotIn)) return;

		//Set Node
		LocalNode.Quest = QuestInventory[QuestSlotIn];
		LocalNode.Node = LocalTaskID;
	}
	
	//Client select reward
	PlayerSelectReward(LocalNode, IsDialogIn);

	//no way i can check if the selection was sucessful, must do anyways.
	if (GetNetMode() == NM_Client)
	{
		//Server Select reward
		ROS_PlayerSelectReward(LocalNode, IsDialogIn);
	}
}

TArray<FRewardTabSlotInfo> UQuestManager::GetRetriggerRewardsUIData(bool IsDialogIn, bool IsNPCRewardIn, const FString& LocationNameIn, const FNPCQuestDefRep& NPCDefIn) const
{
	TArray<FRewardTabSlotInfo> RetriggerData = TArray<FRewardTabSlotInfo>();

	if (IsDialogIn)
	{
		//Dialogs
		RetriggerData = GetRetriggerRewardsUIDataWithArrays(IsDialogIn, IsNPCRewardIn, LocationNameIn, NPCDefIn, DialogsTrack);
	}
	else
	{
		//Quest
		RetriggerData = GetRetriggerRewardsUIDataWithArrays(IsDialogIn, IsNPCRewardIn, LocationNameIn, NPCDefIn, QuestsTrack);
	}

	return RetriggerData;
}

TArray<FRewardTabSlotInfo> UQuestManager::GetRetriggerRewardsUIDataWithArrays(bool IsDialogIn, bool IsNPCRewardIn, const FString& LocationNameIn, const FNPCQuestDefRep& NPCDefIn, const TArray<FQuestTrack>& QuestsTrackIN) const
{
	TArray<FRewardTabSlotInfo> RetriggerData = TArray<FRewardTabSlotInfo>();
	FRewardTabSlotInfo RetriggerDataSlot = FRewardTabSlotInfo();

	//Variables for current node
	bool CurrentNodeIsReward = false;
	bool CurrentIsNPCReward = false;
	bool CurrentIsLocationReward = false;
	FNPCQuestDefRep CurrentNPCDef = FNPCQuestDefRep();
	FString CurrentLocationName = FString();

	FQuestNodeID LocalNode = FQuestNodeID();
	
	bool FoundCurrentRetrigger = false;

	//Search inventory for active tasks and for first reward task, and from that same quest get all active reward ones
	for (int i = 0; i < QuestsTrackIN.Num(); i++)
	{
		//Update node
		LocalNode.QuestSlot = i;
		
		//Find reward tasks in the actives
		for (int j = 0; j < QuestsTrackIN[i].ActiveTasks.Num(); j++)
		{
			//Update node
			LocalNode.Node = QuestsTrackIN[i].ActiveTasks[j];

			//Is a task? only tasks can be rewards
			if (QuestsTrackIN[i].ActiveTasks[j].TaskType == EQuestTaskType::Task)
			{
				//Get node data
				CurrentNodeIsReward = GetRewardNodeData(LocalNode, IsDialogIn, QuestsTrackIN, CurrentIsNPCReward, CurrentIsLocationReward, CurrentNPCDef, CurrentLocationName);

				//Is reward
				if (CurrentNodeIsReward)
				{
					//Is an NPC node and same NPC or a location node and same location
					if ((IsNPCRewardIn && CurrentIsNPCReward && CurrentNPCDef == NPCDefIn) || (!IsNPCRewardIn && CurrentIsLocationReward && CurrentLocationName == LocationNameIn))
					{
						//This quest is the retrigger one and will show all its tasks
						FoundCurrentRetrigger = true;

						//Build data for slot
						RetriggerDataSlot.QuestSlot = i;
						RetriggerDataSlot.TaskSlot = QuestsTrackIN[i].ActiveTasks[j].TaskID;
						RetriggerDataSlot.IsDialogTask = IsDialogIn;
						//Get task reward slot					
						RetriggerDataSlot.TaskRewardSlot = UQuestsFunctionLibrary::GetTaskRewardSlot(GetTask(EQuestSelectedType::Active, RetriggerDataSlot.QuestSlot, RetriggerDataSlot.TaskSlot, IsDialogIn));

						//Add to output
						RetriggerData.Add(RetriggerDataSlot);
					}
				}				
			}
		}	

		//If found data no need to continue
		if (FoundCurrentRetrigger)
		{
			break;
		}
	}

	return RetriggerData;
}

TArray<FRewardTabSlotInfo> UQuestManager::GetCurrentRewardsUIData(const TArray <FRewardTabSlotInfo>& ActivatedRewardsIn, bool& IsNPCRewardOut, FString& LocationNameOut, FNPCQuestDefRep& NPCDefOut) const
{
	// ActivatedRewardsIn are the tasks indexes and quest I need, that activated the UI, this means that i can get if it is a location or if it is NPC and get them... etc?
	if (ActivatedRewardsIn.Num() == 0) return TArray<FRewardTabSlotInfo>();
	
	//Tak first reward ID and use that one to compare. Get all rewards associated with that one.
	FQuestNodeID SampleNodeID = FQuestNodeID();
	SampleNodeID.QuestSlot = ActivatedRewardsIn[0].QuestSlot;
	SampleNodeID.Node.TaskID = ActivatedRewardsIn[0].TaskSlot;
	SampleNodeID.Node.TaskType = EQuestTaskType::Task;

	//First node is a NPC or a location Node? For this i need to check and see what does it use.
	FQuestTask TaskLocal = FQuestTask();
	bool SampleIsNPCReward = false;
	bool SampleIsLocationReward = false;
	FNPCQuestDefRep SampleNPCDef = FNPCQuestDefRep();
	FString SampleLocationName = FString();

	//---------------------------------------DETERMINE IF SAMPLE IS NPC OR LOCATION REWARD 
	bool SampledNodeIsReward = false;

	if (ActivatedRewardsIn[0].IsDialogTask)  //Use sample node
	{
		//Dialogs
		SampledNodeIsReward = GetRewardNodeData(SampleNodeID, ActivatedRewardsIn[0].IsDialogTask, DialogsTrack, SampleIsNPCReward, SampleIsLocationReward, SampleNPCDef, SampleLocationName);
	}
	else
	{
		//Quest
		SampledNodeIsReward = GetRewardNodeData(SampleNodeID, ActivatedRewardsIn[0].IsDialogTask, QuestsTrack, SampleIsNPCReward, SampleIsLocationReward, SampleNPCDef, SampleLocationName);
	}				
	
	//If is not reward task stop 
	//if (!SampledNodeIsReward)
	//{
	//	return TArray<FRewardTabSlotInfo>();
	//}
			
	//---------Get all associated rewards with the sampled one. Same mode, questslot and location/NPC def.
	//Variables for current node
	bool CurrentNodeIsReward = false;
	bool CurrentIsNPCReward = false;
	bool CurrentIsLocationReward = false;
	FNPCQuestDefRep CurrentNPCDef = FNPCQuestDefRep();
	FString CurrentLocationName = FString();

	TArray<FRewardTabSlotInfo> OutTabsInfo = TArray<FRewardTabSlotInfo>();
	FRewardTabSlotInfo CurrentRewardInfo = FRewardTabSlotInfo();
	
	//Get Quest for sampled node
	FQuest QuestCurrent = FQuest();

	if (ActivatedRewardsIn[0].IsDialogTask)
	{
		//get dialog
		if (DialogsInventory.IsValidIndex(SampleNodeID.QuestSlot))
		{
			QuestCurrent = UQuestsFunctionLibrary::QuestRepToQuest(DialogsInventory[SampleNodeID.QuestSlot]);
		}		
	}
	else
	{
		//get quest
		if (QuestInventory.IsValidIndex(SampleNodeID.QuestSlot))
		{
			QuestCurrent = UQuestsFunctionLibrary::QuestRepToQuest(QuestInventory[SampleNodeID.QuestSlot]);
		}
	}

	//Create local node because is neded as TaskID
	FQuestNodeID LocalNodeID = FQuestNodeID();
	LocalNodeID.Node.TaskType = EQuestTaskType::Task;  //always task for rewards

	bool IsNPCRewardLocal = false;	//If is not NPC is location
	FString LocationNameLocal = FString();
	FNPCQuestDefRep NPCDefLocal = FNPCQuestDefRep();

	for (int i = 0; i < ActivatedRewardsIn.Num(); i++)
	{
		//This reward is same mode than sample node? I dont mix modes
		if (ActivatedRewardsIn[i].IsDialogTask != ActivatedRewardsIn[0].IsDialogTask) continue; //skip if not same mode

		//Update Local Node
		LocalNodeID.QuestSlot = ActivatedRewardsIn[i].QuestSlot;
		LocalNodeID.Node.TaskID = ActivatedRewardsIn[i].TaskSlot;

		//Get values for current node
		if (ActivatedRewardsIn[i].IsDialogTask)
		{
			CurrentNodeIsReward = GetRewardNodeData(LocalNodeID, ActivatedRewardsIn[i].IsDialogTask, DialogsTrack, CurrentIsNPCReward, CurrentIsLocationReward, CurrentNPCDef, CurrentLocationName);
		}
		else
		{
			CurrentNodeIsReward = GetRewardNodeData(LocalNodeID, ActivatedRewardsIn[i].IsDialogTask, QuestsTrack, CurrentIsNPCReward, CurrentIsLocationReward, CurrentNPCDef, CurrentLocationName);
		}

		//Save the data for retrigger on first loop from the sample node
		if (i == 0)		//sample node is 0
		{
			IsNPCRewardLocal = CurrentIsNPCReward;
			LocationNameLocal = CurrentLocationName;
			NPCDefLocal = CurrentNPCDef;
		}

		//Is valid reward. If is not active for example is not valid anymore.
		if (CurrentNodeIsReward)
		{
			//Same quest? Do not mix quests
			if (SampleNodeID.QuestSlot == ActivatedRewardsIn[i].QuestSlot)
			{
				//Compare current node to sampled node
				//Same location or NPC and type?
				if (SampleIsNPCReward == CurrentIsNPCReward && SampleIsLocationReward == CurrentIsLocationReward && SampleNPCDef == CurrentNPCDef && SampleLocationName == CurrentLocationName)
				{
					//Build data for slot
					CurrentRewardInfo.QuestSlot = ActivatedRewardsIn[i].QuestSlot;
					CurrentRewardInfo.TaskSlot = ActivatedRewardsIn[i].TaskSlot;
					CurrentRewardInfo.IsDialogTask = ActivatedRewardsIn[i].IsDialogTask;
					CurrentRewardInfo.TaskRewardSlot = UQuestsFunctionLibrary::GetTaskRewardSlot(QuestCurrent.Tasks[ActivatedRewardsIn[i].TaskSlot]);

					//Add to out array
					OutTabsInfo.Add(CurrentRewardInfo);
				}
			}
		}	
	}

	//Out vars for retrigger
	IsNPCRewardOut = IsNPCRewardLocal;
	LocationNameOut = LocationNameLocal;
	NPCDefOut = NPCDefLocal;

	return OutTabsInfo;
}

bool UQuestManager::GetRewardNodeData(const FQuestNodeID& NodeIn, bool IsDialogIn, const TArray<FQuestTrack>& QuestsTrackIN, bool& IsNPCRewardOUT, bool& IsLocationRewardOut, FNPCQuestDefRep& NPCDefOut, FString& LocationNameOUT) const
{
	//Variables needed to process
	FQuestTask TaskLocal = FQuestTask();
	bool IsNPCReward = false;
	bool IsLocationReward = false;
	FNPCQuestDefRep SampleNPCDef = FNPCQuestDefRep();
	FString SampleLocationName = FString();

	//---------------------------------------DETERMINE IF SAMPLE IS NPC OR LOCATION REWARD 

	//The sample node is valid in track?
	if (QuestsTrackIN.IsValidIndex(NodeIn.QuestSlot) && QuestsTrackIN[NodeIn.QuestSlot].TasksTracks.IsValidIndex(NodeIn.Node.TaskID))
	{
		//The task is active?
		if (QuestsTrackIN[NodeIn.QuestSlot].ActiveTasks.Contains(NodeIn.Node))
		{
			//Get task struct for the sample one
			TaskLocal = GetTask(EQuestSelectedType::Active, NodeIn.QuestSlot, NodeIn.Node.TaskID, IsDialogIn);
			
			//Determine if is a NPC reward and the sampled NPC.
			IsNPCReward = UQuestsFunctionLibrary::GetTaskRewardGiver(TaskLocal, SampleNPCDef);

			//If the sample is not a NPC reward, then must be a Location reward, but i need to make sure it is a reward.
			if (IsNPCReward)
			{
				//Is a NPC reward ?
				//Update variables OUT
				NPCDefOut = SampleNPCDef;
				LocationNameOUT = FString();  //has no location for npc
				IsNPCRewardOUT = true;
				IsLocationRewardOut = false;
				//it is a reward
				return true;
			}
			//Not NPC Reward
			else
			{
				IsLocationReward = UQuestsFunctionLibrary::GetTaskRewardLocation(TaskLocal, SampleLocationName);

				if (IsLocationReward)
				{
					//Is loaction reward
					//Update variables OUT
					NPCDefOut = FNPCQuestDefRep();   //has no npc for location
					LocationNameOUT = SampleLocationName;
					IsNPCRewardOUT = false;
					IsLocationRewardOut = true;
					//it is a reward
					return true;
				}
			}
		}
	}

	//out variables are false and cleared
	IsNPCRewardOUT = false;
	IsLocationRewardOut = false;
	NPCDefOut = FNPCQuestDefRep();
	LocationNameOUT = FString();
	return false;
}

bool UQuestManager::CanAcceptQuest(const FQuestRep& QuestRepIN, bool IsDirectModeIn, bool IsDialogIn, AActor* NPCIN, bool ShouldPrintErrors) const
{
	bool succes = false;

	if (IsDialogIn)
	{
		//Dialog
		succes = CanAcceptQuestWithArrays(QuestRepIN, IsDirectModeIn, IsDialogIn, NPCIN, ShouldPrintErrors, DialogsInventory, CompletedDialogsTrack, DialogsFailTrack);
	}
	else
	{
		//Quest
		succes = CanAcceptQuestWithArrays(QuestRepIN, IsDirectModeIn, IsDialogIn, NPCIN, ShouldPrintErrors, QuestInventory, CompletedQuestsTrack, QuestsFailTrack);
	}

	return succes;
}

bool UQuestManager::CanAcceptQuestWithArrays(const FQuestRep& QuestRepIN, bool IsDirectModeIn, bool IsDialogIn, AActor* NPCIN, bool ShouldPrintErrors, const TArray<FQuestRep>& QuestsInventoryIN, const TArray<FCompletedQuestTrack>& CompletedQuestsTrackIN, const TArray<FQuestFailTrack>& QuestsFailTrackIN) const
{
	//in progress
	if (IsQuestInProgressWithArrays(QuestRepIN, QuestsInventoryIN))
	{
		if (GetNetMode() != NM_DedicatedServer && ShouldPrintErrors && !IsDirectModeIn) QM_GameplayMessage.Broadcast(FText::FromString("Quest already in progress."));
		return false;
	}

	//Already completed
	if (IsQuestCompletedWithArrays(QuestRepIN, CompletedQuestsTrackIN))
	{
		if (GetNetMode() != NM_DedicatedServer && ShouldPrintErrors && !IsDirectModeIn) QM_GameplayMessage.Broadcast(FText::FromString("You already completed this quest."));
		return false;
	}

	//Is failed?
	if (IsQuestFailedWithArrays(QuestRepIN, IsDialogIn, QuestsFailTrackIN))
	{
		if (GetNetMode() != NM_DedicatedServer && ShouldPrintErrors && !IsDirectModeIn) QM_GameplayMessage.Broadcast(FText::FromString("This quest is failed and cannot be accepted."));
		return false;
	}

	//----------------------------------------------------------------------------------
	//--------------------------Task initial activation
	//----------------------------------------------------------------------------------

	//Get quest
	FQuest QuestLocal = UQuestsFunctionLibrary::QuestRepToQuest(QuestRepIN);
	
	//Must have at least a task that can activate. Talk task can always activate cuz they dont have a activate condition
	bool result = false;
	TArray <int> InitialTasks = TArray <int>();
	if (IsDirectModeIn)
	{
		//Direct mode - Requires at least a initial task that can activate
		InitialTasks = GetStartingTasksSlots(EQuestTaskType::Task, QuestLocal, nullptr, true); //NPC dont care

		for (int i = 0; i < InitialTasks.Num(); i++)
		{
			//Check if one can activate
			if (TaskActivateConditionCheck(QuestLocal, InitialTasks[i]))
			{
				result = true;
				break;
			}
		}

		//If it has no tasks, i will see if there is a talktask that can be activated.
		if (!result)
		{
			//Get all start talk slots for all NPCs
			TArray <int> InitialTalkTasks = GetStartingTasksSlots(EQuestTaskType::TalkTask, QuestLocal, nullptr, true);

			//Is there any talk task that can be preactivated
			if (InitialTalkTasks.Num() > 0)
			{
				result = true;
			}
		}

	}
	else
	{
		//Talk mode - Requires at least a initial talktask for the NPC OR Pseudo talk Task Slot
		InitialTasks = GetStartingTalkTasksSlots(QuestLocal, NPCIN, false);
		result = InitialTasks.Num() > 0;
	}

	return result;
}

bool UQuestManager::AcceptQuestFromTalk(AActor* NPC, int QuestSlotIn, int TalkTaskIDIn, bool IsDialogIN)
{
	bool succes = false;

	if (IsDialogIN)
	{
		//Dialog
		succes = AcceptQuestFromTalkWithArrays(NPC, QuestSlotIn, TalkTaskIDIn, IsDialogIN, DialogsInventory, DialogsTrack, LocalObjectsDialogsTrack);
	}
	else
	{
		//Quest
		succes = AcceptQuestFromTalkWithArrays(NPC, QuestSlotIn, TalkTaskIDIn, IsDialogIN, QuestInventory, QuestsTrack, LocalObjectsQuestsTrack);
	}

	return succes;
}

bool UQuestManager::AcceptQuestFromTalkWithArrays(AActor* NPC, int QuestSlotIn, int TalkTaskIDIn, bool IsDialogIN, TArray<FQuestRep>& QuestsInventoryIN, TArray<FQuestTrack>& QuestsTrackIN, TArray<FQuestObjectTrack>& ObjectsTrackIN)
{
	//get NPC manage
	if (!NPC) return false;
	UNPCManager* NPCManagerComponent;
	NPCManagerComponent = Cast<UNPCManager>(NPC->FindComponentByClass(UNPCManager::StaticClass()));

	//NPC null
	if (!NPCManagerComponent) return false;

	//Quest NPC
	TArray <FQuestRep> NPCQuests = TArray <FQuestRep>(); 
		
	if (IsDialogIN)
	{
		//Get dialogs
		NPCQuests = NPCManagerComponent->GetNPCDialogsRep();
	}
	else
	{
		//Get quests
		NPCQuests = NPCManagerComponent->GetNPCQuestsRep();
	}

	//Quest slot invalid?
	if (!NPCQuests.IsValidIndex(QuestSlotIn))
	{
		return false;
	}

	//Get Quest rep
	FQuestRep QuestRepLocal = NPCQuests[QuestSlotIn];

	//Get Quest	
	FQuest QuestLocal = UQuestsFunctionLibrary::QuestRepToQuest(QuestRepLocal);

	//-----------------------Can accept
	//there is room for quests??
	if (IsDialogIN)
	{
		//Max dialog limit? I clean inventory
		if (QuestsInventoryIN.Num() >= MaxActiveDialogs)
		{
			//Remove first dialog if needed
			RemoveQuestSlot(0, IsDialogIN, QuestsInventoryIN, QuestsTrackIN, ObjectsTrackIN);
			UE_LOG(QuestEditorLog, Log, TEXT("UQuestManager::AcceptQuestFromTalk - Player cannot accept more dialogs. Removed first dialog from Inventory"));
		}
	}
	else
	{
		//Max quest limit?
		if (QuestsInventoryIN.Num() >= MaxActiveQuests)
		{
			if (GetNetMode() != NM_DedicatedServer) QM_GameplayMessage.Broadcast(FText::FromString("You can't accept more quests."));
			UE_LOG(QuestEditorLog, Log, TEXT("UQuestManager::AcceptQuestFromTalk - Player cannot accept more quests. Max limit of active quests reached"));
			return false;
		}
	}
	
	//Can accept?
	if (!CanAcceptQuest(QuestRepLocal, false, IsDialogIN, NPC, true)) return false;

	//----Accept Quest	
	AddQuestToInvFromTalk(QuestRepLocal, IsDialogIN, NPC, QuestSlotIn, TalkTaskIDIn);

	return true;
}

bool UQuestManager::CompleteQuest(int QuestSlotIn, const FQuest& QuestIn, bool IsDialogIN, TArray<FQuestRep>& QuestsInventoryIN, TArray<FQuestTrack>& QuestsTrackIN, TArray<FQuestObjectTrack>& ObjectsTrackIN, TArray<FCompletedQuestTrack>& CompletedQuestsTrackIN)
{
	//valid quest slot?
	if (!QuestsTrackIN.IsValidIndex(QuestSlotIn))
	{
		UE_LOG(QuestEditorLog, Warning, TEXT("UQuestManager::CompleteQuest - Cannot complete quest. Quest Slot %i is invalid"), QuestSlotIn);
		return false;
	}		
	
	//Track update and events
	FQuestTrack  QuestTrackLocal = QuestsTrackIN[QuestSlotIn];
	FQuestRep QuesRepLocal = QuestsInventoryIN[QuestSlotIn];
	FCompletedQuestTrack QTrackLocal;
	QTrackLocal.QuestRep = QuesRepLocal;
	QTrackLocal.QuestTrack = QuestTrackLocal;
	CompletedQuestsTrackIN.Add(QTrackLocal);

	if (IsDialogIN)
	{
		DialogCompleted.Broadcast(QuestIn, CompletedQuestsTrackIN.Num() - 1);
	}
	else
	{
		QuestCompleted.Broadcast(QuestIn, CompletedQuestsTrackIN.Num() - 1);

		//Selected Quest correction for clients
		if (GetNetMode() != NM_DedicatedServer)
		{
			//Is the completed quest the selected one? needs to update to 0
			if (SelectedQuestSlot == QuestSlotIn) SetSelectedQuest(0);
		}
	}

	//remove quest from inv
	RemoveQuestSlot(QuestSlotIn, IsDialogIN, QuestsInventoryIN, QuestsTrackIN, ObjectsTrackIN);

	//Extra talk update. Its needed in some cases 
	if (GetNetMode() != NM_DedicatedServer) OnNPCTalkTextsChanged.Broadcast();

	return true;
}

void UQuestManager::Pred_AcceptQuestDirectly(const FQuestRep& QuestRepIn, bool IsDialogIn)
{
	//Client accept quest
	if (AcceptQuestDirectly(QuestRepIn, IsDialogIn) == true && GetNetMode() == NM_Client)
	{
		//Server accept quest if need
		ROS_AcceptQuestDirectly(QuestRepIn, IsDialogIn);
	}
	//Client adjust selected quest to 0
	SetSelectedQuest(0);
}

bool UQuestManager::RemoveQuestSlot(int Slot, bool IsDialogIn, TArray<FQuestRep>& QuestsInventoryIN, TArray<FQuestTrack>& QuestsTrackIN, TArray<FQuestObjectTrack>& ObjectsTrackIN)
{
	//QuestInvCheck
	if (!QuestsInventoryIN.IsValidIndex(Slot))
	{
		UE_LOG(QuestEditorLog, Warning, TEXT("UQuestManager::RemoveQuestSlot - Cannot remove quest slot %i of quest inventory. Quest Slot is invalid"), Slot);
		return false;
	}
	//Questrack check
	if (!QuestsTrackIN.IsValidIndex(Slot))
	{
		UE_LOG(QuestEditorLog, Warning, TEXT("UQuestManager::RemoveQuestSlot - Cannot remove quest slot %i of quest track. Quest Slot is invalid"), Slot);
		return false;
	}
	
	//SaveQuestForDispatching
	FQuest QuestLocal = UQuestsFunctionLibrary::QuestRepToQuest(QuestsInventoryIN[Slot]);

	//Destroy task Objects 
	DestroyAllRemainingBaseTasksObjectsForQuest(Slot, ObjectsTrackIN);
	
	//----------Update tasks quest slots that are superior to the removed
	ReduceAllQuestSlotsObjectsAfterRemove(Slot, ObjectsTrackIN);

	//Remove Object track
	ObjectsTrackIN.RemoveAt(Slot);

	//Quest track remove
	QuestsTrackIN.RemoveAt(Slot);

	//-------------Remove slot from quest
	//Remove
	QuestsInventoryIN.RemoveAt(Slot);

	//Dispatch event
	if (IsDialogIn)
	{
		DialogRemoved.Broadcast(Slot, QuestLocal);
	}
	else
	{
		QuestRemoved.Broadcast(Slot, QuestLocal);
		if (GetNetMode() != NM_DedicatedServer) Client_QuestRemoved.Broadcast(Slot, QuestLocal);
	}

	return true;
}

bool UQuestManager::CompleteQuestTalkTask(int QuestSlotIn, int TaskIDIn, bool IsDialogIN, AActor * NPC)
{	
	bool succes = false;

	if (IsDialogIN)
	{
		//Dialog
		succes = CompleteQuestTalkTaskWithArrays(QuestSlotIn, TaskIDIn, IsDialogIN, NPC, DialogsInventory, DialogsTrack, LocalObjectsDialogsTrack, CompletedDialogsTrack, DialogsFailTrack);
	}
	else
	{
		//Quest
		succes = CompleteQuestTalkTaskWithArrays(QuestSlotIn, TaskIDIn, IsDialogIN, NPC, QuestInventory, QuestsTrack, LocalObjectsQuestsTrack, CompletedQuestsTrack, QuestsFailTrack);
	}

	return succes;
}

bool UQuestManager::CompleteQuestTalkTaskWithArrays(int QuestSlotIn, int TaskIDIn, bool IsDialogIN, AActor* NPC, TArray<FQuestRep>& QuestsInventoryIN, TArray<FQuestTrack>& QuestsTrackIN, TArray<FQuestObjectTrack>& ObjectsTrackIN, TArray<FCompletedQuestTrack>& CompletedQuestsTrackIN, TArray<FQuestFailTrack>& QuestsFailTrackIN)
{
	//Variables
	if (!QuestsInventoryIN.IsValidIndex(QuestSlotIn))
	{
		UE_LOG(QuestEditorLog, Log, TEXT("UQuestManager::CompleteQuestTalkTaskWithArrays - Quest %i is invalid."), QuestSlotIn);
		return false;
	}

	FQuest QuestLocal = UQuestsFunctionLibrary::QuestRepToQuest(QuestsInventoryIN[QuestSlotIn]);

	//CurrentID
	FQuestTaskID CurrentID = FQuestTaskID();
	CurrentID.TaskID = TaskIDIn;
	CurrentID.TaskType = EQuestTaskType::TalkTask;

	TArray <int> ActiveTasks = TArray <int>();

	//If one node completes quest, there is no need in continuing checking other nodes.
	bool bQuestCompleted = false;

	//------------------------NPC Info.---------------------------		
	//Calculates once
	UNPCManager* NPCManagerComponent;
	NPCManagerComponent = Cast<UNPCManager>(NPC->FindComponentByClass(UNPCManager::StaticClass()));
	//NPC null
	if (!NPCManagerComponent)
	{
		UE_LOG(QuestEditorLog, Warning, TEXT("UQuestManager::CompleteQuestTalkTask - Failed to check tasks. Npc manager is invalid"));
		return false;
	}

	//NPC name
	FNPCQuestDefRep NPCLocal = NPCManagerComponent->GetNPCQuestDefRep();

	//-------------------------Pre activation------------------------------------
	//If talk text is preactivated, it should activate now.
	if (IsTaskPreActive(QuestSlotIn, CurrentID, IsDialogIN))
	{
		//Activate talktask from talktext info
		ActivateTalkTaskFromPreActiveTrack(QuestSlotIn, IsDialogIN, QuestLocal, TaskIDIn, QuestsInventoryIN, QuestsTrackIN, ObjectsTrackIN, CompletedQuestsTrackIN, QuestsFailTrackIN);
	}

	//-------------------------Completion------------------------------------
	//Get Active talk tasks
	ActiveTasks = GetActiveTasksSlotsFromTrackWithArrays(QuestSlotIn, EQuestTaskType::TalkTask, QuestsTrackIN);

	//This task id active?
	if (ActiveTasks.Contains(TaskIDIn))
	{
		//Is this the NPC?
		if (QuestLocal.TalkTasks[TaskIDIn].TalkNPC == NPCLocal)
		{
			//Attemp to complete 
			bQuestCompleted = CompleteNode(QuestSlotIn, IsDialogIN, QuestLocal, CurrentID, QuestsInventoryIN, QuestsTrackIN, ObjectsTrackIN, CompletedQuestsTrackIN, QuestsFailTrackIN);
		}
		else UE_LOG(QuestEditorLog, Error, TEXT("UQuestManager::CompleteQuestTalkTask - Incorrect NPC for this talk task."));
	}

	return bQuestCompleted;
}

bool UQuestManager::AbandonQuestRep(bool IsDialogIN, const FQuestRep& QuestRepIn)
{
	//Find quest slot	
	int SlotLocal = 0;
	bool found = false;

	if (IsDialogIN)
	{
		found = DialogsInventory.Find(QuestRepIn, SlotLocal);
	}
	else
	{
		found = QuestInventory.Find(QuestRepIn, SlotLocal);
	}
	
	//If is not active then needs to cancel.
	if (!found) return false;

	//Abandon slot
	bool succes = AbandonQuest(IsDialogIN, SlotLocal);
	
	return succes;
}

bool UQuestManager::AbandonQuest(bool IsDialogIN, int Slot)
{
	bool succes = false;

	if (IsDialogIN)
	{
		//Dispatch first, while dialog is up
		if (DialogsInventory.IsValidIndex(Slot))
		{
			DialogAbandoned.Broadcast(Slot, UQuestsFunctionLibrary::QuestRepToQuest(DialogsInventory[Slot]));
		}

		//Dialog
		succes = RemoveQuestSlot(Slot, IsDialogIN, DialogsInventory, DialogsTrack, LocalObjectsDialogsTrack);
	}
	else
	{
		//Dispatch first, while quest is up
		if (QuestInventory.IsValidIndex(Slot))
		{
			QuestAbandoned.Broadcast(Slot, UQuestsFunctionLibrary::QuestRepToQuest(QuestInventory[Slot]));
		}

		//Quest
		succes = RemoveQuestSlot(Slot, IsDialogIN, QuestInventory, QuestsTrack, LocalObjectsQuestsTrack);
	}

	return succes;
}

void UQuestManager::Pred_AbandonSelectedQuest()
{
	//Handles client and server logic for abandonning client selected active quest.
	int QSlot = GetSelectedQuestsSlot(EQuestSelectedType::Active);

	//Client abandon quest
	Pred_AbandonQuestBySlot(QSlot);
}

void UQuestManager::Pred_AbandonQuestBySlot(int QuestSlotIn)
{
	//Client abandon quest	
	if (AbandonQuest(false, QuestSlotIn) == true && GetNetMode() == NM_Client)
	{
		//Server abandon quest if need
		ROS_AbandonQuest(QuestSlotIn);
	}

	//Client adjust selected quest to 0
	SetSelectedQuest(0);
}

bool UQuestManager::AcceptQuestDirectly(const FQuestRep& QuestRepIn, bool IsDialogIn)
{
	//if cant accept returns false
	if (!CanAcceptQuest(QuestRepIn, true, IsDialogIn, nullptr, true)) return false;  

	AddQuestToInv(QuestRepIn, true, IsDialogIn);  

	return true;
}

int UQuestManager::AddQuestToInv(const FQuestRep& QuestRepIN, bool DirectModeIn, bool IsDialogIN)
{
	int OutValue = -1;

	if (IsDialogIN)
	{
		//Dialog
		OutValue = AddQuestToInvWithArrays(QuestRepIN, DirectModeIn, IsDialogIN, DialogsInventory, DialogsTrack, LocalObjectsDialogsTrack, CompletedDialogsTrack, DialogsFailTrack);
	}
	else
	{
		//Quest
		OutValue = AddQuestToInvWithArrays(QuestRepIN, DirectModeIn, IsDialogIN, QuestInventory, QuestsTrack, LocalObjectsQuestsTrack, CompletedQuestsTrack, QuestsFailTrack);
	}

	return OutValue;
}

int UQuestManager::AddQuestToInvWithArrays(const FQuestRep& QuestRepIN, bool DirectModeIn, bool IsDialogIN, TArray<FQuestRep>& QuestsInventoryIN, TArray<FQuestTrack>& QuestsTrackIN, TArray<FQuestObjectTrack>& ObjectsTrackIN, TArray<FCompletedQuestTrack>& CompletedQuestsTrackIN, TArray<FQuestFailTrack>& QuestsFailTrackIN)
{
	//Get Quest
	FQuest QuestLocal = UQuestsFunctionLibrary::QuestRepToQuest(QuestRepIN);

	//Quest fail track clearing
	HandleQuestFailTrackOnAccept(QuestRepIN, IsDialogIN, QuestsFailTrackIN);

	//add quest to inv
	QuestsInventoryIN.Add(QuestRepIN);

	//Quests add
	AddQuestTrack(QuestLocal, QuestsTrackIN, ObjectsTrackIN);

	int QuestSlot = -1;

	//------------Dispatchers
	if (IsDialogIN)
	{
		//Get Slot
		QuestSlot = DialogsInventory.Num() - 1;

		//Notify
		DialogAccepted.Broadcast(QuestSlot, QuestLocal);		
	}
	else
	{
		//Get Slot
		QuestSlot = QuestInventory.Num() - 1;

		//Notify
		QuestAccepted.Broadcast(QuestSlot, QuestLocal);

		//Client dispatcher only for ui
		if (GetNetMode() != NM_DedicatedServer)
		{
			Client_QuestAccepted.Broadcast(QuestSlot, QuestLocal);
		}
	}

	//------------------------------------------------------------------------------
	//---------------------------Start logic for tasks
	//------------------------------------------------------------------------------

	//--------------------------------Talk Tasks
	//PreActivate all talk tasks
	PreActivateAllInitialTalkTasks(QuestSlot, QuestLocal, IsDialogIN, QuestsTrackIN);

	//--------------------------Attemp to Activate ALL initials tasks
	//Talk tasks do not activate here, they are only activated via talking to a NPC

	//--------------------------------Tasks
	//----------------Node is completing state
	//I use here a fake node. Just make sure to use the same to deactivate
	FQuestTaskID FakeNode = FQuestTaskID(-100, EQuestTaskType::Task);
	NodeStartHandlingOperation(QuestSlot, FakeNode, QuestsTrackIN);

	//--------------------------Attempt Activate all initials tasks
	TArray <FQuestTaskID> InitialTasksIDS;
	InitialTasksIDS.Empty();
	TArray <int> InitialTasks = GetStartingTasksSlots(EQuestTaskType::Task, QuestLocal, nullptr, true);//NPC is not checked for tasks
	InitialTasksIDS.Empty();
	FQuestTaskID TaskIDLocal;
	TaskIDLocal.TaskType = EQuestTaskType::Task;

	//Convert initials  tasks to IDS
	for (int i = 0; i < InitialTasks.Num(); i++)
	{
		TaskIDLocal.TaskID = InitialTasks[i];
		InitialTasksIDS.Add(TaskIDLocal);
	}

	//Attemp activation of starting nodes.
	AttempToActivateInferiorNodes(QuestSlot, IsDialogIN, QuestLocal, true, FQuestTaskID(), InitialTasksIDS, QuestsInventoryIN, QuestsTrackIN, ObjectsTrackIN, CompletedQuestsTrackIN, QuestsFailTrackIN);

	//Handle quest fail if needed 
	//Via talk does not check for quest fail. It has at least 1 talk active. Also if checked here would fail, because all talk tasks are only preactive
	//This check most of the times should not fail, a quest should not fail upon on activation. But it can happen if the quest is not properly designed or if somehow the task fails on activate event.
	if (DirectModeIn)
	{
		if (QuestShouldFail(QuestSlot, QuestsTrackIN)) HandleQuestFail(QuestSlot, QuestLocal, IsDialogIN, QuestsInventoryIN, QuestsTrackIN, ObjectsTrackIN, QuestsFailTrackIN);
	}

	//----------------Node is completing state
	NodeStopHandlingOperation(QuestSlot, FakeNode, QuestsTrackIN);

	return QuestSlot;
}

bool UQuestManager::AddQuestToInvFromTalk(const FQuestRep & QuestRepIN, bool IsDialogIN, AActor* NPCIN, int QuestSlotIn, int TalkTaskIDIn)
{
	//Add quest to inv, activate all initial stuff etc.
	int QuestSlot = AddQuestToInv(QuestRepIN, false, IsDialogIN);

	//-------------Check first talktext (Talk text de entrada)

	//Set talktext active
	if (GetNetMode() != NM_DedicatedServer)
	{
		//Find and Set TalkText to active
		SetActiveTalkText(GetTalkTextInfoForTalkTask(NPCIN, QuestSlot, TalkTaskIDIn, IsDialogIN));
	}

	//Check talk task with update text
	CompleteQuestTalkTask(QuestSlot, TalkTaskIDIn, IsDialogIN, NPCIN); 

	return true;
}

bool UQuestManager::HandleQuestFail(int QuestSlotIn, const FQuest& QuestIn, bool IsDialogIN, TArray<FQuestRep>& QuestsInventoryIN, TArray<FQuestTrack>& QuestsTrackIN, TArray<FQuestObjectTrack>& ObjectsTrackIN, TArray<FQuestFailTrack>& QuestsFailTrackIN)
{	
	//Log
	UE_LOG(QuestEditorLog, Warning, TEXT("UQuestManager::HandleQuestFail - Quest %s failed."), *QuestIn.QuestName.ToString());

	if (IsDialogIN)
	{
		//Add to fail track if needed
		if (TrackFailDialogs)
		{
			FQuestRep QuestRepLocal = QuestsInventoryIN[QuestSlotIn];
			AddQuestToFailtrack(QuestRepLocal, QuestsFailTrackIN);
		}

		//Quest fail 
		DialogFailed.Broadcast(QuestSlotIn, QuestIn);
	}
	else
	{
		//Add to fail track if needed
		if (TrackFailQuests)
		{
			FQuestRep QuestRepLocal = QuestsInventoryIN[QuestSlotIn];
			AddQuestToFailtrack(QuestRepLocal, QuestsFailTrackIN);
		}

		//Quest fail 
		QuestFailed.Broadcast(QuestSlotIn, QuestIn);
	}
	
	//Remove from inventory
	RemoveQuestSlot(QuestSlotIn, IsDialogIN, QuestsInventoryIN, QuestsTrackIN, ObjectsTrackIN);

	return true;
}

bool UQuestManager::QuestShouldFail(int QuestIN, TArray<FQuestTrack>& QuestsTrackIN)
{
	//no more active tasks fails
	if (!QuestsTrackIN.IsValidIndex(QuestIN))
	{
		//Log
		UE_LOG(QuestEditorLog, Warning, TEXT("UQuestManager::QuestShouldFail - QuestTrack %i is not a valid index."), QuestIN);
		return false;
	}
	
	//If quest is pending completion cannot fail
	if (QuestsTrackIN[QuestIN].bIsPendingCompletion) return false;

	//The quest must have active tasks. If the quest is accepted directly with talk tasks, i wont fail the quest because there is a preactive task.
	return ((QuestsTrackIN[QuestIN].ActiveTasks.Num() + QuestsTrackIN[QuestIN].PreActiveTasks.Num()) <= 0);
}

bool UQuestManager::ForceQuestFail(int QuestSlotIn, bool IsDialogIn, TArray<FQuestRep>& QuestsInventoryIN, TArray<FQuestTrack>& QuestsTrackIN, TArray<FQuestObjectTrack>& ObjectsTrackIN, TArray<FQuestFailTrack>& QuestsFailTrackIN)
{
	if (!QuestsInventoryIN.IsValidIndex(QuestSlotIn)) return false;

	//Get track and quest of quest slot
	FQuestTrack TrackLocal = QuestsTrackIN[QuestSlotIn];
	FQuest QuestLocal = UQuestsFunctionLibrary::QuestRepToQuest(QuestsInventoryIN[QuestSlotIn]);

	//Fail each active node
	for (int i = 0; i < TrackLocal.ActiveTasks.Num(); i++)
	{
		//fail node
		FailNode(QuestSlotIn, QuestLocal, IsDialogIn, TrackLocal.ActiveTasks[i], QuestsTrackIN, ObjectsTrackIN);
	}
	
	//fail quest.
	HandleQuestFail(QuestSlotIn, QuestLocal, IsDialogIn, QuestsInventoryIN, QuestsTrackIN, ObjectsTrackIN, QuestsFailTrackIN);
	
	return true;
}

bool UQuestManager::ForceActiveNodesFailWithException(int QuestSlotIn, bool IsDialogTaskIN, const FQuestTaskID& ExceptionNodeIn)
{
	bool succes = false;

	if (IsDialogTaskIN)
	{
		//Dialog
		succes = ForceActiveNodesFailWithExceptionWithArrays(QuestSlotIn, IsDialogTaskIN, ExceptionNodeIn, DialogsInventory, DialogsTrack, LocalObjectsDialogsTrack);
	}
	else
	{
		//Quest
		succes = ForceActiveNodesFailWithExceptionWithArrays(QuestSlotIn, IsDialogTaskIN, ExceptionNodeIn, QuestInventory, QuestsTrack, LocalObjectsQuestsTrack);
	}

	return succes;
}

bool UQuestManager::ForceActiveNodesFailWithExceptionWithArrays(int QuestSlotIn, bool IsDialogIn, const FQuestTaskID& ExceptionNodeIn, TArray<FQuestRep>& QuestsInventoryIN, TArray<FQuestTrack>& QuestsTrackIN, TArray<FQuestObjectTrack>& ObjectsTrackIN)
{
	//Variables
	if (!QuestsInventoryIN.IsValidIndex(QuestSlotIn))
	{
		UE_LOG(QuestEditorLog, Log, TEXT("UQuestManager::ForceActiveNodesFailWithExceptionWithArrays - Quest %i is invalid."), QuestSlotIn);
		return false;
	}

	FQuest QuestLocal = UQuestsFunctionLibrary::QuestRepToQuest(QuestsInventoryIN[QuestSlotIn]);
	FQuestTrack TrackLocal = QuestsTrackIN[QuestSlotIn];
	TArray <FQuestTaskID> ActiveTasks = TrackLocal.ActiveTasks;

	//Remove exception
	ActiveTasks.Remove(ExceptionNodeIn);

	//Fail all nodes
	for (int i = 0; i < ActiveTasks.Num(); i++)
	{
		//fail node
		FailNode(QuestSlotIn, QuestLocal, IsDialogIn, ActiveTasks[i], QuestsTrackIN, ObjectsTrackIN);
	}

	return true;
}

bool UQuestManager::IsTaskIDValid(int QuestSlotIN, const FQuestTaskID& TaskIDIN, bool IsDialogIn) const
{
	//Get Quest
	FQuest QuestLocal = GetQuestsInSlot(EQuestSelectedType::Active, QuestSlotIN, IsDialogIn);

	//Invalid task ?
	switch (TaskIDIN.TaskType)
	{
	case EQuestTaskType::Task: if (QuestLocal.Tasks.IsValidIndex(TaskIDIN.TaskID)) return true;
		break;
	case EQuestTaskType::TalkTask: if (QuestLocal.TalkTasks.IsValidIndex(TaskIDIN.TaskID)) return true;
		break;
	default:
		break;
	}

	return false;
}

bool UQuestManager::RemoveCompletedQuestSlot(int QuestSlotIn, bool IsDialogIn)
{
	bool succes = false;

	if (IsDialogIn)
	{
		//Is valid slot?
		if (CompletedDialogsTrack.IsValidIndex(QuestSlotIn))
		{
			//Remove slot
			FQuest QuestLocal = UQuestsFunctionLibrary::QuestRepToQuest(CompletedDialogsTrack[QuestSlotIn].QuestRep);
			CompletedDialogsTrack.RemoveAt(QuestSlotIn);
			DialogCompletedRemoved.Broadcast(QuestLocal, QuestSlotIn);
			succes = true;
		}
	}
	else
	{
		//Is valid slot?
		if (CompletedQuestsTrack.IsValidIndex(QuestSlotIn))
		{
			//Remove slot
			FQuest QuestLocal = UQuestsFunctionLibrary::QuestRepToQuest(CompletedQuestsTrack[QuestSlotIn].QuestRep);
			CompletedQuestsTrack.RemoveAt(QuestSlotIn);
			QuestCompletedRemoved.Broadcast(QuestLocal, QuestSlotIn);
			succes = true;
		}
	}

	return succes;
}

bool UQuestManager::RemoveCompletedQuestRep(const FQuestRep& QuestRepIn, bool IsDialogIn)
{
	bool succes = false;

	if (IsDialogIn)
	{
		//search dialog
		for (int i = 0; i < CompletedDialogsTrack.Num(); i++)
		{
			//Same quest?
			if (CompletedDialogsTrack[i].QuestRep == QuestRepIn)
			{
				//Remove quest
				FQuest QuestLocal = UQuestsFunctionLibrary::QuestRepToQuest(CompletedDialogsTrack[i].QuestRep);
				CompletedDialogsTrack.RemoveAt(i);
				DialogCompletedRemoved.Broadcast(QuestLocal, i);
				succes = true;
				break;
			}
		}
	}
	else
	{
		//search quest
		for (int i = 0; i < CompletedQuestsTrack.Num(); i++)
		{
			//Same quest?
			if (CompletedQuestsTrack[i].QuestRep == QuestRepIn)
			{
				//Remove quest
				FQuest QuestLocal = UQuestsFunctionLibrary::QuestRepToQuest(CompletedQuestsTrack[i].QuestRep);
				CompletedQuestsTrack.RemoveAt(i);
				QuestCompletedRemoved.Broadcast(QuestLocal, i);
				succes = true;
				break;
			}
		}
	}

	return succes;
}

void UQuestManager::Pred_RemoveCompletedQuestSlot(int QuestSlotIn)
{
	//Client abandon quest	
	if (RemoveCompletedQuestSlot(QuestSlotIn, false) == true && GetNetMode() == NM_Client) //The predicted version is for quests only
	{
		//Server abandon quest if need
		ROS_RemoveCompletedQuestSlot(QuestSlotIn);
	}
}

void UQuestManager::Pred_RemoveCompletedQuestRep(const FQuestRep& QuestRepIn)
{
	//Client abandon quest	
	if (RemoveCompletedQuestRep(QuestRepIn, false) == true && GetNetMode() == NM_Client)
	{
		//Server abandon quest if need
		ROS_RemoveCompletedQuestRep(QuestRepIn);
	}
}

bool UQuestManager::IsQuestInProgress(bool IsDialogIn, const FQuestRep& QuestRepIn) const
{
	bool succes = false;

	if (IsDialogIn)
	{
		//Dialog
		succes = IsQuestInProgressWithArrays(QuestRepIn, DialogsInventory);
	}
	else
	{
		//Quest
		succes = IsQuestInProgressWithArrays(QuestRepIn, QuestInventory);
	}

	return succes;
}

bool UQuestManager::IsQuestInProgressWithArrays(const FQuestRep& QuestRepIn, const TArray<FQuestRep>& QuestsInventoryIN) const
{
	return QuestsInventoryIN.Contains(QuestRepIn);
}

bool UQuestManager::IsQuestCompleted(bool IsDialogIn, const FQuestRep& QuestRepIN) const
{
	bool succes = false;

	if (IsDialogIn)
	{
		//Dialog
		succes = IsQuestCompletedWithArrays(QuestRepIN, CompletedDialogsTrack);
	}
	else
	{
		//Quest
		succes = IsQuestCompletedWithArrays(QuestRepIN, CompletedQuestsTrack);
	}

	return succes;
}

bool UQuestManager::IsQuestCompletedWithArrays(const FQuestRep& QuestRepIN, const TArray<FCompletedQuestTrack>& CompletedQuestsTrackIN) const
{
	for (int i = 0; i < CompletedQuestsTrackIN.Num(); i++)
	{
		//Same quest name and path?
		if (CompletedQuestsTrackIN[i].QuestRep.QuestDTName == QuestRepIN.QuestDTName && CompletedQuestsTrackIN[i].QuestRep.Datatable == QuestRepIN.Datatable) return true;
	}
	return false;
}

bool UQuestManager::IsQuestCompletedWithEnding(bool IsDialogIn, const FQuestNode& QuestEndIn) const
{
	bool succes = false;

	if (IsDialogIn)
	{
		//Dialog
		succes = IsQuestCompletedWithEndingWithArrays(QuestEndIn, CompletedDialogsTrack);
	}
	else
	{
		//Quest
		succes = IsQuestCompletedWithEndingWithArrays(QuestEndIn, CompletedQuestsTrack);
	}

	return succes;
}

bool UQuestManager::IsQuestCompletedWithEndingWithArrays(const FQuestNode& QuestEndIn, const TArray<FCompletedQuestTrack>& CompletedQuestsTrackIN) const
{
	//If quest is not completed return false
	if (!IsQuestCompletedWithArrays(QuestEndIn.Quest, CompletedQuestsTrackIN)) return false;

	bool succes = false;

	for (int i = 0; i < CompletedQuestsTrackIN.Num(); i++)
	{
		//Is this quest?
		if (CompletedQuestsTrackIN[i].QuestRep == QuestEndIn.Quest)
		{
			switch (QuestEndIn.Node.TaskType)
			{
			case EQuestTaskType::Task:
			{
				//valid task?
				if (CompletedQuestsTrackIN[i].QuestTrack.TasksTracks.IsValidIndex(QuestEndIn.Node.TaskID))
				{
					succes = CompletedQuestsTrackIN[i].QuestTrack.TasksTracks[QuestEndIn.Node.TaskID].State == EQuestTaskState::Completed;
				}
			}
			break;
			case EQuestTaskType::TalkTask:
			{
				//valid task?
				if (CompletedQuestsTrackIN[i].QuestTrack.TalkTasksTracks.IsValidIndex(QuestEndIn.Node.TaskID))
				{
					succes = CompletedQuestsTrackIN[i].QuestTrack.TalkTasksTracks[QuestEndIn.Node.TaskID].State == EQuestTaskState::Completed;
				}
			}
			break;
			default:
				break;
			}
		}
	}
	return succes;
}

EQuestTaskState UQuestManager::GetQuestState(bool IsDialogIn, const FQuestRep& QuestIn) const
{
	if (IsQuestInProgress(IsDialogIn, QuestIn)) return EQuestTaskState::Active;
	else if (IsQuestCompleted(IsDialogIn, QuestIn)) return EQuestTaskState::Completed; 
	else return EQuestTaskState::Locked;
}

TArray<int> UQuestManager::GetTasksSlotsByState(int QuestSlotIN, EQuestTaskType TaskTypeIN, bool IsDialogIn, EQuestTaskState StateIN) const
{
	TArray<int> MatchingObjectives;
	MatchingObjectives.Empty();

	if (IsDialogIn)
	{
		if (!DialogsTrack.IsValidIndex(QuestSlotIN)) return MatchingObjectives;

		switch (TaskTypeIN)
		{
		case EQuestTaskType::Task:

			for (int i = 0; i < DialogsTrack[QuestSlotIN].TasksTracks.Num(); i++)
			{
				//matches state?
				if (DialogsTrack[QuestSlotIN].TasksTracks[i].State == StateIN)MatchingObjectives.Add(i);
			}
			break;
		case EQuestTaskType::TalkTask:
			for (int i = 0; i < DialogsTrack[QuestSlotIN].TalkTasksTracks.Num(); i++)
			{
				//matches state?
				if (DialogsTrack[QuestSlotIN].TalkTasksTracks[i].State == StateIN)MatchingObjectives.Add(i);
			}
			break;
		default:
			break;
		}
	}
	else
	{
		if (!QuestsTrack.IsValidIndex(QuestSlotIN)) return MatchingObjectives;

		switch (TaskTypeIN)
		{
		case EQuestTaskType::Task:

			for (int i = 0; i < QuestsTrack[QuestSlotIN].TasksTracks.Num(); i++)
			{
				//matches state?
				if (QuestsTrack[QuestSlotIN].TasksTracks[i].State == StateIN)MatchingObjectives.Add(i);
			}
			break;
		case EQuestTaskType::TalkTask:
			for (int i = 0; i < QuestsTrack[QuestSlotIN].TalkTasksTracks.Num(); i++)
			{
				//matches state?
				if (QuestsTrack[QuestSlotIN].TalkTasksTracks[i].State == StateIN)MatchingObjectives.Add(i);
			}
			break;
		default:
			break;
		}
	}

	return MatchingObjectives;
}

TArray<FQuestTask> UQuestManager::GetTasksByState(int QuestSlotIN, bool IsDialogIn, EQuestTaskState StateIN) const
{
	TArray<FQuestTask> MatchingObjectives = TArray<FQuestTask>();
	FQuest QuestLocal = FQuest();

	if (IsDialogIn)
	{
		//Dialog
		if (!DialogsTrack.IsValidIndex(QuestSlotIN)) return MatchingObjectives;
		
		for (int i = 0; i < DialogsTrack[QuestSlotIN].TasksTracks.Num(); i++)
		{
			//matches state?
			if (DialogsTrack[QuestSlotIN].TasksTracks[i].State == StateIN)
			{
				QuestLocal = UQuestsFunctionLibrary::QuestRepToQuest(DialogsInventory[QuestSlotIN]);
				MatchingObjectives.Add(QuestLocal.Tasks[i]);
			}	
		}
	}
	else
	{
		//Quests
		if (!QuestsTrack.IsValidIndex(QuestSlotIN)) return MatchingObjectives;

		for (int i = 0; i < QuestsTrack[QuestSlotIN].TasksTracks.Num(); i++)
		{
			//matches state?
			if (QuestsTrack[QuestSlotIN].TasksTracks[i].State == StateIN)
			{
				QuestLocal = UQuestsFunctionLibrary::QuestRepToQuest(QuestInventory[QuestSlotIN]);
				MatchingObjectives.Add(QuestLocal.Tasks[i]);
			}
		}
	}

	return MatchingObjectives;
}

TArray<FQuestTalkTask> UQuestManager::GetTalkTasksByState(int QuestSlotIN, bool IsDialogIn, EQuestTaskState StateIN) const
{
	TArray<FQuestTalkTask> MatchingObjectives = TArray<FQuestTalkTask>();
	FQuest QuestLocal = FQuest();

	if (IsDialogIn)
	{
		//Dialog
		if (!DialogsTrack.IsValidIndex(QuestSlotIN)) return MatchingObjectives;

		for (int i = 0; i < DialogsTrack[QuestSlotIN].TalkTasksTracks.Num(); i++)
		{
			//matches state?
			if (DialogsTrack[QuestSlotIN].TalkTasksTracks[i].State == StateIN)
			{
				QuestLocal = UQuestsFunctionLibrary::QuestRepToQuest(DialogsInventory[QuestSlotIN]);
				MatchingObjectives.Add(QuestLocal.TalkTasks[i]);
			}
		}
	}
	else
	{
		//Quests
		if (!QuestsTrack.IsValidIndex(QuestSlotIN)) return MatchingObjectives;

		for (int i = 0; i < QuestsTrack[QuestSlotIN].TalkTasksTracks.Num(); i++)
		{
			//matches state?
			if (QuestsTrack[QuestSlotIN].TalkTasksTracks[i].State == StateIN)
			{
				QuestLocal = UQuestsFunctionLibrary::QuestRepToQuest(QuestInventory[QuestSlotIN]);
				MatchingObjectives.Add(QuestLocal.TalkTasks[i]);
			}
		}
	}

	return MatchingObjectives;
}

bool UQuestManager::IsTaskPreActive(int QuestSlotIn, FQuestTaskID TaskIDIn, bool IsDialogIn) const
{	
	if (IsDialogIn)
	{
		//Return wether or not is contained in the preactive track
		if (DialogsTrack.IsValidIndex(QuestSlotIn))
		{
			return DialogsTrack[QuestSlotIn].PreActiveTasks.Contains(TaskIDIn);
		}
	}
	else
	{
		//Return wether or not is contained in the preactive track
		if (QuestsTrack.IsValidIndex(QuestSlotIn))
		{
			return QuestsTrack[QuestSlotIn].PreActiveTasks.Contains(TaskIDIn);
		}
	}

	return false;
}

EQuestTaskState UQuestManager::GetNodeStateInActiveTrack(const FQuestRep& QuestRepIn, const FQuestTaskID& NodeIn, bool IsDialogIn) const
{
	int32 position = -1;
	FQuestTrack Track = FQuestTrack();

	if (IsDialogIn)
	{
		//Find position
		position = DialogsInventory.Find(QuestRepIn);

		//If didnt find, return -1
		if (position == -1) return EQuestTaskState::Locked;

		//This should never be invalid but check anyways
		if (!DialogsTrack.IsValidIndex(position)) return EQuestTaskState::Locked;

		Track = DialogsTrack[position];
	}
	else
	{
		//Find position
		position = QuestInventory.Find(QuestRepIn);
		
		//If didnt find, return -1
		if (position == -1) return EQuestTaskState::Locked;

		//This should never be invalid but check anyways
		if (!QuestsTrack.IsValidIndex(position)) return EQuestTaskState::Locked;

		Track = QuestsTrack[position];
	}

	return GetNodeStateFromTrackStruct(NodeIn, Track);
}

EQuestTaskState UQuestManager::GetNodeStateInCompletedTrack(bool IsDialogIn, const FQuestRep& QuestRepIn, const FQuestTaskID& NodeIn) const
{
	FQuestTrack Track = GetCompletedQuestTrackForQuest(IsDialogIn, QuestRepIn);
	
	return GetNodeStateFromTrackStruct(NodeIn, Track);
}

FQuestTrack UQuestManager::GetCompletedQuestTrackForQuest(bool IsDialogIn, const FQuestRep& QuestRepIn) const
{
	FQuestTrack Track = FQuestTrack();

	if (IsDialogIn)
	{
		//Quest
		for (int i = 0; i < CompletedDialogsTrack.Num(); i++)
		{
			if (CompletedDialogsTrack[i].QuestRep == QuestRepIn)
			{
				Track = CompletedDialogsTrack[i].QuestTrack;
				break;
			}
		}
	}
	else
	{
		//Quest
		for (int i = 0; i < CompletedQuestsTrack.Num(); i++)
		{
			if (CompletedQuestsTrack[i].QuestRep == QuestRepIn)
			{
				Track = CompletedQuestsTrack[i].QuestTrack;
				break;
			}
		}
	}	

	return Track;
}

FQuestTrack UQuestManager::GetActiveQuestTrackForQuest(bool IsDialogIn, const FQuestRep& QuestRepIn) const
{
	//search quest in inventory and save track if found
	FQuestTrack Track = FQuestTrack();

	if (IsDialogIn)
	{
		//Dialog
		for (int i = 0; i < DialogsInventory.Num(); i++)
		{
			if (DialogsInventory[i] == QuestRepIn)
			{
				Track =DialogsTrack[i];
				break;
			}
		}
	}
	else
	{
		//Quest
		for (int i = 0; i < QuestInventory.Num(); i++)
		{
			if (QuestInventory[i] == QuestRepIn)
			{
				Track = QuestsTrack[i];
				break;
			}
		}
	}	

	return Track;
}

FQuestTrack UQuestManager::GetQuestTrackForQuest(EQuestSelectedType SelectType, bool IsDialogIn, const FQuestRep& QuestRepIn) const
{
	FQuestTrack Track = FQuestTrack();
	switch (SelectType)
	{
	case EQuestSelectedType::Active: Track = GetActiveQuestTrackForQuest(IsDialogIn, QuestRepIn);
		break;
	case EQuestSelectedType::Completed: Track = GetCompletedQuestTrackForQuest(IsDialogIn, QuestRepIn);
		break;
	default:
		break;
	}

	return Track;
}

EQuestTaskState UQuestManager::GetNodeStateFromTrackStruct(const FQuestTaskID& NodeIn, const FQuestTrack& QuestTrackIn) const
{
	EQuestTaskState NodeState = EQuestTaskState::Locked;
	switch (NodeIn.TaskType)
	{
	case EQuestTaskType::Task:
	{
		if (!QuestTrackIn.TasksTracks.IsValidIndex(NodeIn.TaskID))break;
		NodeState = QuestTrackIn.TasksTracks[NodeIn.TaskID].State;
	}
	break;
	case EQuestTaskType::TalkTask:
	{
		if (!QuestTrackIn.TalkTasksTracks.IsValidIndex(NodeIn.TaskID))break;
		NodeState = QuestTrackIn.TalkTasksTracks[NodeIn.TaskID].State;
	}
	break;
	default:
		break;
	}
	return NodeState;
}

FQuest UQuestManager::GetCompletedQuestBySlot(int Slot, bool IsDialogIn) const
{
	FQuest QuestOut;

	if (IsDialogIn)
	{
		//Invalid slot?
		if (!CompletedDialogsTrack.IsValidIndex(Slot)) return QuestOut;
		
		QuestOut = UQuestsFunctionLibrary::QuestRepToQuest(CompletedDialogsTrack[Slot].QuestRep);		
	}
	else
	{
		//Invalid slot?
		if (!CompletedQuestsTrack.IsValidIndex(Slot)) return QuestOut;

		QuestOut = UQuestsFunctionLibrary::QuestRepToQuest(CompletedQuestsTrack[Slot].QuestRep);
	}	
	
	return QuestOut;
}

int UQuestManager::GetQuestTalkTaskScore() const
{
	return TalkTaskScore;
}

int UQuestManager::GetQuestTaskScore(EQuestSelectedType SelectType, int QuestSlotIn, int TaskSlotIn, bool IsDialogIn) const
{
	//-------------------------Objects that are not spawned
	//Obtain quest
	FQuest QuestLocal = GetQuestsInSlot(SelectType, QuestSlotIn, IsDialogIn);

	if (IsDialogIn)
	{
		return GetQuestTaskScoreFromObjectTrack(SelectType, QuestSlotIn, QuestLocal, TaskSlotIn, LocalObjectsDialogsTrack);
	}
	else
	{
		return GetQuestTaskScoreFromObjectTrack(SelectType, QuestSlotIn, QuestLocal, TaskSlotIn, LocalObjectsQuestsTrack);
	}
	
	return 0;
}

int UQuestManager::GetQuestTaskScoreFromObjectTrack(EQuestSelectedType SelectType, int QuestSlotIn, const FQuest& QuestIn, int TaskSlotIn, const TArray<FQuestObjectTrack>& ObjectsTrackIN) const
{
	//If object already exist do not spawn
	if (!ObjectsTrackIN.IsValidIndex(QuestSlotIn)) return 0;
	if (!ObjectsTrackIN[QuestSlotIn].ObjectTasksTrack.IsValidIndex(TaskSlotIn))return 0;

	//If already is active object, gets score.
	if (ObjectsTrackIN[QuestSlotIn].ObjectTasksTrack[TaskSlotIn].BaseTaskReference != nullptr && IsValid(ObjectsTrackIN[QuestSlotIn].ObjectTasksTrack[TaskSlotIn].BaseTaskReference))
		return ObjectsTrackIN[QuestSlotIn].ObjectTasksTrack[TaskSlotIn].BaseTaskReference->GetParticularTaskScore();

	//-------------------------Objects that are not spawned
	//Obtain payload
	FQuestTaskPayload PayloadLocal = QuestIn.Tasks[TaskSlotIn].Payload;

	//Obtain class to spawn
	TSoftClassPtr<UBaseQuestTask> TaskClassLocal = QuestIn.Tasks[TaskSlotIn].TaskClass;

	//Class null returns false
	if (TaskClassLocal == nullptr) return false;

	//Load class from soft ref
	TSubclassOf <UBaseQuestTask> LoadedClass;
	LoadedClass = TaskClassLocal.LoadSynchronous();

	//Spawn base task
	UBaseQuestTask* MyBaseQuestTask = NewObject<UBaseQuestTask>(GetOwner(), LoadedClass.Get());

	//if invalid return
	if (!MyBaseQuestTask) return false;

	//Update track reference
	//Set task payload just in case is needed for task calculation
	MyBaseQuestTask->InitialPayload = PayloadLocal;

	//Start base task functions
	return MyBaseQuestTask->GetParticularTaskScore();
}

int UQuestManager::GetQuestScore(EQuestSelectedType SelectType, int QuestSlotIn, bool IsDialogIn) const
{	
	//Quest track
	FQuestTrack TrackLocal = GetQuestTrackInSlot(SelectType, QuestSlotIn, IsDialogIn);

	int Score = 0;

	//TalkTasks
	for (int i = 0; i < TrackLocal.TalkTasksTracks.Num(); i++)
	{
		//if completed add to score.
		if (TrackLocal.TalkTasksTracks[i].State == EQuestTaskState::Completed) Score = Score + TalkTaskScore;
	}

	//Tasks
	for (int i = 0; i < TrackLocal.TasksTracks.Num(); i++)
	{
		//if completed add to score.
		if (TrackLocal.TasksTracks[i].State == EQuestTaskState::Completed) Score = Score + GetQuestTaskScore(SelectType, QuestSlotIn, i, IsDialogIn);
	}

	return Score;
}

int UQuestManager::GetQuestTotalScore(EQuestSelectedType SelectType, int QuestSlotIn, bool IsDialogIn) const
{
	//Quest track
	FQuestTrack TrackLocal = GetQuestTrackInSlot(SelectType, QuestSlotIn, IsDialogIn);

	int Score = 0;

	//TalkTasks
	for (int i = 0; i < TrackLocal.TalkTasksTracks.Num(); i++)
	{
		//Always add score
		Score = Score + TalkTaskScore;
	}

	//Tasks
	for (int i = 0; i < TrackLocal.TasksTracks.Num(); i++)
	{
		//Always add score
		Score = Score + GetQuestTaskScore(SelectType, QuestSlotIn, i, IsDialogIn);
	}

	return Score;
}

TArray<FString> UQuestManager::GetQuestTaskCustomPayloadData(EQuestSelectedType SelectType, int QuestSlotIn, int TaskSlotIn, bool IsDialogIn) const
{
	//Quest track
	FQuest QuestLocal = GetQuestsInSlot(SelectType, QuestSlotIn, IsDialogIn);

	TArray<FString> DataLocal = TArray<FString>();

	if (QuestLocal.Tasks.IsValidIndex(TaskSlotIn))
	{
		DataLocal = QuestLocal.Tasks[TaskSlotIn].CustomPayloadData;
	}

	return DataLocal;
}

FString UQuestManager::GetQuestTaskCustomPayloadDataByIndex(EQuestSelectedType SelectType, int QuestSlotIn, int TaskSlotIn, int CustomPayloadSlotIn, bool IsDialogIn) const
{	
	TArray<FString> DataLocal = GetQuestTaskCustomPayloadData(SelectType, QuestSlotIn, TaskSlotIn, IsDialogIn);

	FString DataOut = FString();

	if (DataLocal.IsValidIndex(CustomPayloadSlotIn)) DataOut = DataLocal[CustomPayloadSlotIn];
	
	return DataOut;
}

bool UQuestManager::NPCGivesQuest(AActor* NPCIn) const
{
	//variables
	bool givesquests = false;
	if (NPCIn == nullptr) return false;

	//---------------------------VARIABLES 	
	//get NPC manager
	UNPCManager* NPCManagerComponent;
	NPCManagerComponent = Cast<UNPCManager>(NPCIn->FindComponentByClass(UNPCManager::StaticClass()));
	//NPC null
	if (!NPCManagerComponent) return false;

	//Quest NPC Variables, all in local vars
	TArray <FQuestRep> NPCContainedQuestsRep = NPCManagerComponent->GetNPCQuestsRep();
	
	//--------------------------------------------------------------------
	//--------------------------------------NEW QUESTS--------------------
	//--------------------------------------------------------------------

	int FirstTalkText = -1;

	//Quests from npc.(CAn or not be accepted for the player) With this ones you accept quests
	for (int i = 0; i < NPCContainedQuestsRep.Num(); i++)
	{
		//is a quest that can be accepted?
		if (CanAcceptQuest(NPCContainedQuestsRep[i], false, false, NPCIn, false))  //This is only for quests, no dialogs
		{
			givesquests = true;
			break;

		}
	}
	return givesquests;
}

TArray<FQuestRep> UQuestManager::GetNPCAvailableQuests(AActor* NPCIn) const
{
	//-----------------------Get all npc texts
	FTalkTextInfo TalkInfoLocal;
	TArray <FQuestRep> AvailableQuestsOut = TArray <FQuestRep>();

	if (NPCIn == nullptr) return AvailableQuestsOut;

	//---------------------------VARIABLES 	
	//NPC manager
	UNPCManager* NPCManagerComponent;
	NPCManagerComponent = Cast<UNPCManager>(NPCIn->FindComponentByClass(UNPCManager::StaticClass()));
	//NPC null
	if (!NPCManagerComponent) return AvailableQuestsOut;

	//Questrep inv
	TArray <FQuestRep> NPCContainedQuestsRep = NPCManagerComponent->GetNPCQuestsRep();

	//--------------------------------------------------------------------
	//--------------------------------------NEW QUESTS--------------------
	//--------------------------------------------------------------------
	//Quests that can be accepted

	TArray <int> FirstTalkText;
	FirstTalkText.Empty();

	//Quests from vendor
	for (int i = 0; i < NPCContainedQuestsRep.Num(); i++)
	{
		//can accept this quest?
		if (CanAcceptQuest(NPCContainedQuestsRep[i], false, false, NPCIn, false))   //This is only for quest no dialogs
		{
			AvailableQuestsOut.Add(NPCContainedQuestsRep[i]);
		}
	}

	return AvailableQuestsOut;
}

TArray<int> UQuestManager::GetNPCActiveQuestSlots(AActor* NPCIn) const
{
	TArray<FTalkTextInfo> TalkTexts = GetTalkTextsInfo(NPCIn);

	TArray<int> ActiveQuestSlots = TArray<int>();

	for (int i = 0; i < TalkTexts.Num(); i++)
	{
		//get quests only
		if (TalkTexts[i].Mode == ETalkTextMode::Quest)
		{
			//Must be active or preactive or completed. Cannot be a Quest give or dialog
			if (TalkTexts[i].Type == ETalkTextType::PreActivateTalkTask || TalkTexts[i].Type == ETalkTextType::TalkTask)
			{
				//Add quest slot unique
				ActiveQuestSlots.AddUnique(TalkTexts[i].QuestSlot);
			}
		}		
	}

	return ActiveQuestSlots;
}

TArray<FTalkTextInfo> UQuestManager::GetTalkTextsInfoForQuestSlot(AActor* NPCIN, int QuestSlotIn) const   //Ths function is only used for some cases for searching talktexts of quest, not used for dialogs
{
	//-----------------------Get all npc texts
	FTalkTextInfo TalkInfoLocal;
	TArray <FTalkTextInfo> AllTalkInfoLocalOUT;
	AllTalkInfoLocalOUT.Empty();
	if (NPCIN == nullptr) return AllTalkInfoLocalOUT;

	//---------------------------VARIABLES 	
	//NPC manager
	UNPCManager* NPCManagerComponent;
	NPCManagerComponent = Cast<UNPCManager>(NPCIN->FindComponentByClass(UNPCManager::StaticClass()));
	//NPC null
	if (!NPCManagerComponent) return AllTalkInfoLocalOUT;

	//Quest NPC Variables, all in local vars	
	FNPCQuestDefRep NPCLocal = NPCManagerComponent->GetNPCQuestDefRep();

	//ALL Quest inventory active talk tasks.
	if (!QuestInventory.IsValidIndex(QuestSlotIn)) return TArray <FTalkTextInfo>(); //This function is only for quests not dialogs

	//Get local quest in slot
	FQuest QuestLocal = UQuestsFunctionLibrary::QuestRepToQuest(QuestInventory[QuestSlotIn]); //This function is only for quests not dialogs

	//----------------------------------Quest PRE active---------------
	AllTalkInfoLocalOUT.Append(GetPreActiveTalkTextsOfQuest(QuestSlotIn, false, QuestLocal, NPCLocal, QuestsTrack));	//This function is only for quests not dialogs

	//----------------------------------Quest active-------------------	
	AllTalkInfoLocalOUT.Append(GetActiveTalkTextsOfQuest(QuestSlotIn, false, QuestLocal, NPCLocal, QuestsTrack)); //This function is only for quests not dialogs

	return AllTalkInfoLocalOUT;
}

TArray<int> UQuestManager::GetQuestsSlotsInvolvingNPCTalk(AActor* NPCIn, bool GetFirstOneOnly) const
{	
	//NPC
	TArray<int> QuestSlots;
	QuestSlots.Empty();
	
	//Invalid?
	if (!NPCIn) return QuestSlots;

	//------------------------NPC Info---------------------------
	UNPCManager* NPCManagerComponent;
	NPCManagerComponent = Cast<UNPCManager>(NPCIn->FindComponentByClass(UNPCManager::StaticClass()));

	//NPC null
	if (!NPCManagerComponent)
	{
		return QuestSlots;
	}

	//NPC name
	FNPCQuestDefRep NPCLocal = NPCManagerComponent->GetNPCQuestDefRep();

	//Variables
	TArray <FQuest> QuestInv = GetQuestsInventory();
	TArray <int> ActiveTasks;

	bool shouldbreak = false;

	//quests Analisis
	for (int i = 0; i < QuestInv.Num(); i++)
	{
		//-------------------------------------------NPC 
		//-------------------talk tasks	
		
		//active talk tasks only
		ActiveTasks = GetActiveTasksSlotsFromTrackWithArrays(i, EQuestTaskType::TalkTask, QuestsTrack);  //This is only for quests no dialogs
			
		for (int j = 0; j < ActiveTasks.Num(); j++)
		{
			if (QuestInv.IsValidIndex(i))
			{
				if (QuestInv[i].TalkTasks.IsValidIndex(ActiveTasks[j]))
				{
					//This npc?
					if (QuestInv[i].TalkTasks[ActiveTasks[j]].TalkNPC == NPCLocal)
					{
						//add quest and break
						QuestSlots.Add(i);
						if (GetFirstOneOnly)  shouldbreak = true;
						break;
					}						
				}
			}
		}			

		//break again if needed
		if (shouldbreak) break;
	}

	return QuestSlots;
}

TArray<FQuestTaskID> UQuestManager::GetTaskIDsInvolvingNPCTalk(int QuestSlotIn, AActor* NPCIn) const
{
	TArray<FQuestTaskID> TasksIDS;
	TasksIDS.Empty();

	//Invalid
	if (!NPCIn) return TasksIDS;

	//------------------------NPC Info-------------------------
	UNPCManager* NPCManagerComponent;
	NPCManagerComponent = Cast<UNPCManager>(NPCIn->FindComponentByClass(UNPCManager::StaticClass()));
	
	//NPC null
	if (!NPCManagerComponent)
	{
		return TasksIDS;
	}

	//NPC name
	FNPCQuestDefRep NPCLocal = NPCManagerComponent->GetNPCQuestDefRep();

	//-----------------------------Variables
	//GetQuest
	FQuest QuestLocal = GetQuestsInSlot(EQuestSelectedType::Active, QuestSlotIn, false); //Only for quests no dialogs
	TArray <int> ActiveTasks;
	FQuestTaskID TaskIDLocal;
	
	//active talk tasks
	TaskIDLocal.TaskType = EQuestTaskType::TalkTask;
	ActiveTasks = GetActiveTasksSlotsFromTrackWithArrays(QuestSlotIn, EQuestTaskType::TalkTask, QuestsTrack);  //Only for quests no dialogs

	for (int j = 0; j < ActiveTasks.Num(); j++)
	{			
		if (QuestLocal.TalkTasks.IsValidIndex(ActiveTasks[j]))
		{
			//this npc?
			if (QuestLocal.TalkTasks[ActiveTasks[j]].TalkNPC == NPCLocal && QuestLocal.TalkTasks[ActiveTasks[j]].TalkNPC != FNPCQuestDefRep())
			{
				//add task and continue
				TaskIDLocal.TaskID = ActiveTasks[j];
				TasksIDS.AddUnique(TaskIDLocal);
				continue;
			}
		}			
	}		
	
	return TasksIDS;
}

bool UQuestManager::NodeCanActivate(int QuestSlotIn, const FQuest& QuestIn, const FQuestTaskID& NodeIn, const TArray<FQuestTrack>& QuestsTrackIN) const
{
	//It needs to be locked.
	if (GetNodeStateInTrackWithArrays(QuestSlotIn, NodeIn, QuestsTrackIN) != EQuestTaskState::Locked) return false;

	//nodes required links and states of previous nodes
	return NodeHasLinksToActivate(QuestSlotIn, QuestIn, NodeIn, QuestsTrackIN);
}

bool UQuestManager::NodeCanReActivate(int QuestSlotIn, const FQuest& QuestIn, const FQuestTaskID& NodeIn, const TArray<FQuestTrack>& QuestsTrackIN) const
{
	//It needs to be completed or disabled or failed
	EQuestTaskState NodeState = GetNodeStateInTrackWithArrays(QuestSlotIn, NodeIn, QuestsTrackIN);
	if ((NodeState == EQuestTaskState::Active || NodeState == EQuestTaskState::Locked)) return false;

	//nodes required links and states of previous nodes
	return NodeHasLinksToActivate(QuestSlotIn, QuestIn, NodeIn, QuestsTrackIN);
}

bool UQuestManager::NodeHasLinksToActivate(int QuestSlotIn, const FQuest& QuestIn, const FQuestTaskID& NodeIn, const TArray<FQuestTrack>& QuestsTrackIN) const
{	
	bool canactivate = true;
	int TotalRequiredLinks = 0;
	int TotalCompletedOptionalLinks = 0;
	TArray <FQuestTaskLink> SuperiorNodes = UQuestsFunctionLibrary::GetNodeSuperiorLinksInQuest(QuestIn, NodeIn);

	//If no req nodes it activates
	if (SuperiorNodes.Num() > 0)
	{
		for (int i = 0; i < SuperiorNodes.Num(); i++)
		{
			//Direct link
			switch (SuperiorNodes[i].LinkType)
			{
			case ENodeLinkType::Required:
				// if one req node is not completed then it cannot activate
				if (GetNodeStateInTrackWithArrays(QuestSlotIn, SuperiorNodes[i].Node, QuestsTrackIN) != EQuestTaskState::Completed)
				{
					//If any req node is not completed then this cannot activate
					canactivate = false;
					break;
				}
				//If node is completed, count amount of completed req nodes
				else
				{
					//Increment the amount of required links that are completed
					TotalRequiredLinks++;
				}
				break;
			case ENodeLinkType::Optional:
				// if optional node is completed then i need to count
				if (GetNodeStateInTrackWithArrays(QuestSlotIn, SuperiorNodes[i].Node, QuestsTrackIN) == EQuestTaskState::Completed)
				{
					TotalCompletedOptionalLinks++;
				}
				break;
			default:
				break;
			}		
		}
		//If is false here, it means that should not activate. 
		//If it is true, it is a "soft true", need to check if at least there is one optional node completed
		if (canactivate == true)
		{
			//If there is not required links, at least one optional must be completed
			if (TotalRequiredLinks == 0)
			{
				canactivate = TotalCompletedOptionalLinks > 0;
			}
		}
		
	}	

	//First nodes can always activate
	else canactivate = true;

	return canactivate;
}

bool UQuestManager::TalkNodeCanPseudoActivate(const FQuest& QuestIn, const FQuestTaskID& NodeIn, const TArray<FQuestTaskID>& PseudoCompletedNodes) const
{
	//Task type invalid out
	if (NodeIn.TaskType == EQuestTaskType::Task) return false;
	
	//Invalid talk slot out
	if (!QuestIn.TalkTasks.IsValidIndex(NodeIn.TaskID)) return false;

	//Similar to the link check, but instead of checking for state, i have a pseudo completed node array to search and determine if the node is pseudo completed(is in the array)	
	bool canactivate = true;
	int TotalRequiredLinks = 0;
	int TotalCompletedOptionalLinks = 0;
	TArray <FQuestTaskLink> SuperiorNodes = UQuestsFunctionLibrary::GetNodeSuperiorLinksInQuest(QuestIn, NodeIn);

	//If no req nodes it activates
	if (SuperiorNodes.Num() > 0)
	{
		for (int i = 0; i < SuperiorNodes.Num(); i++)
		{
			//Direct link
			switch (SuperiorNodes[i].LinkType)
			{
			case ENodeLinkType::Required:
				// if one req node is not completed then it cannot activate
				if (!PseudoCompletedNodes.Contains(SuperiorNodes[i].Node))
				{
					//If any req node is not completed then this cannot activate
					canactivate = false;
					break;
				}
				//If node is completed, count amount of completed req nodes
				else
				{
					//Increment the amount of required links that are completed
					TotalRequiredLinks++;
				}
				break;
			case ENodeLinkType::Optional:
				// if optional node is completed then i need to count
				if (PseudoCompletedNodes.Contains(SuperiorNodes[i].Node))
				{
					TotalCompletedOptionalLinks++;
				}
				break;
			default:
				break;
			}
		}

		//If is false here, it means that should not activate. 
		//If it is true, it is a "soft true", need to check if at least there is one optional node completed
		if (canactivate == true)
		{
			//If there is not required links, at least one optional must be completed
			if (TotalRequiredLinks == 0)
			{
				canactivate = TotalCompletedOptionalLinks > 0;
			}
		}
	}
	//First nodes can always activate
	else canactivate = true;

	return canactivate;
}

bool UQuestManager::TaskActivateConditionCheck(const FQuest& QuestIn, int TaskSlot) const
{
	//--------------------------------Activate condition for tasks
	if (!QuestIn.Tasks[TaskSlot].TaskClass.IsValid())
	{
		QuestIn.Tasks[TaskSlot].TaskClass.LoadSynchronous();
	}

	TSubclassOf <UBaseQuestTask> TaskClass = QuestIn.Tasks[TaskSlot].TaskClass.Get();

	if (TaskClass)
	{
		const UBaseQuestTask* TaskCDO = TaskClass.GetDefaultObject();
		//Check condition
		bool result = TaskCDO->ActivateCondition(QuestIn.Tasks[TaskSlot].Payload, QuestIn.Tasks[TaskSlot].CustomPayloadData, this);
		//Should inverse?
		if (QuestIn.Tasks[TaskSlot].InverseActivateCondition)  return (!result);
		else return result;
	}
	//invalid task return false
	return false;
}

EQuestTaskState UQuestManager::GetNodeStateInTrack(int QuestSlotIn, const FQuestTaskID& NodeIn, bool IsDialogIN) const
{
	EQuestTaskState State = EQuestTaskState::Locked;

	if (IsDialogIN)
	{
		//Dialog
		State = GetNodeStateInTrackWithArrays(QuestSlotIn, NodeIn, DialogsTrack);
	}
	else
	{
		//Quest
		State = GetNodeStateInTrackWithArrays(QuestSlotIn, NodeIn, QuestsTrack);
	}

	return State;
}

EQuestTaskState UQuestManager::GetNodeStateInTrackWithArrays(int QuestSlotIn, const FQuestTaskID& NodeIn, const TArray<FQuestTrack>& QuestsTrackIN) const
{
	EQuestTaskState NodeState = EQuestTaskState::Locked;
	switch (NodeIn.TaskType)
	{
	case EQuestTaskType::Task:
	{
		if (!QuestsTrackIN.IsValidIndex(QuestSlotIn)) break;
		if (!QuestsTrackIN[QuestSlotIn].TasksTracks.IsValidIndex(NodeIn.TaskID))break;
		NodeState = QuestsTrackIN[QuestSlotIn].TasksTracks[NodeIn.TaskID].State;
	}
	break;
	case EQuestTaskType::TalkTask:
	{
		if (!QuestsTrackIN.IsValidIndex(QuestSlotIn)) break;
		if (!QuestsTrackIN[QuestSlotIn].TalkTasksTracks.IsValidIndex(NodeIn.TaskID))break;
		NodeState = QuestsTrackIN[QuestSlotIn].TalkTasksTracks[NodeIn.TaskID].State;
	}
	break;
	default:
		break;
	}

	return NodeState;
}

//TArray<FQuestTaskID> UQuestManager::GetNodeMutuallyDisabledNodes(int QuestSlotIn, const FQuestTaskID& NodeIn) const
//{
//	if (!IsTaskIDValid(QuestSlotIn, NodeIn)) return TArray<FQuestTaskID>();
//
//	FQuest QuestLocal = GetQuestsInSlot(EQuestSelectedType::Active, QuestSlotIn);
//
//	TArray<FQuestTaskID> MutuallyDisabled = TArray<FQuestTaskID>();
//	//get disabled nodes
//	TArray<FQuestTaskID> Disabled = UQuestsFunctionLibrary::GetNodeDisabledNodesInQuest(QuestLocal, NodeIn);
//	TArray<FQuestTaskID> DisabledTwo = TArray<FQuestTaskID>();
//
//	for (int i = 0; i < Disabled.Num(); i++)
//	{
//		//Get disabled nodes of the disabled ones
//		DisabledTwo = UQuestsFunctionLibrary::GetNodeDisabledNodesInQuest(QuestLocal, Disabled[i]);
//		//If contains original node they are mutually disabled nodes
//		if (DisabledTwo.Contains(NodeIn))
//		{
//			MutuallyDisabled.Add(NodeIn);
//		}
//	}	
//	return MutuallyDisabled;
//}

bool UQuestManager::ResetNodeTrack(int QuestSlotIn, const FQuestTaskID& NodeIn, TArray<FQuestTrack>& QuestsTrackIN)
{
	switch (NodeIn.TaskType)
	{
	case EQuestTaskType::Task:
	{
		if (!QuestsTrackIN.IsValidIndex(QuestSlotIn)) break;
		if (!QuestsTrackIN[QuestSlotIn].TasksTracks.IsValidIndex(NodeIn.TaskID))break;

		QuestsTrackIN[QuestSlotIn].TasksTracks[NodeIn.TaskID].State = EQuestTaskState::Locked;
		QuestsTrackIN[QuestSlotIn].TasksTracks[NodeIn.TaskID].ActivationOrder = -1;
		QuestsTrackIN[QuestSlotIn].TasksTracks[NodeIn.TaskID].CompletionOrder = -1;
		QuestsTrackIN[QuestSlotIn].TasksTracks[NodeIn.TaskID].CurrentAmount = 0;
		QuestsTrackIN[QuestSlotIn].TasksTracks[NodeIn.TaskID].CurrentPayload = FQuestTaskPayload();
	}

	break;
	case EQuestTaskType::TalkTask:
	{
		if (!QuestsTrackIN.IsValidIndex(QuestSlotIn)) break;
		if (!QuestsTrackIN[QuestSlotIn].TalkTasksTracks.IsValidIndex(NodeIn.TaskID))break;

		QuestsTrackIN[QuestSlotIn].TalkTasksTracks[NodeIn.TaskID].State = EQuestTaskState::Locked;
		QuestsTrackIN[QuestSlotIn].TalkTasksTracks[NodeIn.TaskID].ActivationOrder = -1;
		QuestsTrackIN[QuestSlotIn].TalkTasksTracks[NodeIn.TaskID].CompletionOrder = -1;
	}
	break;
	default:
		break;
	}

	return true;
}

bool UQuestManager::SetNodeStateInTrack(int QuestSlotIn, const FQuestTaskID& NodeIn, EQuestTaskState NewState, TArray<FQuestTrack>& QuestsTrackIN)
{
	switch (NodeIn.TaskType)
	{
	case EQuestTaskType::Task:
	{
		if (!QuestsTrackIN.IsValidIndex(QuestSlotIn)) break;
		if (!QuestsTrackIN[QuestSlotIn].TasksTracks.IsValidIndex(NodeIn.TaskID))break;
		QuestsTrackIN[QuestSlotIn].TasksTracks[NodeIn.TaskID].State = NewState;
	}		
		break;
	case EQuestTaskType::TalkTask:
	{
		if (!QuestsTrackIN.IsValidIndex(QuestSlotIn)) break;
		if (!QuestsTrackIN[QuestSlotIn].TalkTasksTracks.IsValidIndex(NodeIn.TaskID))break;
		QuestsTrackIN[QuestSlotIn].TalkTasksTracks[NodeIn.TaskID].State = NewState;
	}		
		break;
	default:
		break;
	}

	return true;
}

bool UQuestManager::SetNodeActivationOrderInTrack(int QuestSlotIn, const FQuestTaskID& NodeIn, int ActivationOrder, TArray<FQuestTrack>& QuestsTrackIN)
{
	switch (NodeIn.TaskType)
	{
	case EQuestTaskType::Task: 		
	{
		if (!QuestsTrackIN.IsValidIndex(QuestSlotIn)) break;
		if (!QuestsTrackIN[QuestSlotIn].TasksTracks.IsValidIndex(NodeIn.TaskID))break;
		QuestsTrackIN[QuestSlotIn].TasksTracks[NodeIn.TaskID].ActivationOrder = ActivationOrder;
	}	
		break;
	case EQuestTaskType::TalkTask:
	{
		if (!QuestsTrackIN.IsValidIndex(QuestSlotIn)) break;
		if (!QuestsTrackIN[QuestSlotIn].TalkTasksTracks.IsValidIndex(NodeIn.TaskID))break;
		QuestsTrackIN[QuestSlotIn].TalkTasksTracks[NodeIn.TaskID].ActivationOrder = ActivationOrder;
	}		
		break;
	default:
		break;
	}
	return true;
}

bool UQuestManager::SetNodeCompletionOrderInTrack(int QuestSlotIn, const FQuestTaskID& NodeIn, int CompletionOrder, TArray<FQuestTrack>& QuestsTrackIN)
{
	switch (NodeIn.TaskType)
	{
	case EQuestTaskType::Task:
	{
		if (!QuestsTrackIN.IsValidIndex(QuestSlotIn)) break;
		if (!QuestsTrackIN[QuestSlotIn].TasksTracks.IsValidIndex(NodeIn.TaskID))break;
		QuestsTrackIN[QuestSlotIn].TasksTracks[NodeIn.TaskID].CompletionOrder = CompletionOrder;
	}
	break;
	case EQuestTaskType::TalkTask:
	{
		if (!QuestsTrackIN.IsValidIndex(QuestSlotIn)) break;
		if (!QuestsTrackIN[QuestSlotIn].TalkTasksTracks.IsValidIndex(NodeIn.TaskID))break;
		QuestsTrackIN[QuestSlotIn].TalkTasksTracks[NodeIn.TaskID].CompletionOrder = CompletionOrder;
	}
	break;
	default:
		break;
	}
	return true;
}

bool UQuestManager::ActivateNodeID(int QuestSlotIn, bool IsDialogTaskIN, const FQuest& QuestIn, const FQuestTaskID& NodeIn, TArray<FQuestRep>& QuestsInventoryIN, TArray<FQuestTrack>& QuestsTrackIN, TArray<FQuestObjectTrack>& ObjectsTrackIN, TArray<FCompletedQuestTrack>& CompletedQuestsTrackIN, TArray<FQuestFailTrack>& QuestsFailTrackIN)
{
	//DebugOnly
	//SCOPE_CYCLE_COUNTER(STAT_ActivateNode);

	//First check of condition. If condition checks, node activates. If condition is false node fails.
	bool bShouldAutoComplete = false;
	bool bShouldSpawn = false;
	
	//Handle Spawn of tasksClass
	if (NodeIn.TaskType == EQuestTaskType::Task)
	{
		FQuestTask TaskLocal = QuestIn.Tasks[NodeIn.TaskID];
		//--------------------------------Activate condition for tasks
		//Load task if is not loaded		
		if (!QuestIn.Tasks[NodeIn.TaskID].TaskClass.IsValid())
		{
			QuestIn.Tasks[NodeIn.TaskID].TaskClass.LoadSynchronous();
		}

		if (TaskLocal.TaskClass.IsValid())
		{												   
			//Can activate (only checks activate condition here. Everything else es checked previously)
			if (TaskActivateConditionCheck(QuestIn, NodeIn.TaskID))
			{
				//Is condition only
				if (UQuestsFunctionLibrary::TaskIsActivateConditionOnly(QuestIn, NodeIn.TaskID))
				{
					//Log
					UE_LOG(QuestEditorLog, Log, TEXT("UQuestManager::ActivateNodeID - Task %i of quest %i can activate and is ActivationOnlyTask. Will not spawn task object."), NodeIn.TaskID, QuestSlotIn);
					//Complete on condition passed.
					bShouldAutoComplete = true;
				}
				//Is not condition only
				else
				{
					//-----------------------Normal tasks spawn
					//Log
					UE_LOG(QuestEditorLog, Log, TEXT("UQuestManager::ActivateNodeID - Task %i of quest %i can activate and should spawn task object."), NodeIn.TaskID, QuestSlotIn);
					bShouldSpawn = true;					
				}
			}
			//failed activation
			else
			{
				//Log
				UE_LOG(QuestEditorLog, Warning, TEXT("UQuestManager::ActivateNodeID - Task %i of quest %i failed activation."), NodeIn.TaskID, QuestSlotIn);
				//Set to active in order to make it fail but with no dispatchers and stuff. Its like a preactivation
				SetNodeStateInTrack(QuestSlotIn, NodeIn, EQuestTaskState::Active, QuestsTrackIN);
				FailNode(QuestSlotIn, QuestIn, IsDialogTaskIN, NodeIn, QuestsTrackIN, ObjectsTrackIN);
				return false;
			}
		}
		//invalid task return false (should never happen)
		else
		{
			//Log
			UE_LOG(QuestEditorLog, Error, TEXT("UQuestManager::ActivateNodeID - Task %i of quest %i is invalid."), NodeIn.TaskID, QuestSlotIn);
			return false;
		}			
	}	

	//Should have check if can activate before this
	//------------------------------Change state to active
	SetNodeStateInTrack(QuestSlotIn, NodeIn, EQuestTaskState::Active, QuestsTrackIN);
	OnNodeChangedState.Broadcast(QuestSlotIn, NodeIn, IsDialogTaskIN, EQuestTaskState::Active);

	//------------------------------Activation order
	SetNodeActivationOrderInTrack(QuestSlotIn, NodeIn, GetCurrentActivationOrder(QuestSlotIn, QuestsTrackIN) + 1, QuestsTrackIN);

	//------------------------------add to active track
	AddTaskIDToActiveTrack(QuestSlotIn, NodeIn, QuestsTrackIN);
	
	//Dispatcher
	QTaskActivated.Broadcast(QuestSlotIn, NodeIn, IsDialogTaskIN);
	if (GetNetMode() != NM_DedicatedServer)	Client_QTaskActivated.Broadcast(QuestSlotIn, NodeIn, IsDialogTaskIN);
	
	//activation Dispatcher
	switch (NodeIn.TaskType)
	{
	case EQuestTaskType::Task:
	{
		//ActivateDispatcher
		if (IsDialogTaskIN)
		{
			DialogTaskActivated.Broadcast(QuestSlotIn, NodeIn.TaskID, QuestIn.Tasks[NodeIn.TaskID]);
		}
		else
		{
			QuestTaskActivated.Broadcast(QuestSlotIn, NodeIn.TaskID, QuestIn.Tasks[NodeIn.TaskID]);
		}	
	}
	break;
	case EQuestTaskType::TalkTask:
	{
		//ActivateDispatcher
		if (IsDialogTaskIN)
		{
			DialogTalkTaskActivated.Broadcast(QuestSlotIn, NodeIn.TaskID, QuestIn.TalkTasks[NodeIn.TaskID]);
		}
		else
		{
			QuestTalkTaskActivated.Broadcast(QuestSlotIn, NodeIn.TaskID, QuestIn.TalkTasks[NodeIn.TaskID]);
		}		
	}
	break;
	default:
		break;
	}

	//If node is autocomplete needs to complete
	if (bShouldAutoComplete) CompleteNode(QuestSlotIn, IsDialogTaskIN, QuestIn, NodeIn, QuestsInventoryIN, QuestsTrackIN, ObjectsTrackIN, CompletedQuestsTrackIN, QuestsFailTrackIN);

	//Handle spawn at the end, to ensure correct start of task after all variables are set correctly
	if (bShouldSpawn) HandleBaseTaskSpawn(QuestSlotIn, QuestIn, NodeIn.TaskID, IsDialogTaskIN, ObjectsTrackIN, false);

	return true;
}

int UQuestManager::GetCurrentActivationOrder(int QuestSlotIn, TArray<FQuestTrack>& QuestsTrackIN) const
{
	//find max order
	int MaxOrder = -1;

	//tasks
	for (int i = 0; i < QuestsTrackIN[QuestSlotIn].TasksTracks.Num(); i++)
	{
		if (QuestsTrackIN[QuestSlotIn].TasksTracks[i].ActivationOrder > MaxOrder)
		{
			MaxOrder = QuestsTrackIN[QuestSlotIn].TasksTracks[i].ActivationOrder;
		}			
	}

	//talk tasks
	for (int i = 0; i < QuestsTrackIN[QuestSlotIn].TalkTasksTracks.Num(); i++)
	{
		if (QuestsTrackIN[QuestSlotIn].TalkTasksTracks[i].ActivationOrder > MaxOrder)
		{
			MaxOrder = QuestsTrackIN[QuestSlotIn].TalkTasksTracks[i].ActivationOrder;
		}			
	}	

	return MaxOrder;
}

int UQuestManager::GetCurrentCompletionOrder(int QuestSlotIn, TArray<FQuestTrack>& QuestsTrackIN) const
{
	//find max order
	int MaxOrder = -1;

	//tasks
	for (int i = 0; i < QuestsTrackIN[QuestSlotIn].TasksTracks.Num(); i++)
	{
		if (QuestsTrackIN[QuestSlotIn].TasksTracks[i].CompletionOrder > MaxOrder)
		{
			MaxOrder = QuestsTrackIN[QuestSlotIn].TasksTracks[i].CompletionOrder;
		}			
	}

	//talk tasks
	for (int i = 0; i < QuestsTrackIN[QuestSlotIn].TalkTasksTracks.Num(); i++)
	{
		if (QuestsTrackIN[QuestSlotIn].TalkTasksTracks[i].CompletionOrder > MaxOrder)
		{
			MaxOrder = QuestsTrackIN[QuestSlotIn].TalkTasksTracks[i].CompletionOrder;
		}		
	}

	return MaxOrder;
}

int UQuestManager::GetLastCompletedTalkTask(int QuestSlotIn, TArray<FQuestTrack>& QuestsTrackIN) const
{
	//find max order
	int MaxOrder = -1;
	int LastTaskIndex = -1;

	//talk tasks
	for (int i = 0; i < QuestsTrackIN[QuestSlotIn].TalkTasksTracks.Num(); i++)
	{
		if (QuestsTrackIN[QuestSlotIn].TalkTasksTracks[i].CompletionOrder > MaxOrder)
		{
			MaxOrder = QuestsTrackIN[QuestSlotIn].TalkTasksTracks[i].CompletionOrder;
			LastTaskIndex = i;
		}
	}

	return LastTaskIndex;
}

int UQuestManager::GetTasksAmountByState(int QuestSlotIn, TArray<FQuestTrack>& QuestsTrackIN, EQuestTaskState StateIn) const
{
	int amount = 0;

	if (!QuestsTrackIN.IsValidIndex(QuestSlotIn))return 0;

	for (int i = 0; i < QuestsTrackIN[QuestSlotIn].TasksTracks.Num(); i++)
	{
		//add ID in activation position
		if (QuestsTrackIN[QuestSlotIn].TasksTracks[i].State == StateIn) amount++;
	}
	//Talk tasks
	for (int i = 0; i < QuestsTrackIN[QuestSlotIn].TalkTasksTracks.Num(); i++)
	{
		//add ID in activation position
		if (QuestsTrackIN[QuestSlotIn].TalkTasksTracks[i].State == StateIn) amount++;
	}
	
	return amount;
}

TArray <int> UQuestManager::GetStartingTasksSlots(EQuestTaskType TaskTypeIn, const FQuest& QuestIn, AActor* NPCIN, bool AnyNPC) const
{
	TArray <int> SlotsInitial = TArray <int>();

	switch (TaskTypeIn)
	{
	case EQuestTaskType::Task:
	{
		for (int i = 0; i < QuestIn.Tasks.Num(); i++)
		{
			//No NPC Check here
			if (QuestIn.Tasks[i].SuperiorNodes.Num() == 0)
			{
				SlotsInitial.Add(i);
			}
		}
	}
	break;
	case EQuestTaskType::TalkTask:
	{
		//Get NPC to compare in talktasks
		FNPCQuestDefRep NPCLocal = FNPCQuestDefRep();

		//NPC not null 
		if (NPCIN != nullptr)
		{
			//get NPC manager
			UNPCManager* NPCManagerComponent;
			NPCManagerComponent = Cast<UNPCManager>(NPCIN->FindComponentByClass(UNPCManager::StaticClass()));
			
			//NPC null
			if (!NPCManagerComponent) return SlotsInitial;
			NPCLocal = NPCManagerComponent->GetNPCQuestDefRep();
		}

		//If a NPC is specified, it needs to match it.
		for (int i = 0; i < QuestIn.TalkTasks.Num(); i++)
		{
			//Is starting node and for this specific NPC
			if (QuestIn.TalkTasks[i].SuperiorNodes.Num() == 0 && (QuestIn.TalkTasks[i].TalkNPC == NPCLocal || AnyNPC))
			{
				SlotsInitial.Add(i);
			}
		}
	}
	break;
	default:
		break;
	}

	return SlotsInitial;
}

TArray<int> UQuestManager::GetStartingTalkTasksSlots(const FQuest& QuestIn, AActor* NPCIN, bool AnyNPC) const
{
	//Finds all starting talk nodes (with no req nodes)
	TArray<int> AllStartNodesTalk = GetStartingTasksSlots(EQuestTaskType::TalkTask, QuestIn, NPCIN, AnyNPC);

	//Finds all propagated talk taks after activation of ConditionOnlyTasks
	AllStartNodesTalk.Append(GetPseudoStartingTalkTasksSlots(QuestIn, NPCIN, AnyNPC));

	return AllStartNodesTalk;
}

TArray<int> UQuestManager::GetPseudoStartingTalkTasksSlots(const FQuest& QuestIn, AActor* NPCIN, bool AnyNPC) const
{
	//Propagates activation of condition only tasks to get the state of the quest once all are autocompleted and gets the talktasks that this nodes activate.

	//------------------------------------------Obtain  all tasks that are starting and activate condition only
	TArray <int> StartACOTasksCompleted = GetStartingTasksThatCanActivate(QuestIn, NPCIN, AnyNPC);
	
	if (StartACOTasksCompleted.Num() == 0) return TArray <int> ();

	//---------------------------------------get all initial inferior nodes
	TArray <FQuestTaskID> InferiorNodesA;
	TArray <FQuestTaskID> InferiorNodesB;
	TArray <FQuestTaskID> AnalizedNodes;
	TArray <FQuestTaskID> PseudoCompletedNodes;

	//Talk task pseudo initial (they are initial after ACO nodes completed)
	TArray <int> TalkPseudoInitialNodesOut;

	FQuestTaskID TaskIDLocal;
	TaskIDLocal.TaskType = EQuestTaskType::Task;	  

	//If needs to check NPCs, get a NPC ID
	FNPCQuestDefRep NPCLocal = FNPCQuestDefRep();	
	if (!AnyNPC)
	{	
		//NPC not null 
		if (NPCIN != nullptr)
		{
			//get NPC manager
			UNPCManager* NPCManagerComponent;
			NPCManagerComponent = Cast<UNPCManager>(NPCIN->FindComponentByClass(UNPCManager::StaticClass()));
			
			//NPC valid, save ID
			if (NPCManagerComponent) NPCLocal = NPCManagerComponent->GetNPCQuestDefRep();
			
			//This should never happend. They want me to check npc but the npc is invalid.
			else  return TArray <int>();
		}
		//This should never happend. They want me to check npc but the npc is invalid.
		else  return TArray <int>();
	}

	//Add to array all inferior nodes of ACO that are "completed" on start
	for (int i = 0; i < StartACOTasksCompleted.Num(); i++)
	{
		TaskIDLocal.TaskID = StartACOTasksCompleted[i];
		//Temporary save all initial ACO nodes on ID form
		InferiorNodesB.AddUnique(TaskIDLocal);
		//Add all to initial pool
		AnalizedNodes.Append(UQuestsFunctionLibrary::ConvertLinkArrayToNodeArray(QuestIn.Tasks[StartACOTasksCompleted[i]].InferiorNodes));
	}

	//filter to get all unique inferior nodes no duplicates
	for (int i = 0; i < AnalizedNodes.Num(); i++)
	{
		InferiorNodesA.AddUnique(AnalizedNodes[i]);
	}

	//Save all ACO nodes used previously. 
	//Dont care about repeated nodes here
	PseudoCompletedNodes = InferiorNodesB;	//This are condition tasks only, that i will consider completed for a pseudo activate
	AnalizedNodes = InferiorNodesB; //This are all analized nodes


	//Clear Bnodes
	InferiorNodesB.Empty();

	bool WhileEnable = true;

	int32 LoopCounter = 0;
	while (WhileEnable && LoopCounter<MaxWhileLoopCount)
	{
		LoopCounter++;
		if (LoopCounter == MaxWhileLoopCount) 	UE_LOG(QuestEditorLog, Error, TEXT("UQuestManager::GetPseudoStartingTalkTasksSlots - While Max Loop reached"));

		//Check all inferior A nodes
		//talk node
		for (int i = 0; i < InferiorNodesA.Num(); i++)
		{
			//Node is already analized?
			if (AnalizedNodes.Contains(InferiorNodesA[i])) continue;  //continue

			//Talk task
			if (InferiorNodesA[i].TaskType == EQuestTaskType::TalkTask)
			{
				//If any NPC or Specific and equal npcs
				if (AnyNPC  || (!AnyNPC && NPCLocal == QuestIn.TalkTasks[InferiorNodesA[i].TaskID].TalkNPC))
				{
					//Mets activation requeriments? (can activate if the previous conditions were completed)
					if (TalkNodeCanPseudoActivate(QuestIn, InferiorNodesA[i], PseudoCompletedNodes))
					{
						//Check if is truly a pseudo node and has no supeiror nodes that are not conditions
						//I dont need to check this? If a node has a start condition and then meets activation, no need to check other nodes, for circular connections.
						//if (UQuestsFunctionLibrary::AreAllSuperiorNodesOfNodeConditions(QuestIn, InferiorNodesA[i]))	//disabled this for circular connections
						{
							//Add to array out of initial talk task				
							TalkPseudoInitialNodesOut.AddUnique(InferiorNodesA[i].TaskID);
						}
					}
				}			
			}
			//ActivateConditionOnly tasks (no spawn task, only check)
			else if (UQuestsFunctionLibrary::TaskIsActivateConditionOnly(QuestIn, InferiorNodesA[i].TaskID))
			{
				//Condition checks?
				if (TaskActivateConditionCheck(QuestIn, InferiorNodesA[i].TaskID))
				{
					//Consider this task as completed
					PseudoCompletedNodes.Add(InferiorNodesA[i]);

					//add  INFERIOR nodes to arrayB to check in next while loop (propagation)
					InferiorNodesB.Append(UQuestsFunctionLibrary::ConvertLinkArrayToNodeArray(QuestIn.Tasks[InferiorNodesA[i].TaskID].InferiorNodes));
				}
			}

			//add to array of already analized, prevent nodes to be analized more than once
			AnalizedNodes.Add(InferiorNodesA[i]);
		}

		//If B is empty there is nothing more to check
		if (InferiorNodesB.Num() == 0) WhileEnable = false;
		//Need to clear A, load with B and clear B
		else
		{
			InferiorNodesA = InferiorNodesB;
			InferiorNodesB.Empty();
		}
	}

	//Filter unique only
	TArray<int> FilteredNodes;

	for (int i = 0; i < TalkPseudoInitialNodesOut.Num(); i++)
	{
		FilteredNodes.AddUnique(TalkPseudoInitialNodesOut[i]);
	}

	return FilteredNodes;	
}

TArray<int> UQuestManager::GetStartingTasksThatCanActivate(const FQuest& QuestIn, AActor* NPCIN, bool AnyNPC) const
{
	//Returns all tasks that will activate and complete upon quest accept
	TArray <int> InitialTasks;
	InitialTasks.Empty();

	TArray <int> TaskOut;
	TaskOut.Empty();

	//has at least a activable task?
	InitialTasks = GetStartingTasksSlots(EQuestTaskType::Task, QuestIn, NPCIN, AnyNPC);

	//Convert initials tasks to IDS
	for (int i = 0; i < InitialTasks.Num(); i++)
	{
		//Task is only check?
		if (UQuestsFunctionLibrary::TaskIsActivateConditionOnly(QuestIn, InitialTasks[i]))
		{
			//Check if can activate on start and add to array
			if (TaskActivateConditionCheck(QuestIn, InitialTasks[i]))
			{
				TaskOut.Add(InitialTasks[i]);
			}
		}
	}	
	return TaskOut;
}

TArray<FQuestTaskID> UQuestManager::GetCompletedTasksIDsByActivationOrder(int QuestSlotIn, EQuestSelectedType QuestTypeIn, EQuestTaskTypeSelection SelectionIn, bool IsDialogIn) const
{
	TArray<FQuestTaskID> LocalTasks = TArray<FQuestTaskID>();

	if (IsDialogIn)
	{
		//Dialogs
		LocalTasks = GetCompletedTasksIDsByActivationOrderWithArrays(QuestSlotIn, QuestTypeIn, SelectionIn, DialogsTrack, CompletedDialogsTrack);
	}
	else
	{
		//Quests
		LocalTasks = GetCompletedTasksIDsByActivationOrderWithArrays(QuestSlotIn, QuestTypeIn, SelectionIn, QuestsTrack, CompletedQuestsTrack);
	}

	return LocalTasks;
}

TArray<FQuestTaskID> UQuestManager::GetCompletedTasksIDsByActivationOrderWithArrays(int QuestSlotIn, EQuestSelectedType QuestTypeIn, EQuestTaskTypeSelection SelectionIn, const TArray<FQuestTrack>& QuestsTrackIN, const TArray<FCompletedQuestTrack>& CompletedQuestsTrackIN) const
{
	//Make a map with id + activation order
	TMap <FQuestTaskID, int32> TaskIDActivation;
	TaskIDActivation.Empty();

	FQuestTaskID TaskIDLocal;

	//Get track
	FQuestTrack QuestTrackLocal;
	switch (QuestTypeIn)
	{
	case EQuestSelectedType::Active:
	{
		if (QuestsTrackIN.IsValidIndex(QuestSlotIn))QuestTrackLocal = QuestsTrackIN[QuestSlotIn];
		else return TArray<FQuestTaskID>();
	}
	break;
	case EQuestSelectedType::Completed:
	{
		if (CompletedQuestsTrackIN.IsValidIndex(QuestSlotIn)) QuestTrackLocal = CompletedQuestsTrackIN[QuestSlotIn].QuestTrack;
		else return TArray<FQuestTaskID>();
	}
	break;
	default: return TArray<FQuestTaskID>();
		break;
	}

	if (SelectionIn == EQuestTaskTypeSelection::All || SelectionIn == EQuestTaskTypeSelection::Task)
	{
		//Tasks
		TaskIDLocal.TaskType = EQuestTaskType::Task;
		for (int i = 0; i < QuestTrackLocal.TasksTracks.Num(); i++)
		{
			TaskIDLocal.TaskID = i;
			//add ID in activation position
			if (QuestTrackLocal.TasksTracks[i].ActivationOrder >= 0 && QuestTrackLocal.TasksTracks[i].State == EQuestTaskState::Completed)TaskIDActivation.Add(TaskIDLocal, QuestTrackLocal.TasksTracks[i].ActivationOrder);
		}
	}
	if (SelectionIn == EQuestTaskTypeSelection::All || SelectionIn == EQuestTaskTypeSelection::TalkTask)
	{
		//Talk tasks
		TaskIDLocal.TaskType = EQuestTaskType::TalkTask;
		for (int i = 0; i < QuestTrackLocal.TalkTasksTracks.Num(); i++)
		{
			TaskIDLocal.TaskID = i;
			//add ID in activation position
			if (QuestTrackLocal.TalkTasksTracks[i].ActivationOrder >= 0 && QuestTrackLocal.TalkTasksTracks[i].State == EQuestTaskState::Completed)TaskIDActivation.Add(TaskIDLocal, QuestTrackLocal.TalkTasksTracks[i].ActivationOrder);
		}
	}

	//Sort by activation order using lambda.
	TaskIDActivation.ValueSort([](int a, int b)
		{
			return a < b;
		});

	//Out vars
	TArray<FQuestTaskID> SortedIDs;
	TaskIDActivation.GenerateKeyArray(SortedIDs);
	return SortedIDs;
}

TArray<FQuestTaskID> UQuestManager::GetCompletedTasksIDsByCompletionOrder(int QuestSlotIn, EQuestSelectedType QuestTypeIn, EQuestTaskTypeSelection SelectionIn, bool IsDialogIn) const
{
	TArray<FQuestTaskID> LocalTasks = TArray<FQuestTaskID>();

	if (IsDialogIn)
	{
		//Dialogs
		LocalTasks = GetCompletedTasksIDsByCompletionOrderWithArrays(QuestSlotIn, QuestTypeIn, SelectionIn, DialogsTrack, CompletedDialogsTrack);
	}
	else
	{
		//Quests
		LocalTasks = GetCompletedTasksIDsByCompletionOrderWithArrays(QuestSlotIn, QuestTypeIn, SelectionIn, QuestsTrack, CompletedQuestsTrack);
	}

	return LocalTasks;
}

TArray<FQuestTaskID> UQuestManager::GetCompletedTasksIDsByCompletionOrderWithArrays(int QuestSlotIn, EQuestSelectedType QuestTypeIn, EQuestTaskTypeSelection SelectionIn, const TArray<FQuestTrack>& QuestsTrackIN, const TArray<FCompletedQuestTrack>& CompletedQuestsTrackIN) const
{
	//Same than previous function but for completion order
	//Make map
	TMap <FQuestTaskID, int32> TaskIDActivation;
	TaskIDActivation.Empty();

	FQuestTaskID TasksIDsLocal;

	//Get track
	FQuestTrack QuestsTrackLocal;
	switch (QuestTypeIn)
	{
	case EQuestSelectedType::Active:
	{
		if (QuestsTrackIN.IsValidIndex(QuestSlotIn))QuestsTrackLocal = QuestsTrackIN[QuestSlotIn];
		else return TArray<FQuestTaskID>();
	}
	break;
	case EQuestSelectedType::Completed:
	{
		if (CompletedQuestsTrackIN.IsValidIndex(QuestSlotIn)) QuestsTrackLocal = CompletedQuestsTrackIN[QuestSlotIn].QuestTrack;
		else return TArray<FQuestTaskID>();
	}
	break;
	default: return TArray<FQuestTaskID>();
		break;
	}

	if (SelectionIn == EQuestTaskTypeSelection::All || SelectionIn == EQuestTaskTypeSelection::Task)
	{
		//Tasks
		TasksIDsLocal.TaskType = EQuestTaskType::Task;
		for (int i = 0; i < QuestsTrackLocal.TasksTracks.Num(); i++)
		{
			TasksIDsLocal.TaskID = i;
			//Add Id in completion position
			if (QuestsTrackLocal.TasksTracks[i].CompletionOrder >= 0 && QuestsTrackLocal.TasksTracks[i].State == EQuestTaskState::Completed)TaskIDActivation.Add(TasksIDsLocal, QuestsTrackLocal.TasksTracks[i].CompletionOrder);
		}
	}

	if (SelectionIn == EQuestTaskTypeSelection::All || SelectionIn == EQuestTaskTypeSelection::TalkTask)
	{
		//Talk tasks
		TasksIDsLocal.TaskType = EQuestTaskType::TalkTask;
		for (int i = 0; i < QuestsTrackLocal.TalkTasksTracks.Num(); i++)
		{
			TasksIDsLocal.TaskID = i;
			//Add Id in completion position
			if (QuestsTrackLocal.TalkTasksTracks[i].CompletionOrder >= 0 && QuestsTrackLocal.TalkTasksTracks[i].State == EQuestTaskState::Completed)TaskIDActivation.Add(TasksIDsLocal, QuestsTrackLocal.TalkTasksTracks[i].CompletionOrder);
		}
	}

	//Sort by activation order using lambda.
	TaskIDActivation.ValueSort([](int a, int b)
		{
			return a < b;
		});

	//vars out
	TArray<FQuestTaskID> SortedIDs;
	TaskIDActivation.GenerateKeyArray(SortedIDs);
	return SortedIDs;
}

bool UQuestManager::AttempToActivateInferiorNodes(int QuestSlotIn, bool IsDialogTaskIN, const FQuest& QuestIn, bool IsStartingNodeIn, const FQuestTaskID& SuperiorNode, const TArray<FQuestTaskID>& InferiorNodesIn, TArray<FQuestRep>& QuestsInventoryIN, TArray<FQuestTrack>& QuestsTrackIN, TArray<FQuestObjectTrack>& ObjectsTrackIN, TArray<FCompletedQuestTrack>& CompletedQuestsTrackIN, TArray<FQuestFailTrack>& QuestsFailTrackIN)
{
	//Save a local value of the slot.
	int QuestSlotLocal = QuestSlotIn;

	//Save the quest rep before executing, at this point the questrep is stable
	FQuestRep QuestRepBeforeInferiorNodes = QuestsInventoryIN[QuestSlotLocal];

	//Activate all inferior nodes
	for (int i = 0; i < InferiorNodesIn.Num(); i++)
	{
		//Need to verify QuestSlot after each activation, to ensure is still valid, in case previous activation changed array lenghts
		QuestSlotLocal = CorrectQuestSlotForNodeExecution(QuestSlotLocal, IsDialogTaskIN, QuestRepBeforeInferiorNodes, QuestsInventoryIN);

		//Should allow reactivation?
		if (!IsStartingNodeIn)
		{
			//Can Re- activate?
			if (NodeCanReActivate(QuestSlotLocal, QuestIn, InferiorNodesIn[i], QuestsTrackIN))
			{
				//Re-Activation
				HandleTaskEnd_ResetTrackToNode(QuestSlotLocal, SuperiorNode, InferiorNodesIn[i], IsDialogTaskIN, true);

				//If a node reactivated, cannot activate after
				continue;
			}
		}

		//Can activate?
		if (NodeCanActivate(QuestSlotLocal, QuestIn, InferiorNodesIn[i], QuestsTrackIN))
		{
			//Activation
			ActivateNodeID(QuestSlotLocal, IsDialogTaskIN, QuestIn, InferiorNodesIn[i], QuestsInventoryIN, QuestsTrackIN, ObjectsTrackIN, CompletedQuestsTrackIN, QuestsFailTrackIN);
		}
	}

	return true;
}

bool UQuestManager::DeactivateNodes(int QuestSlotIn, const TArray<FQuestTaskID>& NodesIn, bool IsDialogIn, TArray<FQuestTrack>& QuestsTrackIN, TArray<FQuestObjectTrack>& ObjectsTrackIN)
{
	//Only disable NodesIn changing state
	for (int i = 0; i < NodesIn.Num(); i++)
	{
		DeactivateSingleNode(QuestSlotIn, NodesIn[i], IsDialogIn, QuestsTrackIN, ObjectsTrackIN);
	}
	return true;
}

bool UQuestManager::DeactivateSingleNode(int QuestSlotIn, const FQuestTaskID& NodeIn, bool IsDialogIn, TArray<FQuestTrack>& QuestsTrackIN, TArray<FQuestObjectTrack>& ObjectsTrackIN)
{
	//I need a local ref because i will remove from track
	FQuestTaskID NodeLocal = NodeIn;

	EQuestTaskState NodeState = GetNodeStateInTrackWithArrays(QuestSlotIn, NodeLocal, QuestsTrackIN);
	
	//is completed
	if (NodeState == EQuestTaskState::Completed) return false;

	//If the node is locked then it might be a Reactivate connection
	if (NodeState == EQuestTaskState::Locked) return false;

	//remove from active if needed
	if (NodeState == EQuestTaskState::Active)
	{
		//If task has valid objects, deactivation logic
		if (NodeLocal.TaskType == EQuestTaskType::Task)
		{
			//Object valid?
			if (ObjectsTrackIN[QuestSlotIn].ObjectTasksTrack[NodeLocal.TaskID].BaseTaskReference != nullptr)
			{
				//Disable logic
				ObjectsTrackIN[QuestSlotIn].ObjectTasksTrack[NodeLocal.TaskID].BaseTaskReference->DisableQuestTask();
			}
		}
		//Because i am removing from track, i need to have the node in local variable
		RemoveTaskIDFromActiveTrack(QuestSlotIn, NodeLocal, QuestsTrackIN, ObjectsTrackIN);
	}
	   
	//Remove from pre active if needed
	if (NodeState == EQuestTaskState::PreActive)
	{
		//Because i am removing from track, i need to have the node in local variable
		RemoveTaskIDFromPreActiveTrack(QuestSlotIn, NodeLocal, QuestsTrackIN);
	}

	//If is not disabled 
	if (NodeState != EQuestTaskState::Disabled)
	{
		SetNodeStateInTrack(QuestSlotIn, NodeLocal, EQuestTaskState::Disabled, QuestsTrackIN);
		OnNodeChangedState.Broadcast(QuestSlotIn, NodeLocal, IsDialogIn, EQuestTaskState::Disabled);
		QTaskDeactivated.Broadcast(QuestSlotIn, NodeLocal, IsDialogIn);

		if (GetNetMode() != NM_DedicatedServer)	Client_QTaskIsNoLongerActive.Broadcast(QuestSlotIn, NodeLocal, IsDialogIn);
	}
	//If disabled return false
	else return false;
	return true;
}

bool UQuestManager::FailNode(int QuestSlotIn, const FQuest& QuestIn, bool IsDialogIn, const FQuestTaskID& NodeIn, TArray<FQuestTrack>& QuestsTrackIN, TArray<FQuestObjectTrack>& ObjectsTrackIN)
{
	//When a node fails, it also makes the disabled nodes fail and needs to check for fail quest.
	TArray <FQuestTaskID> NodesToFail;
	NodesToFail.Empty();

	//The node fails
	NodesToFail.Add(NodeIn);

	//All disabled nodes also fails
	NodesToFail.Append(UQuestsFunctionLibrary::GetNodeDisabledNodesInQuest(QuestIn, NodeIn));

	//Handle fail of all nodes.
	MultipleNodesFail(QuestSlotIn, QuestIn, IsDialogIn, NodesToFail, QuestsTrackIN, ObjectsTrackIN);

	return true;
}

bool UQuestManager::SingleNodeFail(int QuestSlotIn, const FQuest& QuestIn, bool IsDialogIn, const FQuestTaskID& NodeIn, TArray<FQuestTrack>& QuestsTrackIN, TArray<FQuestObjectTrack>& ObjectsTrackIN)
{
	//Any node can fail, Talk Tasks can only fail of a task makes them fail after failing itself.

	//if completed dont do anything
	if (GetNodeStateInTrackWithArrays(QuestSlotIn, NodeIn, QuestsTrackIN) != EQuestTaskState::Active) return false;

	//If node is task, needs to handle objects
	if (NodeIn.TaskType == EQuestTaskType::Task)
	{
		//If task spawned objets need to handle them. ActivateCondOnly do not spawn task objects
		//if objects are invalid then this is skipped
		if (ObjectsTrackIN[QuestSlotIn].ObjectTasksTrack[NodeIn.TaskID].BaseTaskReference != nullptr)
		{
			//Fail logic
			ObjectsTrackIN[QuestSlotIn].ObjectTasksTrack[NodeIn.TaskID].BaseTaskReference->FailQuestTask();
		}
	}

	//Remove from track and handle destroy
	RemoveTaskIDFromActiveTrack(QuestSlotIn, NodeIn, QuestsTrackIN, ObjectsTrackIN);

	//Track state
	SetNodeStateInTrack(QuestSlotIn, NodeIn, EQuestTaskState::Failed, QuestsTrackIN);
	OnNodeChangedState.Broadcast(QuestSlotIn, NodeIn, IsDialogIn, EQuestTaskState::Failed);

	//Dispatchers for TASK fail
	if (NodeIn.TaskType == EQuestTaskType::Task)
	{
		//Dispatcher
		if (IsDialogIn)
		{
			DialogTaskFailed.Broadcast(QuestSlotIn, NodeIn.TaskID, QuestIn.Tasks[NodeIn.TaskID]);
		}
		else
		{
			QuestTaskFailed.Broadcast(QuestSlotIn, NodeIn.TaskID, QuestIn.Tasks[NodeIn.TaskID]);
			
		}
		if (GetNetMode() != NM_DedicatedServer)	Client_QTaskIsNoLongerActive.Broadcast(QuestSlotIn, NodeIn, IsDialogIn);
	}
	else
	{
		//Dispatcher
		if (IsDialogIn)
		{
			DialogTalkTaskFailed.Broadcast(QuestSlotIn, NodeIn.TaskID, QuestIn.TalkTasks[NodeIn.TaskID]);
		}
		else
		{
			QuestTalkTaskFailed.Broadcast(QuestSlotIn, NodeIn.TaskID, QuestIn.TalkTasks[NodeIn.TaskID]);
		}		
	}

	return true;
}

bool UQuestManager::MultipleNodesFail(int QuestSlotIn, const FQuest& QuestIn, bool IsDialogIn, const TArray<FQuestTaskID>& NodesIn, TArray<FQuestTrack>& QuestsTrackIN, TArray<FQuestObjectTrack>& ObjectsTrackIN)
{
	for (int i = 0; i < NodesIn.Num(); i++)
	{
		//Fail
		SingleNodeFail(QuestSlotIn, QuestIn, IsDialogIn, NodesIn[i], QuestsTrackIN, ObjectsTrackIN);
	}
	return true;
}

bool UQuestManager::SingleNodeLock(int QuestSlotIn, const FQuest& QuestIn, const FQuestTaskID& NodeIn, bool IsDialogIn, TArray<FQuestTrack>& QuestsTrackIN, TArray<FQuestObjectTrack>& ObjectsTrackIN)
{
	//if locked dont do anything
	if (GetNodeStateInTrackWithArrays(QuestSlotIn, NodeIn, QuestsTrackIN) == EQuestTaskState::Locked) return false;

	//Remove from track and handle destroy
	RemoveTaskIDFromActiveTrack(QuestSlotIn, NodeIn, QuestsTrackIN, ObjectsTrackIN);

	//Track state
	SetNodeStateInTrack(QuestSlotIn, NodeIn, EQuestTaskState::Locked, QuestsTrackIN);

	//Track reset 
	ResetNodeTrack(QuestSlotIn, NodeIn, QuestsTrackIN);

	//Dispatcher state.
	OnNodeChangedState.Broadcast(QuestSlotIn, NodeIn, IsDialogIn, EQuestTaskState::Locked);

	return true;
}

bool UQuestManager::LockNodesInBetweenNodes(int QuestSlotIn, const FQuestTaskID& StartNodeIn, const FQuestTaskID& EndNodeIn, bool IsDialogIN)
{
	bool succes = false;

	if (IsDialogIN)
	{
		//Dialog
		succes = LockNodesInBetweenNodesWithArrays(QuestSlotIn, IsDialogIN, StartNodeIn, EndNodeIn, DialogsInventory, DialogsTrack, LocalObjectsDialogsTrack);
	}
	else
	{
		//Quest
		succes = LockNodesInBetweenNodesWithArrays(QuestSlotIn, IsDialogIN, StartNodeIn, EndNodeIn, QuestInventory, QuestsTrack, LocalObjectsQuestsTrack);
	}

	return succes;
}

bool UQuestManager::LockNodesInBetweenNodesWithArrays(int QuestSlotIn, bool IsDialogIn, const FQuestTaskID& StartNodeIn, const FQuestTaskID& EndNodeIn, TArray<FQuestRep>& QuestsInventoryIN, TArray<FQuestTrack>& QuestsTrackIN, TArray<FQuestObjectTrack>& ObjectsTrackIN)
{
	if (!QuestsInventoryIN.IsValidIndex(QuestSlotIn)) return false;
	const FQuest& QuestLocal = UQuestsFunctionLibrary::QuestRepToQuest(QuestsInventoryIN[QuestSlotIn]);

	//are nodes not valid? The task fails
	if (!UQuestsFunctionLibrary::IsTaskIDValidInQuest(QuestLocal, StartNodeIn)) return false;
	if (!UQuestsFunctionLibrary::IsTaskIDValidInQuest(QuestLocal, EndNodeIn)) return false;

	//same node? Handle here. No need to propagate
	if (StartNodeIn == EndNodeIn)
	{
		SingleNodeLock(QuestSlotIn, QuestLocal, StartNodeIn, IsDialogIn, QuestsTrackIN, ObjectsTrackIN);
		return true;
	}

	//Get all nodes connected between the nodes that are not locked. This DOES NOT includes the start node. Not the end node
	TArray<FQuestTaskID> NodesToLock = GetAllNotLockedNodesBetweenNodes(QuestSlotIn, QuestLocal, StartNodeIn, EndNodeIn, false, QuestsTrackIN);

	//Add start and end nodes if they havent been added yet.
	NodesToLock.AddUnique(StartNodeIn);
	NodesToLock.AddUnique(EndNodeIn);

	//Lock nodes
	for (int i = 0; i < NodesToLock.Num(); i++)
	{
		SingleNodeLock(QuestSlotIn, QuestLocal, NodesToLock[i], IsDialogIn, QuestsTrackIN, ObjectsTrackIN);
	}

	return true;
}

TArray<FQuestTaskID> UQuestManager::GetAllNotLockedNodesBetweenNodes(int QuestSlotIn, const FQuest& QuestIn, const FQuestTaskID& StartNodeIn, const FQuestTaskID& EndNodeIn, bool IncludeDisabledNodesIn, const TArray<FQuestTrack>& QuestsTrackIN)
{
	//------------------------------------------Superior nodes from active texts. First loop 
	TArray <FQuestTaskID> SuperiorNodesA; //all superior initial nodes 
	TArray <FQuestTaskID> SuperiorNodesB; //nodes for next loop
	TArray <FQuestTaskID> AnalizedNodes; //track the ones already used
	TArray <FQuestTaskID> DisabledNodes; //track the disabled ones of the analized nodes
	TArray <FQuestTaskID> LockedNodes;

	//---------------------------------------All initial nodes 	

	//Start with all sup nodes of start node
	SuperiorNodesA = UQuestsFunctionLibrary::GetNodeSuperiorNodesInQuest(QuestIn, StartNodeIn);

	//If the Start node has disabled nodes, i need to take them into consideration, it wont be done in the while loop otherwise.
	//If track disabled nodes, need to add them to a array.
	if (IncludeDisabledNodesIn)
	{
		//Add to array that never resets
		DisabledNodes = UQuestsFunctionLibrary::GetNodeDisabledNodesInQuest(QuestIn, StartNodeIn);
		DisabledNodes.Append(UQuestsFunctionLibrary::GetNodeDisabledNodesInQuest(QuestIn, EndNodeIn)); //Also add end nodes disableds. Some conections may need it.
	}
	
	//Secondary loop nodes
	SuperiorNodesB.Empty();

	//keep track to avoid doble check 
	AnalizedNodes.Empty();

	//Starting node is also added, is the first one.
	AnalizedNodes.Add(StartNodeIn);

	bool WhileEnable = true;
	int32 LoopCounter = 0;

	//If the first node contains the end in its superior nodes, there is no need to loop. In this case the only nodes used are the disableds of the start node.
	if (SuperiorNodesB.Contains(EndNodeIn)) WhileEnable = false;

	while (WhileEnable && LoopCounter < MaxWhileLoopCount)
	{
		LoopCounter++;
		if (LoopCounter == MaxWhileLoopCount) 	UE_LOG(QuestEditorLog, Error, TEXT("UQuestManager::GetAllNotLockedNodesBetweenNodes - While Max Loop reached"));

		//Check all A nodes
		for (int i = 0; i < SuperiorNodesA.Num(); i++)
		{
			if (!AnalizedNodes.Contains(SuperiorNodesA[i]))
			{
				//Node Is not locked?
				if (GetNodeStateInTrackWithArrays(QuestSlotIn, SuperiorNodesA[i], QuestsTrackIN) != EQuestTaskState::Locked)
				{
					//Add to array out
					LockedNodes.AddUnique(SuperiorNodesA[i]);
					
					//Add to Array B all superior nodes
					//This might add duplicated nodes. This will need a filter later
					SuperiorNodesB.Append(UQuestsFunctionLibrary::GetNodeSuperiorNodesInQuest(QuestIn, SuperiorNodesA[i]));

					//If track disabled nodes, need to add them to a array. This nodes do not include their superior nodes to the propagation.
					if (IncludeDisabledNodesIn)
					{
						//Add to array that never resets
						DisabledNodes.Append(UQuestsFunctionLibrary::GetNodeDisabledNodesInQuest(QuestIn, SuperiorNodesA[i]));
					}					
				}
			}
			//If node is locked or not doesnt matter, its added to analizaed array to avoid doble check
			AnalizedNodes.AddUnique(SuperiorNodesA[i]);
		}
				
		//If the node END was found in the next loop, do not continue
		if (SuperiorNodesB.Contains(EndNodeIn)) WhileEnable = false;
		//If the node END was analized in this loop
		else if (AnalizedNodes.Contains(EndNodeIn)) WhileEnable = false;
		//if B is empty cancel loop
		else if (SuperiorNodesB.Num() == 0 ) WhileEnable = false;
		//Prepare for next loop
		else
		{
			SuperiorNodesA = SuperiorNodesB;
			SuperiorNodesB.Empty();
		}
	}

	//To the locked nodes, i will add the disabled ones
	if (IncludeDisabledNodesIn) LockedNodes.Append(DisabledNodes);

	//Remove duplicated nodes
	TArray <FQuestTaskID> FilteredNodes;

	for (int i = 0; i < LockedNodes.Num(); i++)
	{
		FilteredNodes.AddUnique(LockedNodes[i]);
	}

	return FilteredNodes;
}

TArray<FQuestTaskID> UQuestManager::GetTasksExclusivelyLinkedToNode(int QuestSlotIn, const FQuest& QuestIn, const FQuestTaskID& NodeIn, const TArray<FQuestTrack>& QuestsTrackIN) const
{
	//------------------------------------------Superior nodes from active texts. First loop 	
	TArray <FQuestTaskID> SuperiorNodesA; //all superior initial nodes 
	TArray <FQuestTaskID> SuperiorNodesB;
	TArray <FQuestTaskID> AnalizedNodes;
	TArray <FQuestTaskID> ExclusiveDisabledNodes;

	//---------------------------------------All initial nodes 	
	
	//Start with all sup nodes of node
	SuperiorNodesA = UQuestsFunctionLibrary::GetNodeSuperiorNodesInQuest(QuestIn, NodeIn);

	//Secondary loop nodes
	SuperiorNodesB.Empty();

	//keep track to avoid doble check 
	AnalizedNodes.Empty();

	//Starting node is also added, is the first one.
	AnalizedNodes.Add(NodeIn);

	bool WhileEnable = true;

	int32 LoopCounter = 0;
	while (WhileEnable && LoopCounter < MaxWhileLoopCount)
	{
		LoopCounter++;
		if (LoopCounter == MaxWhileLoopCount) 	UE_LOG(QuestEditorLog, Error, TEXT("UQuestManager::GetTasksExclusivelyLinkedToNode - While Max Loop reached"));

		//Check all A nodes
		for (int i = 0; i < SuperiorNodesA.Num(); i++)
		{
			if (!AnalizedNodes.Contains(SuperiorNodesA[i]))
			{
				//Node Is exclusive?
				if (IsNodeExclusive(QuestSlotIn, QuestIn, SuperiorNodesA[i], NodeIn, QuestsTrackIN))
				{				
					//Add to array out
					ExclusiveDisabledNodes.AddUnique(SuperiorNodesA[i]);
					//Add to Array B all superior nodes
					//This might add duplicated nodes. This will need a filter later
					SuperiorNodesB.Append(UQuestsFunctionLibrary::GetNodeSuperiorNodesInQuest(QuestIn, SuperiorNodesA[i]));
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

	//Remove duplicated nodes
	TArray <FQuestTaskID> FilteredNodes;

	for (int i = 0; i < ExclusiveDisabledNodes.Num(); i++)
	{
		FilteredNodes.AddUnique(ExclusiveDisabledNodes[i]);
	}

	return FilteredNodes;
}

bool UQuestManager::IsNodeExclusive(int QuestSlotIn, const FQuest& QuestIn, const FQuestTaskID& NodeIn, const FQuestTaskID& BottomNodeIn, const TArray<FQuestTrack>& QuestsTrackIN) const
{
	EQuestTaskState NodeState = GetNodeStateInTrackWithArrays(QuestSlotIn, NodeIn, QuestsTrackIN);

	//If node itself is not active (this should not happen) then the node is already a dead end and is not exclusive. Break the chain.
	if (NodeState == EQuestTaskState::Completed || NodeState == EQuestTaskState::Disabled || NodeState == EQuestTaskState::Failed) return false;

	TArray <FQuestTaskID> InferiorNodes = UQuestsFunctionLibrary::GetNodeInferiorNodesInQuest(QuestIn, NodeIn);
	
	//If there is only one inferior node it means it connects exclusively to the one that is beign tested and there is no need to go further.
	if (InferiorNodes.Num() <= 1) return true;
	
	//If there are more inferior nodes. 
	//Make sure that only one of them is capable of activation (Active, PreActive or Locked are all capable of activation.)
	//Also need to make a exception for the bottom node, because this node is already completed when this operation takes place, but is actually considered a path already
	int PossiblePaths = 0;
	bool Result = true;

	for (int i = 0; i < InferiorNodes.Num(); i++)
	{
		//Get State Of inferior node
		NodeState = GetNodeStateInTrackWithArrays(QuestSlotIn, InferiorNodes[i], QuestsTrackIN);

		//This node is a valid path? OR The node is the bottom node (that is already completed)
		if ((NodeState == EQuestTaskState::Active || NodeState == EQuestTaskState::PreActive || NodeState == EQuestTaskState::Locked) || (InferiorNodes[i] == BottomNodeIn))
		{
			//Increase possible paths
			PossiblePaths = PossiblePaths + 1;
			//If there are more paths than 1 the node is not exclusive and there is no need to go further
			if (PossiblePaths > 1)
			{
				//Node is not exclusive
				Result = false;
				break;
			}
		}
	}
	
	return Result;
}

bool UQuestManager::CompleteNode(int QuestSlotIn, bool IsDialogTaskIN, const FQuest& QuestIn, const FQuestTaskID& NodeIn, TArray<FQuestRep>& QuestsInventoryIN, TArray<FQuestTrack>& QuestsTrackIN, TArray<FQuestObjectTrack>& ObjectsTrackIN, TArray<FCompletedQuestTrack>& CompletedQuestsTrackIN, TArray<FQuestFailTrack>& QuestsFailTrackIN)
{
	//DebugOnly
	//SCOPE_CYCLE_COUNTER(STAT_CompleteNode);

	//UE_LOG(QuestEditorLog, Error, TEXT("UQuestManager::CompleteNode"));
	//TRACE_CPUPROFILER_EVENT_SCOPE(TEXT("UQuestManager::CompleteNode"));

	//Check if is active, only can complete a active node
	if (!(GetNodeStateInTrackWithArrays(QuestSlotIn, NodeIn, QuestsTrackIN) == EQuestTaskState::Active))
	{
		//Log
		UE_LOG(QuestEditorLog, Warning, TEXT("UQuestManager::CompleteNode - Task %i of quest %i cannot complete. State is not active."), NodeIn.TaskID, QuestSlotIn);
		return false;
	}

	//-----------------------Quest slot correction, in case tasks change their value modifying array lenghts
	//Save a local value of the slot.
	int QuestSlotLocal = QuestSlotIn;

	//Save the quest rep before executing
	FQuestRep QuestRepBeforeInferiorNodes = QuestsInventoryIN[QuestSlotLocal];

	//If this node completion is triggered from inferior nodes activation from previous CompleteNode execution, might need to correct slot.
	QuestSlotLocal = CorrectQuestSlotForNodeExecution(QuestSlotLocal, IsDialogTaskIN, QuestRepBeforeInferiorNodes, QuestsInventoryIN);

	//----------------Node start handling operation. This node is responsible for main checks and wont allow other nodes
	NodeStartHandlingOperation(QuestSlotLocal, NodeIn, QuestsTrackIN);

	//-----------------------------------------Save previous active array for conversation check	
	TArray <FQuestTaskID> PreviousConversationsIDS = QuestsTrackIN[QuestSlotLocal].ActiveTasks;

	//-----------------------------------------------Change state to completed
	SetNodeStateInTrack(QuestSlotLocal, NodeIn, EQuestTaskState::Completed, QuestsTrackIN);
	OnNodeChangedState.Broadcast(QuestSlotLocal, NodeIn, IsDialogTaskIN, EQuestTaskState::Completed);

	//Completion order
	SetNodeCompletionOrderInTrack(QuestSlotLocal, NodeIn, GetCurrentCompletionOrder(QuestSlotLocal, QuestsTrackIN) + 1, QuestsTrackIN);

	//---------------------------Remove Self from active track---------------
	RemoveTaskIDFromActiveTrack(QuestSlotLocal, NodeIn, QuestsTrackIN, ObjectsTrackIN);
	
	//Dispatchers
	switch (NodeIn.TaskType)
	{
	case EQuestTaskType::Task:
	{
		//Dispatcher
		if (IsDialogTaskIN)
		{
			DialogTaskCompleted.Broadcast(QuestSlotLocal, NodeIn.TaskID, QuestIn.Tasks[NodeIn.TaskID]);
		}
		else
		{
			QuestTaskCompleted.Broadcast(QuestSlotLocal, NodeIn.TaskID, QuestIn.Tasks[NodeIn.TaskID]);
		}		
	}
	break;
	case EQuestTaskType::TalkTask:
	{
		//Dispatcher
		if (IsDialogTaskIN)
		{
			DialogTalkTaskCompleted.Broadcast(QuestSlotLocal, NodeIn.TaskID, QuestIn.TalkTasks[NodeIn.TaskID]);
		}
		else
		{
			QuestTalkTaskCompleted.Broadcast(QuestSlotLocal, NodeIn.TaskID, QuestIn.TalkTasks[NodeIn.TaskID]);
		}
	}
	break;
	default:
		break;
	}		
	//------------------------dispatchers
	QTaskCompleted.Broadcast(QuestSlotLocal, NodeIn, IsDialogTaskIN);
	if (GetNetMode() != NM_DedicatedServer)
	{
		Client_QTaskCompleted.Broadcast(QuestSlotLocal, NodeIn, IsDialogTaskIN);
		Client_QTaskIsNoLongerActive.Broadcast(QuestSlotLocal, NodeIn, IsDialogTaskIN);
	}
	
	//----------------------------Disabled Nodes that are exclusively connected-----------
	//Find and disable exclusive conected nodes
	DeactivateNodes(QuestSlotLocal, GetTasksExclusivelyLinkedToNode(QuestSlotLocal, QuestIn, NodeIn, QuestsTrackIN), IsDialogTaskIN, QuestsTrackIN, ObjectsTrackIN);

	//--------------------------Disabled Nodes------------------------------
	DeactivateNodes(QuestSlotLocal, UQuestsFunctionLibrary::GetNodeDisabledNodesInQuest(QuestIn, NodeIn), IsDialogTaskIN, QuestsTrackIN, ObjectsTrackIN);

	//--------------------------Completion check--------------------------------------
	//Check if completes quest
	TArray <FQuestTaskID> InferiorNodes = UQuestsFunctionLibrary::GetNodeInferiorNodesInQuest(QuestIn, NodeIn);
	if (InferiorNodes.Num() == 0)
	{
		//---------------------------Conversations check for ends
		//Before Quest Completed
		if (NodeIn.TaskType == EQuestTaskType::TalkTask) CheckConversationStartAndEnd(QuestSlotLocal, QuestIn, IsDialogTaskIN, PreviousConversationsIDS, QuestsTrackIN);

		//Disable all active nodes. All nodes that couldnt be completed are disabled, no active tasks remain in track
		DeactivateNodes(QuestSlotLocal, QuestsTrackIN[QuestSlotLocal].ActiveTasks, IsDialogTaskIN, QuestsTrackIN, ObjectsTrackIN);

		//MARK to Complete Quest in node handler
		QuestsTrackIN[QuestSlotLocal].bIsPendingCompletion = true;
		
	}
	//Activate next nodes and add them to active tasks track
	else
	{
		//-------------------------Activate inferior nodes
		AttempToActivateInferiorNodes(QuestSlotLocal, IsDialogTaskIN, QuestIn, false, NodeIn, InferiorNodes, QuestsInventoryIN, QuestsTrackIN, ObjectsTrackIN, CompletedQuestsTrackIN, QuestsFailTrackIN);

		//If there is a task in the execution, that changes the lenght of arrays (leaves a quest for example), the QuestSlot might need to be recalculated here.
		QuestSlotLocal = CorrectQuestSlotForNodeExecution(QuestSlotLocal, IsDialogTaskIN, QuestRepBeforeInferiorNodes, QuestsInventoryIN);

		//The operation handler node can execute all the post execution logic
		if (IsNodeTheOperationHandler(QuestSlotLocal, NodeIn, QuestsTrackIN))
		{
			//---------------------------Conversations check for ends 
			if (NodeIn.TaskType == EQuestTaskType::TalkTask) CheckConversationStartAndEnd(QuestSlotLocal, QuestIn, IsDialogTaskIN, PreviousConversationsIDS, QuestsTrackIN);

			//Handle quest fail if needed
			if (QuestShouldFail(QuestSlotLocal, QuestsTrackIN))
			{
				//if quest fails cancel everything else
				if (HandleQuestFail(QuestSlotLocal, QuestIn, IsDialogTaskIN, QuestsInventoryIN, QuestsTrackIN, ObjectsTrackIN, QuestsFailTrackIN)) return false;
			}
		}	
	}
	//---------------------------Complete task object logic
	//Task
	if (NodeIn.TaskType == EQuestTaskType::Task)
	{
		//Object valid? (activate cond only dont execute this)
		if (ObjectsTrackIN[QuestSlotLocal].ObjectTasksTrack[NodeIn.TaskID].BaseTaskReference != nullptr)
		{
			//Disable logic
			ObjectsTrackIN[QuestSlotLocal].ObjectTasksTrack[NodeIn.TaskID].BaseTaskReference->CompleteQuestTask();
		}		
	}

	//track output
	bool completesQuest = false;

	//----------------Node stops operation handling
	//if need at end of execution
	//Need to track manually to ensure it works properly
	bool NodeWasHandler = false;

	if (IsNodeTheOperationHandler(QuestSlotLocal, NodeIn, QuestsTrackIN))
	{
		NodeWasHandler = true;
		NodeStopHandlingOperation(QuestSlotLocal, NodeIn, QuestsTrackIN);
	}

	//------------------Complete quest logic
	//Was handling operation and quest IS MARKED to end?
	if (NodeWasHandler && QuestsTrackIN[QuestSlotLocal].bIsPendingCompletion)
	{
		CompleteQuest(QuestSlotLocal, QuestIn, IsDialogTaskIN, QuestsInventoryIN, QuestsTrackIN, ObjectsTrackIN, CompletedQuestsTrackIN);
		completesQuest = true;
	}

	//Return if the node completed the quest.
	return completesQuest;
}

int UQuestManager::CorrectQuestSlotForNodeExecution(int QuestSlotIn, bool IsDialogTaskIN, const FQuestRep& QuestRepIn, const TArray<FQuestRep>& QuestsInventoryIN) const
{
	/*If eventually this continues to give problems, i can mark dirty instead of removing quests and remove at the end of execution*/

	//Set Slot local
	int QuestSlotLocal = QuestSlotIn; //This is the slot before correcting

	//If there is a task in the execution, that changes the lenght of arrays (leaves a quest for example), the QuestSlot might need to be recalculated here.
	if (!IsQuestSlotAfterNodeExecutionValid(QuestSlotLocal, IsDialogTaskIN, QuestRepIn, QuestsInventoryIN))
	{
		//Correct Quest Slot local with real value
		QuestSlotLocal = GetValidQuestSlotAfterNodeExecution(QuestRepIn, QuestsInventoryIN);
	}

	return QuestSlotLocal;
}

bool UQuestManager::IsQuestSlotAfterNodeExecutionValid(int QuestSlotIn, bool IsDialogTaskIN, const FQuestRep& QuestRepIn, const TArray<FQuestRep>& QuestsInventoryIN) const
{
	//Is not valid? this means that array got reduced
	if (!QuestsInventoryIN.IsValidIndex(QuestSlotIn)) return false;
	
	//If the quest in the slot is the same as the quest provided, then the quest is valid
	if (QuestsInventoryIN[QuestSlotIn] == QuestRepIn) return true;	//This is the only possible way that the slot is valid.
	
	//Every other case is invalid slot.
	return false;
}

int UQuestManager::GetValidQuestSlotAfterNodeExecution(const FQuestRep& QuestRepIn, const TArray<FQuestRep>& QuestsInventoryIN) const
{
	int LocalQuestSlot = -1;

	//Find the quest in the inventory
	for (int i = 0; i < QuestsInventoryIN.Num(); i++)
	{
		//Is this the quest?
		if (QuestsInventoryIN[i] == QuestRepIn)
		{
			//save slot
			LocalQuestSlot = i;

			//stop searching
			break;
		}
	}

	return LocalQuestSlot;
}

int UQuestManager::FindTalkTextSlotOfTask(int QuestIn, int TalkTaskSlotIN, AActor* NPCIN, bool IsDialogIn, FTalkTextInfo& TalkInfoOut) const
{
	//Get quest
	FQuest QuestLocal = GetQuestsInSlot(EQuestSelectedType::Active, QuestIn, IsDialogIn);

	//Get talk texts
	TArray <FTalkTextInfo> TalkTexts = GetTalkTextsInfo(NPCIN);
	
	int TextInfoOut=0;

	for (int i = 0; i < TalkTexts.Num(); i++)
	{
		if (TalkTexts[i].PlayerDialog.ToString() == QuestLocal.TalkTasks[TalkTaskSlotIN].PlayerDialog.ToString() && TalkTexts[i].NPCResponse.ToString() == QuestLocal.TalkTasks[TalkTaskSlotIN].NPCResponse.ToString())
		{
			TalkInfoOut = TalkTexts[i];
			TextInfoOut = i;
			break;
		}
	}

	return TextInfoOut;
}

TArray<int> UQuestManager::FindReadedTalkTasks(int QuestSlotIn, const FQuest& QuestIn, const TArray <int>& ActiveTalkTasksIn, const FNPCQuestDefRep& NPCIN, const TArray<FQuestTrack>& QuestsTrackIN) const
{
	if (!QuestsTrackIN.IsValidIndex(QuestSlotIn)) return TArray<int>();

	//------------------------------------------Superior nodes of active text first loop
	FQuestTaskID TaskTalkID;
	TArray <FQuestTaskID> SuperiorNodesA; //all superior initial nodes 
	TArray <FQuestTaskID> ReadedNodes;
	TArray <FQuestTaskID> SuperiorNodesB;

	//---------------------------------------All superior nodes
	//All superior nodes of all quests
	TaskTalkID.TaskType = EQuestTaskType::TalkTask; //they are all talk tasks
	
	for (int i = 0; i < ActiveTalkTasksIn.Num(); i++)
	{
		TaskTalkID.TaskID = ActiveTalkTasksIn[i];		
		ReadedNodes.Append(UQuestsFunctionLibrary::GetNodeSuperiorNodesInQuest(QuestIn, TaskTalkID)); //temporal use of array to make the operation
	}
	
	//filter to obtain UNIQUE only
	for (int i = 0; i < ReadedNodes.Num(); i++)
	{
		SuperiorNodesA.AddUnique(ReadedNodes[i]);
	}
	
	SuperiorNodesB.Empty();
	ReadedNodes.Empty();

	bool WhileEnable = true;

	int32 LoopCounter = 0;
	while (WhileEnable && LoopCounter < MaxWhileLoopCount)
	{
		LoopCounter++;
		if (LoopCounter == MaxWhileLoopCount) 	UE_LOG(QuestEditorLog, Error, TEXT("UQuestManager::FindReadedTalkTasks - While Max Loop reached"));

		//Check  all superior nodes A		
		for (int i = 0; i < SuperiorNodesA.Num(); i++)
		{
			//talk task?
			if (SuperiorNodesA[i].TaskType == EQuestTaskType::TalkTask)
			{
				//readed?
				if(QuestsTrackIN[QuestSlotIn].TalkTasksTracks[SuperiorNodesA[i].TaskID].State == EQuestTaskState::Completed)
				{
					//Same NPC?
					if (QuestIn.TalkTasks[SuperiorNodesA[i].TaskID].TalkNPC == NPCIN)
					{
						//add to readed array
						ReadedNodes.AddUnique(SuperiorNodesA[i]);

						//add its superior nodes to array B to check on next loop
						SuperiorNodesB.Append(UQuestsFunctionLibrary::GetNodeSuperiorNodesInQuest(QuestIn, SuperiorNodesA[i]));
					}
				}				
			}			
		}

		//If B is empty cancels while
		if (SuperiorNodesB.Num() == 0) WhileEnable = false;		
		//Clean arrays for next loop
		else
		{
			SuperiorNodesA = SuperiorNodesB;
			SuperiorNodesB.Empty();
		}
	}

	//Filter uniques only
	TArray <int> FilteredNodes;

	for (int i = 0; i < ReadedNodes.Num(); i++)
	{
		FilteredNodes.AddUnique(ReadedNodes[i].TaskID);
	}

	return FilteredNodes;
}

FQuestTalkTask UQuestManager::GetTalkTask(EQuestSelectedType SelectType, int QuestIN, int TaskIn, bool IsDialogIn) const
{
	FQuestTalkTask TaskLocal = FQuestTalkTask();
	
	//get quest
	FQuest QuestLocal = GetQuestsInSlot(SelectType, QuestIN, IsDialogIn);

	//invalid task
	if (!QuestLocal.TalkTasks.IsValidIndex(TaskIn)) return TaskLocal;

	TaskLocal = QuestLocal.TalkTasks[TaskIn];

	return TaskLocal;
}

FTalkTaskTrack UQuestManager::GetTalkTaskTrack(EQuestSelectedType SelectType, int QuestIN, int TaskIn, bool IsDialogIn) const
{
	FTalkTaskTrack TaskLocal = FTalkTaskTrack();
	
	//get quest
	FQuestTrack TrackLocal = GetQuestTrackInSlot(SelectType, QuestIN, IsDialogIn);

	//invalid task
	if (!TrackLocal.TalkTasksTracks.IsValidIndex(TaskIn)) return TaskLocal;

	TaskLocal = TrackLocal.TalkTasksTracks[TaskIn];

	return TaskLocal;
}

bool UQuestManager::RemoveNPCFromDialogTrack(AActor* NPCIN)
{
	//get NPC manager
	UNPCManager* NPCManagerComponent;
	NPCManagerComponent = Cast<UNPCManager>(NPCIN->FindComponentByClass(UNPCManager::StaticClass()));

	//NPC valid, save ID
	if (!NPCManagerComponent) return false;

	//Get Def
	FNPCQuestDefRep NPCLocal = NPCManagerComponent->GetNPCQuestDefRep();

	//Remove from def
	return RemoveNPCDefFromDialogTrack(NPCLocal);
}

bool UQuestManager::RemoveNPCDefFromDialogTrack(const FNPCQuestDefRep& NPCDefIn)
{
	FNPCQuestDef NPCInfo = UQuestsFunctionLibrary::NPCQuestDefRepToNPCQuestDef(NPCDefIn);

	//NPC dialogs
	TArray <FQuestRep> NPCDialogs = NPCInfo.Dialogs;

	int DialogPositionInInv = -1;

	//Abandon all dialogs
	for (int i = 0; i < NPCDialogs.Num(); i++)
	{
		//If this dialog is in the dialogs inventory abandon it.
		DialogPositionInInv = DialogsInventory.Find(NPCDialogs[i]);

		//Found it in array?
		if (DialogsInventory.IsValidIndex(DialogPositionInInv))			
		{
			//Remove quest from inventory
			RemoveQuestSlot(DialogPositionInInv, true, DialogsInventory, DialogsTrack, LocalObjectsDialogsTrack);		//This function is only for dialogs
		}
	}

	return true;
}

bool UQuestManager::ReachedNPC(AActor* NPCIN, bool CompleteFirstDialogIn)
{
	//Handle dialogs if can execute cosmetics
	if (GetNetMode() != NM_DedicatedServer)
	{
		//Save a NPC reference for convenience
		CurrentNPC = NPCIN;
		
		//NPC reach dispatcher, client only
		OnNPCReached.Broadcast(NPCIN);
		Client_OnNPCReached.Broadcast(NPCIN);
		
		//Should commit first dialog?
		if (CompleteFirstDialogIn)
		{
			//Handle first dialogs
			CommitFirstNPCDialog(NPCIN);
		}	
		else
		{
			//Force Update of talktext if did not commit a dialog to update talk tab anyways
			OnNPCTalkTextsChanged.Broadcast();
		}		
	}
	
	//Call NPC Reached on server if is not listen to set NPC reference
	if (GetNetMode() != NM_ListenServer)ROS_NPCReached(NPCIN);
	
	return true;
}

bool UQuestManager::CommitFirstNPCDialog(AActor* NPCIN)
{
	//invalid npc¿?
	if (NPCIN == nullptr) return false;

	//NPC manager
	UNPCManager* NPCManagerComponent = Cast<UNPCManager>(NPCIN->FindComponentByClass(UNPCManager::StaticClass()));

	//NPC null
	if (!NPCManagerComponent) return false;

	//Local vars
	bool foundTalkText = false;
	FTalkTextInfo TalkInfoLocal = FTalkTextInfo();

	//NPC Dialogs
	TArray <FQuestRep> NPCQuestRep = NPCManagerComponent->GetNPCDialogsRep();
	FNPCQuestDefRep NPCDefRep = NPCManagerComponent->GetNPCQuestDefRep();
	
	//Get talktexts for dialog only for this NPC
	TArray <FTalkTextInfo> DialogsTalkTexts = GetTalkTextsInfoWithArrays(true, NPCIN, NPCDefRep, NPCQuestRep, DialogsInventory, DialogsTrack);

	//Find from an Completed dialog, the last commited one. This happends when the dialog is not shown in talktexts because is no longer reelevant, but was completed previosly.
	//For all completed ones, find first one
	int LastCompletedTask = -1;

	TArray <int> AnalizedActiveQuests = TArray <int>(); //make sure i dont analize the same quest twice
	FQuest QuestLocal = FQuest();

	for (int i = 0; i < DialogsTalkTexts.Num(); i++)
	{
		//Is completed one?
		if (DialogsTalkTexts[i].Type == ETalkTextType::TalkTask && DialogsTalkTexts[i].State == EQuestTaskState::Completed)  //Is a completed node?
		{
			//Has not been analized
			if (!AnalizedActiveQuests.Contains(DialogsTalkTexts[i].QuestSlot))
			{
				//Get last completed talktask for this dialog
				LastCompletedTask = GetLastCompletedTalkTask(DialogsTalkTexts[i].QuestSlot, DialogsTrack);

				//found a completed talktask?
				if (LastCompletedTask != -1)
				{
					//Get quest
					QuestLocal = UQuestsFunctionLibrary::QuestRepToQuest(DialogsInventory[DialogsTalkTexts[i].QuestSlot]);

					//Make a fake talk text info that is temporal only for showing in UI and no commiting
					TalkInfoLocal.PlayerDialog = QuestLocal.TalkTasks[LastCompletedTask].PlayerDialog;
					TalkInfoLocal.NPCResponse = QuestLocal.TalkTasks[LastCompletedTask].NPCResponse;
					TalkInfoLocal.CustomPayloadData = QuestLocal.TalkTasks[LastCompletedTask].CustomPayloadData;
					TalkInfoLocal.PlayerSoundCue = QuestLocal.TalkTasks[LastCompletedTask].PlayerSoundCue;
					TalkInfoLocal.NPCSoundCue = QuestLocal.TalkTasks[LastCompletedTask].NPCSoundCue;
					TalkInfoLocal.State = EQuestTaskState::Completed;
					TalkInfoLocal.Type = ETalkTextType::TalkTask;
					TalkInfoLocal.QuestSlot = DialogsTalkTexts[i].QuestSlot;
					TalkInfoLocal.TaskSlot = LastCompletedTask;

					//Found
					foundTalkText = true;
					break;
				}
				else
				{
					//Add quest to analized to not repeat operations with different talktext.
					AnalizedActiveQuests.Add(DialogsTalkTexts[i].QuestSlot);
				}
			}			
		}
	}

	//Find one preactive and commit it
	if (!foundTalkText)
	{
		for (int i = 0; i < DialogsTalkTexts.Num(); i++)
		{
			//Is a preactive node?
			if (DialogsTalkTexts[i].Type == ETalkTextType::PreActivateTalkTask)
			{
				//Set talk text
				TalkInfoLocal = DialogsTalkTexts[i];

				//Found
				foundTalkText = true;
				break;
			}
		}
	}

	//Find a dialog that can be accepted
	if (!foundTalkText)
	{
		//Get all accept talktext for dialogs
		for (int i = 0; i < DialogsTalkTexts.Num(); i++)
		{
			//Is accept quest?
			if (DialogsTalkTexts[i].Type == ETalkTextType::AcceptQuest)
			{
				//Set talk text
				TalkInfoLocal = DialogsTalkTexts[i];

				//Found
				foundTalkText = true;
				break;
			}
		}		
	}	

	bool ShouldResetTalkText = false;

	//Complete first if needed
	if (foundTalkText)
	{
		//Commit the talktext
		Pred_TalkToNPC(TalkInfoLocal, NPCIN);		
	}
	else
	{
		//Set a reseted talktext
		ShouldResetTalkText = true;

		//Log
		UE_LOG(QuestEditorLog, Log, TEXT("UQuestManager::CommitFirstNPCDialog - Cannot find a commitable talk text."));		
	}

	//Reset if needed
	if (ShouldResetTalkText)
	{
		//Set Talk Text to invalid one
		TalkInfoLocal = FTalkTextInfo();
		TalkInfoLocal.Mode = ETalkTextMode::Dialog;	
		TalkInfoLocal.QuestSlot = -1;
		TalkInfoLocal.TaskSlot = -1;
		SetActiveTalkText(TalkInfoLocal);

		//If the commit is not good then i need to update anyways
		OnNPCTalkTextsChanged.Broadcast();		
	}
	
	return true;
}

void UQuestManager::ResetNPCDialogsFromTask(int QuestSlotIn, bool IsDialogIn, const FNPCQuestDefRep& NPCDefIn)
{
	//NPC info
	FNPCQuestDef NPCInfo = UQuestsFunctionLibrary::NPCQuestDefRepToNPCQuestDef(NPCDefIn);

	//NPC dialogs
	TArray <FQuestRep> NPCDialogs = NPCInfo.Dialogs;

	//was called from dialog?
	if (IsDialogIn)
	{
		//Remove the quest itself from the "NPC" array. This means that the caller quest is not affected by this process.
		NPCDialogs.Remove(DialogsInventory[QuestSlotIn]); 
	}

	//Reset
	ResetNPCDialogsWithArray(NPCDialogs);
}

void UQuestManager::ResetNPCDialogs(AActor* NPCIN)
{
	//get NPC manager
	UNPCManager* NPCManagerComponent;
	NPCManagerComponent = Cast<UNPCManager>(NPCIN->FindComponentByClass(UNPCManager::StaticClass()));

	//NPC valid, save ID
	if (!NPCManagerComponent) return;

	//Get Def
	FNPCQuestDefRep NPCLocal = NPCManagerComponent->GetNPCQuestDefRep();

	//Remove from def
	ResetNPCDialogsFromDef(NPCLocal);
}

void UQuestManager::ResetNPCDialogsFromDef(const FNPCQuestDefRep& NPCDefIn)
{
	//NPC info
	FNPCQuestDef NPCInfo = UQuestsFunctionLibrary::NPCQuestDefRepToNPCQuestDef(NPCDefIn);

	//NPC dialogs
	TArray <FQuestRep> NPCDialogs = NPCInfo.Dialogs;

	//Reset
	ResetNPCDialogsWithArray(NPCDialogs);
}

void UQuestManager::ResetNPCDialogsWithArray(const TArray<FQuestRep>& DialogsArrayIn)
{
	//Local dialogs
	TArray<FQuestRep> DialogsLocal = DialogsArrayIn;
	TArray<FQuestRep> DialogsLocalSecondary = TArray<FQuestRep>();

	//No more to check?
	if (DialogsLocal.Num() == 0) return;

	//Remove from active track from last to first
	for (int i = DialogsLocal.Num()-1 ; i >= 0; i--)
	{
		//Abandon dialogs
		if (!AbandonQuestRep(true, DialogsLocal[i]))
		{
			//If did not abandon means should check further
			DialogsLocalSecondary.Add(DialogsLocal[i]);
		}		
	}

	//Update next loop
	DialogsLocal = DialogsLocalSecondary;
	DialogsLocalSecondary.Empty();

	//No more to check?
	if (DialogsLocal.Num() == 0) return;

	//Remove from completed track from last to first
	for (int i = DialogsLocal.Num()-1 ; i >= 0; i--)
	{
		//Remove each. I dont check if is completed since it would be more costly.
		if (!RemoveCompletedQuestRep(DialogsLocal[i], true))
		{
			//If did not abandon means should check further
			DialogsLocalSecondary.Add(DialogsLocal[i]);
		}
	}

	//Update next loop
	DialogsLocal = DialogsLocalSecondary;
	DialogsLocalSecondary.Empty();

	//No more to check?
	if (DialogsLocal.Num() == 0) return;

	//Remove from fail track from last to first
	for (int i = DialogsLocal.Num()-1; i >= 0; i--)
	{
		//Remove each. I dont check if is failed since it would be more costly.
		RemoveQuestFromFailTrack(true, DialogsLocal[i]);
	}
}

TArray<int> UQuestManager::GetActiveTalkTasksForNPC(int QuestSlotIn, const FQuest& QuestIn, const FNPCQuestDefRep& NPCDefIn, const TArray<FQuestTrack>& QuestTrackIN) const
{
	//---------------------------VARIABLES 	
	TArray <int> SlotsTasks = TArray <int>();
	   
	for (int i = 0; i < QuestTrackIN[QuestSlotIn].ActiveTasks.Num(); i++)
	{
		if (QuestTrackIN[QuestSlotIn].ActiveTasks[i].TaskType == EQuestTaskType::TalkTask)
		{
			//same NPC
			if (QuestIn.TalkTasks[QuestTrackIN[QuestSlotIn].ActiveTasks[i].TaskID].TalkNPC == NPCDefIn)
			{
				//add id to out var
				SlotsTasks.Add(QuestTrackIN[QuestSlotIn].ActiveTasks[i].TaskID);
			}
		}
	}

	return SlotsTasks;
}

TArray<int> UQuestManager::GetCompletedTalkTasksForNPC(int QuestSlotIn, const FQuest& QuestIn, const FNPCQuestDefRep& NPCDefIn, const TArray<FQuestTrack>& QuestTrackIN) const
{
	//---------------------------VARIABLES 	
	TArray <int> SlotsTasks = TArray <int>();

	for (int i = 0; i < QuestTrackIN[QuestSlotIn].TalkTasksTracks.Num(); i++)   //The order here is how it is set in the quest. I could order by completion, but would be more expensive
	{
		//completed state
		if (QuestTrackIN[QuestSlotIn].TalkTasksTracks[i].State == EQuestTaskState::Completed)
		{
			//same NPC 
			if (QuestIn.TalkTasks[i].TalkNPC == NPCDefIn)
			{
				//add id to out var
				SlotsTasks.Add(i);
			}
		}
	}

	return SlotsTasks;
}

TArray<FTalkTextInfo> UQuestManager::GetPreActiveTalkTextsOfQuest(int QuestSlotIn, bool IsDialogIN, const FQuest& QuestIn, const FNPCQuestDefRep& NPCDefIn, const TArray<FQuestTrack>& QuestTrackIN) const
{
	//Set mode before looping, is the same for all
	FTalkTextInfo TalkInfoLocal = FTalkTextInfo();
	
	if (IsDialogIN)
	{
		TalkInfoLocal.Mode = ETalkTextMode::Dialog;
	}
	else
	{
		TalkInfoLocal.Mode = ETalkTextMode::Quest;
	}

	TArray <FTalkTextInfo> AllTalkInfoLocalOUT = TArray <FTalkTextInfo>();

	//Get PRE active texts for this npc
	TArray <int> ActiveTalkTasks;
	ActiveTalkTasks = GetPreActiveTalkTasksSlotsFromTrack(QuestSlotIn, QuestIn, NPCDefIn, QuestTrackIN);

	for (int j = 0; j < ActiveTalkTasks.Num(); j++)
	{
		//---------------------------------PRE-ACTIVE 
		TalkInfoLocal.PlayerDialog = QuestIn.TalkTasks[ActiveTalkTasks[j]].PlayerDialog;
		TalkInfoLocal.NPCResponse = QuestIn.TalkTasks[ActiveTalkTasks[j]].NPCResponse;
		TalkInfoLocal.CustomPayloadData = QuestIn.TalkTasks[ActiveTalkTasks[j]].CustomPayloadData;
		TalkInfoLocal.PlayerSoundCue = QuestIn.TalkTasks[ActiveTalkTasks[j]].PlayerSoundCue;
		TalkInfoLocal.NPCSoundCue = QuestIn.TalkTasks[ActiveTalkTasks[j]].NPCSoundCue;
		TalkInfoLocal.State = EQuestTaskState::PreActive; //This is to show as non readed
		TalkInfoLocal.Type = ETalkTextType::PreActivateTalkTask;
		TalkInfoLocal.QuestSlot = QuestSlotIn;
		TalkInfoLocal.TaskSlot = ActiveTalkTasks[j];
		//add to array
		AllTalkInfoLocalOUT.Add(TalkInfoLocal);
	}
		
	return AllTalkInfoLocalOUT;
}

TArray<FTalkTextInfo> UQuestManager::GetAcceptQuestTalkTextsOfNPC(bool IsDialogIN, AActor* NPCIN, const TArray<FQuestRep>& NPCQuestsRepIN) const
{
	//Quests that can be accepted
	TArray <FTalkTextInfo> AllTalkInfoLocalOUT = TArray <FTalkTextInfo>();
	TArray <int> FirstTalkText = TArray <int>();

	//Set mode before looping, is the same for all
	FTalkTextInfo TalkInfoLocal = FTalkTextInfo();
	if (IsDialogIN)
	{
		TalkInfoLocal.Mode = ETalkTextMode::Dialog;
	}
	else
	{
		TalkInfoLocal.Mode = ETalkTextMode::Quest;
	}

	FQuest QuestLocal = FQuest();

	//Quests from vendor
	for (int i = 0; i < NPCQuestsRepIN.Num(); i++)
	{
		//can accept this quest?
		if (CanAcceptQuest(NPCQuestsRepIN[i], false, IsDialogIN, NPCIN, false))
		{
			//Gets all talk task initials, initials defaults and those propagated with ActivateConditionOnlyTasks
			QuestLocal = UQuestsFunctionLibrary::QuestRepToQuest(NPCQuestsRepIN[i]);
			FirstTalkText = GetStartingTalkTasksSlots(QuestLocal, NPCIN, false);

			//valid first talktext?
			for (int j = 0; j < FirstTalkText.Num(); j++)
			{
				//Add questdata
				TalkInfoLocal.Type = ETalkTextType::AcceptQuest;
				TalkInfoLocal.State = EQuestTaskState::Active;
				TalkInfoLocal.PlayerDialog = QuestLocal.TalkTasks[FirstTalkText[j]].PlayerDialog;
				TalkInfoLocal.NPCResponse = QuestLocal.TalkTasks[FirstTalkText[j]].NPCResponse;
				TalkInfoLocal.CustomPayloadData = QuestLocal.TalkTasks[FirstTalkText[j]].CustomPayloadData;
				TalkInfoLocal.PlayerSoundCue = QuestLocal.TalkTasks[FirstTalkText[j]].PlayerSoundCue;
				TalkInfoLocal.NPCSoundCue = QuestLocal.TalkTasks[FirstTalkText[j]].NPCSoundCue;
				//Slots for quest and task
				TalkInfoLocal.QuestSlot = i;
				TalkInfoLocal.TaskSlot = FirstTalkText[j];
				//add to array
				AllTalkInfoLocalOUT.Add(TalkInfoLocal);
			}
		}
	}

	return AllTalkInfoLocalOUT;
}

TArray<FTalkTextInfo> UQuestManager::GetActiveTalkTextsOfQuest(int QuestSlotIn, bool IsDialogIN, const FQuest& QuestIn,const FNPCQuestDefRep& NPCDefIn, const TArray<FQuestTrack>& QuestTrackIN) const
{
	//Set mode before looping, is the same for all
	FTalkTextInfo TalkInfoLocal = FTalkTextInfo();
	
	if (IsDialogIN)
	{
		TalkInfoLocal.Mode = ETalkTextMode::Dialog;
	}
	else
	{
		TalkInfoLocal.Mode = ETalkTextMode::Quest;
	}

	TArray <FTalkTextInfo> AllTalkInfoLocalOUT = TArray <FTalkTextInfo>();

	//-----------------------------------Completed
	if (ShowCompletedTalkTexts) //User can configure this to not show. Could increase performance.
	{
		//All Readed texts for this quest that are connected to a active talk
		TArray <int> ReadedTalkTasks = GetCompletedTalkTasksForNPC(QuestSlotIn, QuestIn, NPCDefIn, QuestTrackIN);

		for (int k = 0; k < ReadedTalkTasks.Num(); k++)
		{
			//add readed
			TalkInfoLocal.PlayerDialog = QuestIn.TalkTasks[ReadedTalkTasks[k]].PlayerDialog;
			TalkInfoLocal.NPCResponse = QuestIn.TalkTasks[ReadedTalkTasks[k]].NPCResponse;
			TalkInfoLocal.CustomPayloadData = QuestIn.TalkTasks[ReadedTalkTasks[k]].CustomPayloadData;
			TalkInfoLocal.PlayerSoundCue = QuestIn.TalkTasks[ReadedTalkTasks[k]].PlayerSoundCue;
			TalkInfoLocal.NPCSoundCue = QuestIn.TalkTasks[ReadedTalkTasks[k]].NPCSoundCue;
			TalkInfoLocal.State = EQuestTaskState::Completed;
			TalkInfoLocal.Type = ETalkTextType::TalkTask;
			TalkInfoLocal.QuestSlot = QuestSlotIn;
			TalkInfoLocal.TaskSlot = ReadedTalkTasks[k];
			//add to array
			AllTalkInfoLocalOUT.Add(TalkInfoLocal);
		}
	}	

	//-------------------------------------Active------
	//Get active texts for this npc
	TArray <int> ActiveTalkTasks = GetActiveTalkTasksForNPC(QuestSlotIn, QuestIn, NPCDefIn, QuestTrackIN);

	for (int j = 0; j < ActiveTalkTasks.Num(); j++)
	{
		//---------------------------------ACTIVE 
		TalkInfoLocal.PlayerDialog = QuestIn.TalkTasks[ActiveTalkTasks[j]].PlayerDialog;
		TalkInfoLocal.NPCResponse = QuestIn.TalkTasks[ActiveTalkTasks[j]].NPCResponse;
		TalkInfoLocal.CustomPayloadData = QuestIn.TalkTasks[ActiveTalkTasks[j]].CustomPayloadData;
		TalkInfoLocal.PlayerSoundCue = QuestIn.TalkTasks[ActiveTalkTasks[j]].PlayerSoundCue;
		TalkInfoLocal.NPCSoundCue = QuestIn.TalkTasks[ActiveTalkTasks[j]].NPCSoundCue;
		TalkInfoLocal.State = EQuestTaskState::Active;
		TalkInfoLocal.Type = ETalkTextType::TalkTask;
		TalkInfoLocal.QuestSlot = QuestSlotIn;
		TalkInfoLocal.TaskSlot = ActiveTalkTasks[j];
		//add to array
		AllTalkInfoLocalOUT.Add(TalkInfoLocal);
	}

	return AllTalkInfoLocalOUT;
}

TArray<FTalkTextInfo> UQuestManager::GetTalkTextsInfoWithArrays(bool IsDialogIN, AActor* NPCIN, const FNPCQuestDefRep& NPCDefIN, const TArray<FQuestRep>& NPCQuestsRepIN, const TArray<FQuestRep>& QuestInventoryIN, const TArray<FQuestTrack>& QuestsTrackIN) const
{
	//--------------------------------------------------------------------
	//--------------------------------------NEW QUESTS--------------------
	//--------------------------------------------------------------------
	//This are all of new quests for this NPC
	 TArray <FTalkTextInfo> AllTalkInfoLocalOUT = GetAcceptQuestTalkTextsOfNPC(IsDialogIN, NPCIN, NPCQuestsRepIN); //Set array directly

	//----------------------------------------------------------------------
	//----------------------------------Quests PRE active---------------
	//----------------------------------------------------------------------

	//PRE ACTIVE TALK TEXTS for all quest active, 
	for (int i = 0; i < QuestInventoryIN.Num(); i++)
	{
		AllTalkInfoLocalOUT.Append(GetPreActiveTalkTextsOfQuest(i, IsDialogIN, UQuestsFunctionLibrary::QuestRepToQuest(QuestInventoryIN[i]), NPCDefIN, QuestsTrackIN));
	}

	//----------------------------------------------------------------------
	//----------------------------------Quests active-------------------
	//----------------------------------------------------------------------

	//TALK TEXTS for all quest active or completed, does not include quest firsts.
	for (int i = 0; i < QuestInventoryIN.Num(); i++)
	{
		AllTalkInfoLocalOUT.Append(GetActiveTalkTextsOfQuest(i, IsDialogIN, UQuestsFunctionLibrary::QuestRepToQuest(QuestInventoryIN[i]), NPCDefIN, QuestsTrackIN));
	}

	return AllTalkInfoLocalOUT;
}

TArray <FTalkTextInfo> UQuestManager::GetTalkTextsInfo(AActor* NPCIN) const
{
	//-----------------------Get all npc texts
	TArray <FTalkTextInfo> AllTalkInfoLocalOUT = TArray <FTalkTextInfo>();

	if (NPCIN == nullptr) return AllTalkInfoLocalOUT;

	//---------------------------VARIABLES 	
	//NPC manager
	UNPCManager* NPCManagerComponent = Cast<UNPCManager>(NPCIN->FindComponentByClass(UNPCManager::StaticClass()));	
	
	//NPC null
	if (!NPCManagerComponent) return AllTalkInfoLocalOUT;
	
	//NPC
	FNPCQuestDefRep NPCLocal = NPCManagerComponent->GetNPCQuestDefRep();
		
	//Dialogs
	TArray <FQuestRep> NPCQuestRep = NPCManagerComponent->GetNPCDialogsRep();
	AllTalkInfoLocalOUT = GetTalkTextsInfoWithArrays(true, NPCIN, NPCLocal, NPCQuestRep, DialogsInventory, DialogsTrack);
	
	//Quest
	NPCQuestRep = NPCManagerComponent->GetNPCQuestsRep();
	AllTalkInfoLocalOUT.Append(GetTalkTextsInfoWithArrays(false, NPCIN, NPCLocal, NPCQuestRep, QuestInventory, QuestsTrack));
	
	return AllTalkInfoLocalOUT;
}

FTalkTextInfo UQuestManager::GetTalkTextInfoForTalkTask(AActor* NPCIN, int QuestSlotIn, int TalkTaskIDIn, bool IsDialogIN) const
{		
	//Find all talktexts info
	TArray <FTalkTextInfo> AllTalkTextsInfo = GetTalkTextsInfo(NPCIN);
	FTalkTextInfo CurrentTalkTextInfo = FTalkTextInfo();

	//Create the mode to compare
	ETalkTextMode LocalModeToCheck = ETalkTextMode::Quest;
	if (IsDialogIN) LocalModeToCheck = ETalkTextMode::Dialog;

	//Searches the Talk info in the array
	for (int i = 0; i < AllTalkTextsInfo.Num(); i++)
	{
		//Is same quest, task and mode?
		if (AllTalkTextsInfo[i].QuestSlot == QuestSlotIn && AllTalkTextsInfo[i].TaskSlot == TalkTaskIDIn && AllTalkTextsInfo[i].Mode == LocalModeToCheck)
		{
			CurrentTalkTextInfo = AllTalkTextsInfo[i];
		}
	}	
	   
	return CurrentTalkTextInfo;
}

bool UQuestManager::IsTalkTextsInfoValid(AActor* NPCIN, const FTalkTextInfo& TalkTextInfoIn) const
{
	TArray <FTalkTextInfo> TalkTexts = GetTalkTextsInfo(NPCIN);

	bool valid = false;

	for (int i = 0; i < TalkTexts.Num(); i++)
	{
		if (TalkTexts[i] == TalkTextInfoIn)
		{
			valid = true;
			break;
		}
	}

	return valid;
}

bool UQuestManager::AreTalkTextsExactlyEqual(AActor* NPCIN, const FTalkTextInfo& TalkTextInfoA, const FTalkTextInfo& TalkTextInfoB) const
{
	if (TalkTextInfoA != TalkTextInfoB) return false;
	if (TalkTextInfoA.QuestSlot != TalkTextInfoB.QuestSlot) return false;
	if (TalkTextInfoA.Type != TalkTextInfoB.Type) return false;
	if (TalkTextInfoA.Mode != TalkTextInfoB.Mode) return false;
	return (TalkTextInfoA.State == TalkTextInfoB.State);
}

void UQuestManager::Pred_TalkToNPC(const FTalkTextInfo& TalkTextInfoIn, AActor * NPCIN)
{
	//full on client	   
	//UI UPDATE
	SetActiveTalkText(TalkTextInfoIn);

	switch (TalkTextInfoIn.Mode)
	{
	case ETalkTextMode::Quest:
	{
		//Quests
		switch (TalkTextInfoIn.Type)
		{

		case ETalkTextType::PreActivateTalkTask:
			//Fall Through

		case ETalkTextType::TalkTask:
		{
			//Quest execution
			CompleteQuestTalkTask(TalkTextInfoIn.QuestSlot, TalkTextInfoIn.TaskSlot, false, NPCIN);

			//If not listen check again on server
			if (GetNetMode() == NM_Client)
			{
				ROS_CompleteQuestTalkTask(TalkTextInfoIn.QuestSlot, TalkTextInfoIn.TaskSlot, NPCIN);
			}
		}
		break;

		case ETalkTextType::AcceptQuest:
		{
			//Accept quest
			if (AcceptQuestFromTalk(NPCIN, TalkTextInfoIn.QuestSlot, TalkTextInfoIn.TaskSlot, false) == true && GetNetMode() == NM_Client)
			{
				ROS_AcceptQuestFromTalk(NPCIN, TalkTextInfoIn.QuestSlot, TalkTextInfoIn.TaskSlot);
			}
		}
		break;	
	
		default:
			break;
		}
	}
		break;
	case ETalkTextMode::Dialog:
	{
		//Dialogs
		switch (TalkTextInfoIn.Type)
		{

		case ETalkTextType::PreActivateTalkTask:
			//Fall Through

		case ETalkTextType::TalkTask:
		{
			//Dialog normal execution
			CompleteQuestTalkTask(TalkTextInfoIn.QuestSlot, TalkTextInfoIn.TaskSlot, true, NPCIN);
		}
		break;

		case ETalkTextType::AcceptQuest:
		{
			//Accept dialog
			AcceptQuestFromTalk(NPCIN, TalkTextInfoIn.QuestSlot, TalkTextInfoIn.TaskSlot, true);
		}
		break;

		default:
			break;
		}
	}
		break;
	default:
		break;
	}

	//Should try to accept dialogs?
	if (AutoAcceptDialogsIfPossible)
	{
		//talk text is invalid now?
		if (GetActiveTalkText().NPCResponse.ToString() == FString())
		{
			//Accept first dialogs
			CommitFirstNPCDialog(NPCIN);
		}
	}

	//Update Ui at the end.
	OnNPCTalkTextsChanged.Broadcast();
	return;	
}

TArray<FText> UQuestManager::GetAllActiveTalkTasksShortDesc(int QuestSlotIn, bool IsDialogIn) const
{
	//Get all short descriptions from active talk tasks
	TArray <FText> ShortDescriptions;
	ShortDescriptions.Empty();

	FText LocalDescription = FText();
	bool ShouldAdd = true;

	for (int i = 0; i < QuestsTrack[QuestSlotIn].ActiveTasks.Num(); i++)		//This is only valid for quests, cannot be used on dialogs.
	{
		//Convert to string to check if is added. Text has no comparing
		LocalDescription = GetTalkTasksInheritedShortDescription(QuestSlotIn, QuestsTrack[QuestSlotIn].ActiveTasks[i].TaskID, IsDialogIn); //This is only valid for quests, cannot be used on dialogs.

		ShouldAdd = true;
		for (int j = 0; j < ShortDescriptions.Num(); j++)
		{
			//If found same description do not add
			if (ShortDescriptions[j].ToString() == LocalDescription.ToString())
			{
				ShouldAdd = false;
				break;
			}
		}

		//is unique?
		if (ShouldAdd)
		{
			ShortDescriptions.Add(LocalDescription);
		}		
	}

	return ShortDescriptions;
}

bool UQuestManager::AreTalkTasksConnected(int QuestSlotIn, const FQuest& QuestIn, int TalkTaskA, int TalkTaskB) const
{
	//If both talk tasks are from the same conversation they are connected.
	//Dont care about their states
	int ConversationID = 0;
	TArray<FQuestTaskID> AllTaskConversations = UQuestsFunctionLibrary::GetAllTalkTasksOfConversation(QuestIn, TalkTaskA, ConversationID);

	//If B is in A. I know already that A is in the conversation cuz thats the conversation i got before. (A is sample task)
	FQuestTaskID TaskTalkID;
	TaskTalkID.TaskID = TalkTaskB;
	TaskTalkID.TaskType = EQuestTaskType::TalkTask;

	//Return value. If contains B, A is contained and both are contained. Same conv
	return AllTaskConversations.Contains(TaskTalkID);
}

FText UQuestManager::GetTalkTasksInheritedShortDescription(int QuestSlotIn, int TalkTaskIn, bool IsDialogIn) const
{
	//Propagates up, to find description overrides
	FQuest QuestLocal = GetQuestsInSlot(EQuestSelectedType::Active, QuestSlotIn, IsDialogIn);
	
	//Vars
	FQuestTaskID TaskTalkID = FQuestTaskID();
	TaskTalkID.TaskID = TalkTaskIn;
	TaskTalkID.TaskType = EQuestTaskType::TalkTask;
	
	//valid tasks?
	if (!UQuestsFunctionLibrary::IsTaskIDValidInQuest(QuestLocal, TaskTalkID)) return FText(); 

	//If this task has override already, dont need to propagate.
	if (QuestLocal.TalkTasks[TalkTaskIn].TaskShortDesc.ToString() != FString()) return QuestLocal.TalkTasks[TalkTaskIn].TaskShortDesc;

	//------------------------------------------Superior nodes of active text, first loop
	TArray <FQuestTaskID> SuperiorNodesA; //all superior initial nodes 
	TArray <FQuestTaskID> SuperiorNodesB;
	TArray <FQuestTaskID> AnalizedNodes;

	//--------------------------------------- superior initial nodes 
	FQuestTalkTask TaskInterna = QuestLocal.TalkTasks[TalkTaskIn];

	//Start with all sup nodes
	SuperiorNodesA.Append(UQuestsFunctionLibrary::ConvertLinkArrayToNodeArray(TaskInterna.SuperiorNodes));
	SuperiorNodesB.Empty();	

	FText ShortDesc = FText();

	//This ones are already analized
	AnalizedNodes = SuperiorNodesA;

	bool WhileEnable = true;
	int32 LoopCounter = 0;
	while (WhileEnable && LoopCounter < MaxWhileLoopCount)
	{
		LoopCounter++;
		if (LoopCounter == MaxWhileLoopCount) 	UE_LOG(QuestEditorLog, Error, TEXT("UQuestManager::GetTalkTasksInheritedShortDescription - While Max Loop reached"));

		//Chek A array
		for (int i = 0; i < SuperiorNodesA.Num(); i++)
		{
			//Dont check circular connections
			if (AnalizedNodes.Contains(SuperiorNodesA[i])) continue;

			//talk 
			if (SuperiorNodesA[i].TaskType == EQuestTaskType::TalkTask)
			{
				//same NPC
				if (QuestLocal.TalkTasks[SuperiorNodesA[i].TaskID].TalkNPC == TaskInterna.TalkNPC)
				{
					//Short desc is overriden?
					if (QuestLocal.TalkTasks[SuperiorNodesA[i].TaskID].TaskShortDesc.ToString() != FString())
					{
						//save short desc
						ShortDesc = QuestLocal.TalkTasks[SuperiorNodesA[i].TaskID].TaskShortDesc;
						
						//stop looping
						WhileEnable = false;
					}
					else
					{
						//Add superior nodes to continue looping
						SuperiorNodesB.Append(UQuestsFunctionLibrary::GetNodeSuperiorNodesInQuest(QuestLocal, SuperiorNodesA[i]));
					}
				}
			}

			//Track analized nodes
			AnalizedNodes.AddUnique(AnalizedNodes[i]);
		}
		//If should continue
		if (WhileEnable)
		{
			//Cancel loop?
			if (SuperiorNodesB.Num() == 0) WhileEnable = false;
			//Continue loop
			else
			{
				SuperiorNodesA = SuperiorNodesB;
				SuperiorNodesB.Empty();
			}
		}		
	}

	return ShortDesc;
}

TArray <int> UQuestManager::GetAllConversationsInTalkNodeArray(int QuestSlotIn, const FQuest& QuestIn, const TArray<FQuestTaskID>& NodeArray) const
{
	//Remove all nodes that have task
	//UE_LOG(QuestEditorLog, Error, TEXT("UQuestManager::GetAllConversationsInTalkNodeArray"));
	//TRACE_CPUPROFILER_EVENT_SCOPE(TEXT("UQuestManager::GetAllConversationsInTalkNodeArray"));

	//If there is a null array, no need to get all the map
	if (NodeArray.Num() == 0) return TArray <int>();
	
	//Out var
	TArray <int> ConversationsIDS = TArray <int>();

	//For each possible conversation in quest, determine if they are included in the nodes array
	for (int i = 0; i < NodeArray.Num(); i++)
	{
		//Is a talk task?
		if (NodeArray[i].TaskType == EQuestTaskType::TalkTask)
		{
			//Add new conversation ID if is unique
			ConversationsIDS.AddUnique(QuestIn.TalkTasks[NodeArray[i].TaskID].ConversationID);
		}		
	}

	//return all conversations IDs that are different.
	return  ConversationsIDS;
}

void UQuestManager::CheckConversationStartAndEnd(int QuestSlotIn, const FQuest& QuestIn, bool IsDialogIn, const TArray <FQuestTaskID>& PreviousActiveTrack, const TArray<FQuestTrack>& QuestTrackIN) const
{
	//UE_LOG(QuestEditorLog, Error, TEXT("UQuestManager::CheckConversationStartAndEnd"));
	//TRACE_CPUPROFILER_EVENT_SCOPE(TEXT("UQuestManager::CheckConversationStartAndEnd"));	
	
	//No need to check conversations on dedicated server with no UI.
	if (GetNetMode() == NM_DedicatedServer)
	{
		return;
	}

	//This function compares previous and actual quest track.
	//This function will determine all diferent Conversations for each array and detect wich are new and wich ended and no longer are in the array.

	if (!QuestTrackIN.IsValidIndex(QuestSlotIn))
	{
		//Log
		UE_LOG(QuestEditorLog, Warning, TEXT("UQuestManager::CheckConversationStartAndEnd - Quest has no valid track. QuestSlot: %i, Check conversation operation canceled."), QuestSlotIn);
		return;
	}

	TArray <int> PreviousConversationsIDS = GetAllConversationsInTalkNodeArray(QuestSlotIn, QuestIn, PreviousActiveTrack);
	
	//if the track is not valid then need to cancel the conversation check.
	TArray <int> ActiveConversationsIDS = GetAllConversationsInTalkNodeArray(QuestSlotIn, QuestIn, QuestTrackIN[QuestSlotIn].ActiveTasks);
	
	//Completed conversations, are those that are contained in the previous array but not on the current actives.
	for (int i = 0; i < PreviousConversationsIDS.Num(); i++)
	{
		if (!ActiveConversationsIDS.Contains(PreviousConversationsIDS[i]))
		{
			QuestConversationCompleted.Broadcast(QuestSlotIn, PreviousConversationsIDS[i], IsDialogIn, QuestIn.TalkTasks[PreviousConversationsIDS[i]]);
		}
	}

	//Started conversations, are those that are contained in the current active but not on the previous
	for (int i = 0; i < ActiveConversationsIDS.Num(); i++)
	{
		if (!PreviousConversationsIDS.Contains(ActiveConversationsIDS[i]))
		{
			QuestConversationActivated.Broadcast(QuestSlotIn, ActiveConversationsIDS[i], IsDialogIn, QuestIn.TalkTasks[ActiveConversationsIDS[i]]);
		}
	}

	return;
}

TArray<FQuestTalkTask> UQuestManager::GetTalkTextNodes(AActor* NPCIn, const FTalkTextInfo& LastNPCCommitedTalkTextsIn, ENodeConnectorType TypeIn, TArray<int>& TalkTasksIDsOut, FQuest& QuestOut)
{
	//Create ID
	FQuestTaskID NodeID = FQuestTaskID();
	NodeID.TaskID = LastNPCCommitedTalkTextsIn.TaskSlot;
	NodeID.TaskType = EQuestTaskType::TalkTask;

	//Get quest for this talk text
	FQuest QuestLocal = FQuest();

	if (LastNPCCommitedTalkTextsIn.Mode == ETalkTextMode::Dialog)
	{
		//For Dialogs
		if (LastNPCCommitedTalkTextsIn.Type == ETalkTextType::AcceptQuest)
		{
			//-------------------------------------------------If the dialog is not active, then we need to find it in the NPC.
			//-----------------------Get all npc quests
			if (NPCIn == nullptr) return TArray<FQuestTalkTask>();

			//---------------------------VARIABLES 	
			//NPC manager
			UNPCManager* NPCManagerComponent;
			NPCManagerComponent = Cast<UNPCManager>(NPCIn->FindComponentByClass(UNPCManager::StaticClass()));
			//NPC null
			if (!NPCManagerComponent) return TArray<FQuestTalkTask>();

			//Quest NPC Variables, all in local vars
			TArray <FQuest> NPCContainedQuests = NPCManagerComponent->GetNPCDialogs();

			//Quest in slot is the NPC one.
			if (!NPCContainedQuests.IsValidIndex(LastNPCCommitedTalkTextsIn.QuestSlot))  return TArray<FQuestTalkTask>();

			//Set quest local
			QuestLocal = NPCContainedQuests[LastNPCCommitedTalkTextsIn.QuestSlot];
		}
		else
		{
			//active Dialog is easy to find.
			if (DialogsInventory.IsValidIndex(LastNPCCommitedTalkTextsIn.QuestSlot))
			{
				QuestLocal = UQuestsFunctionLibrary::QuestRepToQuest(DialogsInventory[LastNPCCommitedTalkTextsIn.QuestSlot]);
			}			
		}
	}
	else
	{
		//For quests
		if (LastNPCCommitedTalkTextsIn.Type == ETalkTextType::AcceptQuest)
		{
			//-------------------------------------------------If the quest is not active, then we need to find it in the NPC.
			//-----------------------Get all npc quests
			if (NPCIn == nullptr) return TArray<FQuestTalkTask>();

			//---------------------------VARIABLES 	
			//NPC manager
			UNPCManager* NPCManagerComponent;
			NPCManagerComponent = Cast<UNPCManager>(NPCIn->FindComponentByClass(UNPCManager::StaticClass()));
			//NPC null
			if (!NPCManagerComponent) return TArray<FQuestTalkTask>();

			//Quest NPC Variables, all in local vars
			TArray <FQuest> NPCContainedQuests = NPCManagerComponent->GetNPCQuests();

			//Quest in slot is the NPC one.
			if (!NPCContainedQuests.IsValidIndex(LastNPCCommitedTalkTextsIn.QuestSlot))  return TArray<FQuestTalkTask>();

			//Set quest local
			QuestLocal = NPCContainedQuests[LastNPCCommitedTalkTextsIn.QuestSlot];
		}
		else
		{
			//active quest is easy to find.
			if (QuestInventory.IsValidIndex(LastNPCCommitedTalkTextsIn.QuestSlot))
			{
				QuestLocal = UQuestsFunctionLibrary::QuestRepToQuest(QuestInventory[LastNPCCommitedTalkTextsIn.QuestSlot]);
			}
		}
	}	
	
	//Is valid data?
	if (!UQuestsFunctionLibrary::IsTaskIDValidInQuest(QuestLocal, NodeID))  
	{
		//This ID is invalid.
		//Log
		UE_LOG(QuestEditorLog, Warning, TEXT("UQuestManager::GetTalkTextInferiorNodes - TalkTextInfo provided is invalid."));
		return TArray<FQuestTalkTask>();
	}

	//Nodes array
	TArray<FQuestTaskID> NodesLocal = TArray<FQuestTaskID>();

	switch (TypeIn)
	{
	case ENodeConnectorType::Inferior: 	NodesLocal = UQuestsFunctionLibrary::GetNodeInferiorNodesInQuest(QuestLocal, NodeID);
		break;
	case ENodeConnectorType::Superior: 	NodesLocal = UQuestsFunctionLibrary::GetNodeSuperiorNodesInQuest(QuestLocal, NodeID);
		break;
	case ENodeConnectorType::Disable: 	NodesLocal = UQuestsFunctionLibrary::GetNodeDisabledNodesInQuest(QuestLocal, NodeID);
		break;
	default:
		break;
	}
		
	TArray<FQuestTalkTask> NodesTalkTasks = TArray<FQuestTalkTask>();
	TArray<int> NodesTalkTasksIDs = TArray<int>();


	for (int i = 0; i < NodesLocal.Num(); i++)
	{
		//Is a talk task?
		if (NodesLocal[i].TaskType == EQuestTaskType::TalkTask)
		{
			//Add to array
			NodesTalkTasks.Add(QuestLocal.TalkTasks[NodesLocal[i].TaskID]);
			NodesTalkTasksIDs.Add(NodesLocal[i].TaskID);
		}
	}

	//out vars
	TalkTasksIDsOut = NodesTalkTasksIDs;
	QuestOut = QuestLocal;

	return NodesTalkTasks;
}

TArray <FTalkTextInfo> UQuestManager::GetSingleSentencePriorityTalkTextsForMode(AActor* NPCIn, const TArray<FTalkTextInfo>& ActiveTalkTextsIn, const ETalkTextMode& AllowedMode, const TArray<EQuestTaskState>& AllowedStates, const TArray<FSingleSentenceModeData>& SSDataIn, TArray <FSingleSentenceModeData>& SSDataOut)
{
	//No texts?
	if (ActiveTalkTextsIn.Num()==0) return TArray <FTalkTextInfo>();
	
	//This will search in order for talktexts, untill it finds options. Then will determine the quest slot that has lower ID.
	TArray <FSingleSentenceModeData> CurrentFilteredSSData = TArray <FSingleSentenceModeData>();
	
	//-----------------------------------------------------------------------------
	//-----------------Quest PLAYER ACTIVE TALK TEXTS
	//-----------------------------------------------------------------------------	
	//Define filters
	TArray<ETalkTextType> AllowedTypes = { ETalkTextType::TalkTask };
	TArray<EDialogType> AllowedSSTypes = {};//Any
	
	//Get all player active choices

	TArray <FTalkTextInfo> CurrentFilteredTalkTexts = UQuestsFunctionLibrary::FilterTalkTextsForSingleSentence(ActiveTalkTextsIn, SSDataIn, AllowedMode, AllowedStates, AllowedTypes, AllowedSSTypes, CurrentFilteredSSData);

	//Found something?
	if (CurrentFilteredTalkTexts.Num() > 0)
	{
		SSDataOut = CurrentFilteredSSData;
		return CurrentFilteredTalkTexts;
	}

	//-----------------------------------------------------------------------------
	//-----------------Quest PLAYER PRE-ACTIVE TALK TEXTS
	//-----------------------------------------------------------------------------
	//Define filters
	AllowedTypes = { ETalkTextType::PreActivateTalkTask };
	AllowedSSTypes = {};//Any

	//Get all player active choices
	CurrentFilteredTalkTexts = UQuestsFunctionLibrary::FilterTalkTextsForSingleSentence(ActiveTalkTextsIn, SSDataIn, AllowedMode, AllowedStates, AllowedTypes, AllowedSSTypes, CurrentFilteredSSData);

	//Found something?
	if (CurrentFilteredTalkTexts.Num() > 0)
	{
		SSDataOut = CurrentFilteredSSData;
		return CurrentFilteredTalkTexts;
	}

	//-----------------------------------------------------------------------------
	//-----------------Quest PLAYER NEW QUEST TALK TEXTS
	//-----------------------------------------------------------------------------
	//Define filters
	AllowedTypes = { ETalkTextType::AcceptQuest };
	AllowedSSTypes = {};//Any

	//Get all player active choices
	CurrentFilteredTalkTexts = UQuestsFunctionLibrary::FilterTalkTextsForSingleSentence(ActiveTalkTextsIn, SSDataIn, AllowedMode, AllowedStates, AllowedTypes, AllowedSSTypes, CurrentFilteredSSData);

	//Found something?
	if (CurrentFilteredTalkTexts.Num() > 0)
	{
		SSDataOut = CurrentFilteredSSData;
		return CurrentFilteredTalkTexts;
	}

	//At this point, this NPC has no talk text, should never happen.
	return TArray <FTalkTextInfo>();
}

TArray<FTalkTextInfo> UQuestManager::GetSingleSentencePriorityTalkTexts(AActor* NPCIn, const TArray<FTalkTextInfo>& ActiveTalkTextsIn, const TArray<EQuestTaskState>& AllowedStates, const TArray<FSingleSentenceModeData>& SSDataIn, TArray<FSingleSentenceModeData>& SSDataOut)
{
	//Find talktexts for quest first
	TArray<FSingleSentenceModeData> PrioritySSD = TArray<FSingleSentenceModeData>();
	TArray <FTalkTextInfo> PriorityTalkTexts = GetSingleSentencePriorityTalkTextsForMode(NPCIn, ActiveTalkTextsIn, ETalkTextMode::Quest, AllowedStates, SSDataIn, PrioritySSD);

	//Has not found anything for quests
	if (PriorityTalkTexts.Num() == 0)
	{
		//Clear arrays
		PrioritySSD = TArray<FSingleSentenceModeData>();
		PriorityTalkTexts = TArray <FTalkTextInfo>();

		//Find info for dialogs
		PriorityTalkTexts = GetSingleSentencePriorityTalkTextsForMode(NPCIn, ActiveTalkTextsIn, ETalkTextMode::Dialog, AllowedStates, SSDataIn, PrioritySSD);
	}

	//Data out
	SSDataOut = PrioritySSD;
	return PriorityTalkTexts;
}

int UQuestManager::GetTalkTextsLowerQuestSlot(const TArray<FTalkTextInfo>& ActiveTalkTextsIn)
{	
	//No talk texts? 
	if (ActiveTalkTextsIn.Num() <= 0) return -1;
	
	int LowerQuestSlot = 1000; //ridiculous high number

	//Does not matter the mode, all TTs are from same mode
	for (int i = 0; i < ActiveTalkTextsIn.Num(); i++)
	{
		if (ActiveTalkTextsIn[i].QuestSlot < LowerQuestSlot)
		{
			LowerQuestSlot = ActiveTalkTextsIn[i].QuestSlot;
		}		
	}

	return LowerQuestSlot;
}

TArray<FTalkTextInfo> UQuestManager::GetTalkTextsFilteredForSSD(AActor* NPCIn, const TArray<FTalkTextInfo>& ActiveTalkTextsIn, const TArray<EQuestTaskState>& AllowedStates, const TArray<FSingleSentenceModeData>& SSDataIn, EDialogType FilterTypeIn, TArray <FSingleSentenceModeData>& SSDataOut)
{
	//Get priority talk texts.
	TArray<FSingleSentenceModeData> PrioritySSD = TArray<FSingleSentenceModeData>();
	TArray <FTalkTextInfo> PriorityTalkTexts = GetSingleSentencePriorityTalkTexts(NPCIn, ActiveTalkTextsIn, AllowedStates, SSDataIn, PrioritySSD);

	//Get quest slot current. Only for current talktexts
	int CurrentQuestSlot = GetTalkTextsLowerQuestSlot(PriorityTalkTexts);

	//Get all talk texts with the desired Slot
	TArray <FTalkTextInfo> QuestSlotFilteredPriorityTalkTexts = TArray <FTalkTextInfo>();
	TArray<FSingleSentenceModeData> QuestSlotFilteredPrioritySSD = TArray<FSingleSentenceModeData>();

	for (int i = 0; i < PriorityTalkTexts.Num(); i++)
	{
		//Quest must match slot
		if (PriorityTalkTexts[i].QuestSlot == CurrentQuestSlot)
		{
			//Matches type?
			if (PrioritySSD[i].Type == FilterTypeIn)
			{
				QuestSlotFilteredPriorityTalkTexts.Add(PriorityTalkTexts[i]);
				QuestSlotFilteredPrioritySSD.Add(PrioritySSD[i]);
			}
		}		
	}

	//Out vars
	SSDataOut = QuestSlotFilteredPrioritySSD;
	return QuestSlotFilteredPriorityTalkTexts;
}

FTalkTextInfo UQuestManager::GetNPCLastCommitedTalkTextForSSD(AActor* NPCIn, const TArray <FTalkTextInfo>& ActiveTalkTextsIn, const TArray <FSingleSentenceModeData>& SSDataIn, FSingleSentenceModeData& SSDataOut, bool& FoundLastCommitedText)
{
	//With the quest slot, i could try to find the completion order and determine the last one.
	
	//Get priority talk texts for NPC ACTIVE only
	TArray<FSingleSentenceModeData> PrioritySSD = TArray<FSingleSentenceModeData>();

	TArray <FTalkTextInfo> PriorityTalkTexts = GetTalkTextsFilteredForSSD(NPCIn, ActiveTalkTextsIn, { EQuestTaskState::Active }, SSDataIn, EDialogType::NPC, PrioritySSD);

	//Nothing filtered?
	if (PriorityTalkTexts.Num() == 0)
	{
		//This means that there are no NPC Active talk tasks. I need to filter completed ones now.	
		PriorityTalkTexts = GetTalkTextsFilteredForSSD(NPCIn, ActiveTalkTextsIn, { EQuestTaskState::Completed }, SSDataIn, EDialogType::NPC, PrioritySSD);

		//If there is no completed either then
		if (PriorityTalkTexts.Num() == 0)
		{
			//Log
			UE_LOG(QuestEditorLog, Log, TEXT("UQuestManager::GetNPCLastCommitedTalkTextForSSD - Failed to find NPC last commited talktext. There is none. NPC Starts conversation."));
			FoundLastCommitedText = false;
			SSDataOut = FSingleSentenceModeData();
			return FTalkTextInfo();
		}

		//If there is one on the completed ones.
		if (PriorityTalkTexts.Num() == 1 && PrioritySSD.Num() == 1)
		{
			FoundLastCommitedText = true;
			SSDataOut = PrioritySSD[0];
			return PriorityTalkTexts[0];
		}

		//If there are more talk tasks. I need to filter by completion order and get the last. This is done at the end.
	}
	
	//Filtered down to 1 talk text? is this one then.
	if (PriorityTalkTexts.Num() == 1 && PrioritySSD.Num() == 1)
	{
		FoundLastCommitedText = true;
		SSDataOut = PrioritySSD[0];
		return PriorityTalkTexts[0];
	}

	//--------------------More than 1 talk text. Need to filter and get the one with higher activation order.
	//If is in NewQuest, this quest is not in inventory and need to get the first one and that is it.
	if (PriorityTalkTexts[0].Type == ETalkTextType::AcceptQuest)
	{
		FoundLastCommitedText = true;
		SSDataOut = PrioritySSD[0];
		return PriorityTalkTexts[0];
	}

	//Order variables
	int SelectedTalkTextIndex = 0;
	int HighestCompletionOrder = 0;

	//------------------------------------------------------------------------
	//-----------------------------Current Dialog------------------------
	//------------------------------------------------------------------------
	//Is this NPC dialog situation?
	if (PriorityTalkTexts[0].Mode == ETalkTextMode::Dialog)
	{
		if (!DialogsTrack.IsValidIndex(PriorityTalkTexts[0].QuestSlot))
		{
			//Log
			UE_LOG(QuestEditorLog, Warning, TEXT("UQuestManager::GetNPCLastCommitedTalkTextForSSD - Failed to find Quest for talktext. The quest is not active."));
			FoundLastCommitedText = false;
			SSDataOut = FSingleSentenceModeData();
			return FTalkTextInfo();
		}

		FQuestTrack QuestTrackLocal = DialogsTrack[PriorityTalkTexts[0].QuestSlot];
		SelectedTalkTextIndex = 0;
		HighestCompletionOrder = 0;

		for (int i = 0; i < PriorityTalkTexts.Num(); i++)
		{
			//This is the highest found?
			if (QuestTrackLocal.TalkTasksTracks[PriorityTalkTexts[i].TaskSlot].CompletionOrder >= HighestCompletionOrder)
			{
				//Update
				HighestCompletionOrder = QuestTrackLocal.TalkTasksTracks[PriorityTalkTexts[i].TaskSlot].CompletionOrder;
				SelectedTalkTextIndex = i;
			}
		}

		if (PrioritySSD.IsValidIndex(SelectedTalkTextIndex) && PriorityTalkTexts.IsValidIndex(SelectedTalkTextIndex))
		{
			FoundLastCommitedText = true;
			SSDataOut = PrioritySSD[SelectedTalkTextIndex];
			return PriorityTalkTexts[SelectedTalkTextIndex];
		}
	}

	//------------------------------------------------------------------------
	//-----------------------------Current Quest------------------------
	//------------------------------------------------------------------------	

	if (!QuestsTrack.IsValidIndex(PriorityTalkTexts[0].QuestSlot))
	{
		//Log
		UE_LOG(QuestEditorLog, Warning, TEXT("UQuestManager::GetNPCLastCommitedTalkTextForSSD - Failed to find Quest for talktext. The quest is not active."));
		FoundLastCommitedText = false;
		SSDataOut = FSingleSentenceModeData();
		return FTalkTextInfo();
	}

	FQuestTrack QuestTrackLocal = QuestsTrack[PriorityTalkTexts[0].QuestSlot];
	SelectedTalkTextIndex = 0;
	HighestCompletionOrder = 0;

	for (int i = 0; i < PriorityTalkTexts.Num(); i++)
	{
		//This is the highest found?
		if (QuestTrackLocal.TalkTasksTracks[PriorityTalkTexts[i].TaskSlot].CompletionOrder >= HighestCompletionOrder)
		{
			//Update
			HighestCompletionOrder = QuestTrackLocal.TalkTasksTracks[PriorityTalkTexts[i].TaskSlot].CompletionOrder;
			SelectedTalkTextIndex = i;
		}
	}

	if (PrioritySSD.IsValidIndex(SelectedTalkTextIndex) && PriorityTalkTexts.IsValidIndex(SelectedTalkTextIndex))
	{
		FoundLastCommitedText = true;
		SSDataOut = PrioritySSD[SelectedTalkTextIndex];
		return PriorityTalkTexts[SelectedTalkTextIndex];
	}

	FoundLastCommitedText = false;
	SSDataOut = FSingleSentenceModeData();
	return FTalkTextInfo();
}

void UQuestManager::LoadQuestsTrack(bool IsDialogIN, TArray<FQuestRep>& QuestInventoryIN, TArray<FQuestTrack>& QuestTrackIN, TArray<FQuestObjectTrack>& ObjectsTrackIN)
{
	//Resize y set up de objects track local. Needed before handle spawns of active tasks.
	GenerateEmptyObjectsTrack(QuestInventoryIN, ObjectsTrackIN);

	FQuest QuestLocal = FQuest();

	//For all quests
	for (int i = 0; i < QuestTrackIN.Num(); i++)
	{	
		//Get Quest from inventory
		QuestLocal = UQuestsFunctionLibrary::QuestRepToQuest(QuestInventoryIN[i]);

		//For active tasks of quest
		for (int j = 0; j < QuestTrackIN[i].ActiveTasks.Num(); j++)
		{
			//If needs spawns, reespawn the class
			if (QuestTrackIN[i].ActiveTasks[j].TaskType == EQuestTaskType::Task) HandleBaseTaskSpawn(i, QuestLocal, QuestTrackIN[i].ActiveTasks[j].TaskID, IsDialogIN, ObjectsTrackIN, true);
		}		
	}		
	return;
}

void UQuestManager::AddQuestTrack(const FQuest & QuestIN, TArray<FQuestTrack>& QuestTrackIN, TArray<FQuestObjectTrack>& ObjectsTrackIN)
{
	//-------------------------------------------------Tasks
	//Resize task track with all tasks	
	TArray <FTaskTrack> TasksTrack;
	TasksTrack.Empty();
	TasksTrack.SetNum(QuestIN.Tasks.Num());

	for (int i = 0; i < QuestIN.Tasks.Num(); i++)
	{
		TasksTrack[i].State = EQuestTaskState::Locked;
		TasksTrack[i].CurrentAmount = 0;
		//Initial payload set
		TasksTrack[i].CurrentPayload = QuestIN.Tasks[i].Payload;
	}

	//-------------------------------------------------Talk Tasks
	//Resize task track with all talk tasks

	TArray <FTalkTaskTrack> TalkTasksTrack;
	TalkTasksTrack.Empty();
	TalkTasksTrack.SetNum(QuestIN.TalkTasks.Num());	

	for (int i = 0; i < TalkTasksTrack.Num(); i++)
	{
		TalkTasksTrack[i].State = EQuestTaskState::Locked;
	}

	//----------------------------------Track	
	//track for a single quest
	FQuestTrack TrackLocal;
	//Update track
	TrackLocal.PreActiveTasks.Empty();
	TrackLocal.ActiveTasks.Empty();
	TrackLocal.TasksTracks = TasksTrack;
	TrackLocal.TalkTasksTracks = TalkTasksTrack;
	
	//Add track
	QuestTrackIN.Add(TrackLocal);
	
	//Generate object track
	GenerateEmptyObjectsTrackForQuest(QuestIN, ObjectsTrackIN);
}

bool UQuestManager::AddTaskIDToActiveTrack (int QuestSlotIn, const FQuestTaskID& TaskIDIn, TArray<FQuestTrack>& QuestTrackIN)
{
	//Ensure its unique id when adding.
	QuestTrackIN[QuestSlotIn].ActiveTasks.AddUnique(TaskIDIn);
	return true;
}

bool UQuestManager::RemoveTaskIDFromActiveTrack(int QuestSlotIn, const FQuestTaskID& TaskIDIn, TArray<FQuestTrack>& QuestTrackIN, TArray<FQuestObjectTrack>& ObjectsTrackIN)
{
	if (!QuestTrackIN.IsValidIndex(QuestSlotIn))
	{
		//Log
		UE_LOG(QuestEditorLog, Warning, TEXT("UQuestManager::RemoveTaskIDFromActiveTrack - Quest %i is a invalid quest track slot and the remove operation failed."), QuestSlotIn);
		return false;
	}
		
	//Before removing, destroys associated task if needed
	if (TaskIDIn.TaskType == EQuestTaskType::Task) HandleBaseTaskDestroy(QuestSlotIn, TaskIDIn.TaskID, ObjectsTrackIN);

	//Remove all tasks for this node (should only be once but...)	
	//Perform remove in a local track
	TArray <FQuestTaskID> ActiveTasksLocal = QuestTrackIN[QuestSlotIn].ActiveTasks;
	ActiveTasksLocal.Remove(TaskIDIn);
	
	//Apply to track
	QuestTrackIN[QuestSlotIn].ActiveTasks = ActiveTasksLocal;
	
	return true;
}

bool UQuestManager::AddTaskIDToPreActiveTrack(int QuestSlotIn, const FQuestTaskID& TaskIDIn, TArray<FQuestTrack>& QuestTrackIN)
{
	//Ensure its unique id when adding.
	QuestTrackIN[QuestSlotIn].PreActiveTasks.AddUnique(TaskIDIn);
	return true;
}

bool UQuestManager::RemoveTaskIDFromPreActiveTrack(int QuestSlotIn, const FQuestTaskID& TaskIDIn, TArray<FQuestTrack>& QuestTrackIN)
{
	if (!QuestTrackIN.IsValidIndex(QuestSlotIn))
	{
		//Log
		UE_LOG(QuestEditorLog, Warning, TEXT("UQuestManager::RemoveTaskIDFromPreActiveTrack - Quest %i is a invalid quest track slot and the remove operation failed."), QuestSlotIn);
		return false;
	}

	//Perform remove in a local track
	TArray <FQuestTaskID> PreActiveTasksLocal = QuestTrackIN[QuestSlotIn].PreActiveTasks;
	PreActiveTasksLocal.Remove(TaskIDIn);
	
	//Apply to track
	QuestTrackIN[QuestSlotIn].PreActiveTasks = PreActiveTasksLocal;

	return true;
}

bool UQuestManager::ActivateTalkTaskFromPreActiveTrack(int QuestSlotIn, bool IsDialogTaskIN, const FQuest& QuestIn, int TalkTaskSlotIn, TArray<FQuestRep>& QuestsInventoryIN, TArray<FQuestTrack>& QuestTrackIN, TArray<FQuestObjectTrack>& ObjectsTrackIN, TArray<FCompletedQuestTrack>& CompletedQuestsTrackIN, TArray<FQuestFailTrack>& QuestsFailTrackIN)
{
	//Make a node
	FQuestTaskID NodeLocal = FQuestTaskID();
	NodeLocal.TaskType = EQuestTaskType::TalkTask;
	NodeLocal.TaskID = TalkTaskSlotIn;
	
	//Valid track?
	if (!QuestTrackIN.IsValidIndex(QuestSlotIn)) return false;

	//Is in preactive array? If not then cannot do anything
	if (!QuestTrackIN[QuestSlotIn].PreActiveTasks.Contains(NodeLocal))
	{
		//Log
		UE_LOG(QuestEditorLog, Error, TEXT("UQuestManager::ActivateTalkTaskFromPreActiveTrack - Node %i from quest %i is not preactivated. Activation cancelled."), NodeLocal.TaskID, QuestSlotIn);
		return false;
	}
	
	//Remove from Pre active
	RemoveTaskIDFromPreActiveTrack(QuestSlotIn, NodeLocal, QuestTrackIN);

	//Set state temporary to locked just to allow activation
	SetNodeStateInTrack(QuestSlotIn, NodeLocal, EQuestTaskState::Locked, QuestTrackIN);

	//Can activate?
	if (NodeCanActivate(QuestSlotIn, QuestIn, NodeLocal, QuestTrackIN))
	{
		//This needs to store active tasks track for conversation check. 
		//This function and complete nodes are the only checks. If this is not here, the initial tasks will not trigger on accepting a quest
		TArray <FQuestTaskID> TrackBeforeActivate = QuestTrackIN[QuestSlotIn].ActiveTasks;

		//Activation
		ActivateNodeID(QuestSlotIn, IsDialogTaskIN, QuestIn, NodeLocal, QuestsInventoryIN, QuestTrackIN, ObjectsTrackIN, CompletedQuestsTrackIN, QuestsFailTrackIN);

		//Check conversation starters. This allows to avoid the check on activate everytime, this is the only activate that occurs alone(or less times, only on quest starts). 
		//All others are connected to a completed node.
		CheckConversationStartAndEnd(QuestSlotIn, QuestIn, IsDialogTaskIN, TrackBeforeActivate, QuestTrackIN);
	}
	//If cannot activate it fails. Should neve happen.
	else
	{
		//Log
		UE_LOG(QuestEditorLog, Error, TEXT("UQuestManager::ActivateTalkTaskFromPreActiveTrack - Node %i from quest %i couldnt be activated from PreActive State. Node is now Locked."), NodeLocal.TaskID, QuestSlotIn);
		return false;
	}
	
	return true;
}

bool UQuestManager::PreActivateTask(int QuestSlotIn, const FQuestTaskID& TaskIDIn, bool IsDialogIn, TArray<FQuestTrack>& QuestTrackIN)
{
	//Change node state
	SetNodeStateInTrack(QuestSlotIn, TaskIDIn, EQuestTaskState::PreActive, QuestTrackIN);
	OnNodeChangedState.Broadcast(QuestSlotIn, TaskIDIn, IsDialogIn, EQuestTaskState::PreActive);

	//Add node ID to pre active track.
	AddTaskIDToPreActiveTrack(QuestSlotIn, TaskIDIn, QuestTrackIN);

	return true;
}

bool UQuestManager::PreActivateAllInitialTalkTasks(int QuestSlotIn, const FQuest& QuestIn, bool IsDialogIn, TArray<FQuestTrack>& QuestTrackIN)
{
	//Search all initial talk tasks
	//Activate tasks for ANY npc, so bool is true
	TArray <int> InitialTalkTasks = GetStartingTasksSlots(EQuestTaskType::TalkTask, QuestIn, nullptr, true);

	FQuestTaskID TaskIDLocal = FQuestTaskID();
	TaskIDLocal.TaskType = EQuestTaskType::TalkTask;

	TArray <FQuestTaskID> InitialTasksIDS = TArray <FQuestTaskID>();

	//Convert initials talk tasks to IDS
	for (int i = 0; i < InitialTalkTasks.Num(); i++)
	{
		//convert to ID
		TaskIDLocal.TaskID = InitialTalkTasks[i];
		//Preactivate
		PreActivateTask(QuestSlotIn, TaskIDLocal, IsDialogIn, QuestTrackIN);
	}
	   	 
	return true;
}

TArray<int> UQuestManager::GetPreActiveTasksSlotsFromTrack(int QuestSlotIn, EQuestTaskType TaskTypeIn, const TArray<FQuestTrack>& QuestTrackIN) const
{
	TArray <int> SlotsTasks = TArray <int>();

	for (int i = 0; i < QuestTrackIN[QuestSlotIn].PreActiveTasks.Num(); i++)
	{
		if (QuestTrackIN[QuestSlotIn].PreActiveTasks[i].TaskType == TaskTypeIn)
		{
			//add id to var out
			SlotsTasks.Add(QuestTrackIN[QuestSlotIn].PreActiveTasks[i].TaskID);
		}
	}

	return SlotsTasks;
}

TArray<int> UQuestManager::GetPreActiveTalkTasksSlotsFromTrack(int QuestSlotIn, const FQuest& QuestIn, const FNPCQuestDefRep& NPCDefIn, const TArray<FQuestTrack>& QuestTrackIN) const
{
	if (!QuestTrackIN.IsValidIndex(QuestSlotIn)) return TArray<int>();

	TArray<int> PreActiveTalksForNPC = TArray<int>();
	FQuestTalkTask TalkTaskLocal = FQuestTalkTask();

	//all talk tasks pre active	
	for (int i = 0; i < QuestTrackIN[QuestSlotIn].PreActiveTasks.Num(); i++)
	{
		//Is talk task?
		if (QuestTrackIN[QuestSlotIn].PreActiveTasks[i].TaskType == EQuestTaskType::TalkTask)
		{
			//Get task
			TalkTaskLocal = QuestIn.TalkTasks[QuestTrackIN[QuestSlotIn].PreActiveTasks[i].TaskID];

			//Same NPC
			if (TalkTaskLocal.TalkNPC == NPCDefIn)
			{
				PreActiveTalksForNPC.AddUnique(QuestTrackIN[QuestSlotIn].PreActiveTasks[i].TaskID);
			}
		}	
	}

	return PreActiveTalksForNPC;
}

TArray<FQuest> UQuestManager::ConvertQuestsInventoryRep(const TArray<FQuestRep>& QuestInventoryIN) const
{
	//Returns a quest array.
	TArray <FQuest> QuestsLocal = TArray <FQuest>();
	QuestsLocal.Reserve(QuestInventoryIN.Num());

	//Adds all quests
	for (int i = 0; i < QuestInventoryIN.Num(); i++)
	{
		QuestsLocal.Add(UQuestsFunctionLibrary::QuestRepToQuest(QuestInventoryIN[i]));
	}

	return QuestsLocal;
}

bool UQuestManager::NodeStartHandlingOperation(int QuestSlotIn, const FQuestTaskID& TaskIDIn, TArray<FQuestTrack>& QuestTrackIN)
{
	//If invalid return false
	if (!QuestTrackIN.IsValidIndex(QuestSlotIn)) return false;

	//If node is not invalid means that another node is handling it. Do not do anything
	if (QuestTrackIN[QuestSlotIn].NodeHandler.TaskID != -1) return false;

	//Node start handling main complete and active operation
	QuestTrackIN[QuestSlotIn].NodeHandler = TaskIDIn;

	return true;
}

bool UQuestManager::NodeStopHandlingOperation(int QuestSlotIn, const FQuestTaskID& TaskIDIn, TArray<FQuestTrack>& QuestTrackIN)
{
	//If invalid return false
	if (!QuestTrackIN.IsValidIndex(QuestSlotIn)) return false;

	//If node is invalid means that this cannot be turned off leave
	if (QuestTrackIN[QuestSlotIn].NodeHandler.TaskID == -1) return false;

	//Any node that is not the handling one cannot deactivate this
	if (QuestTrackIN[QuestSlotIn].NodeHandler != TaskIDIn) return false;

	//Node end handling main complete and active operation. Sets ID to invalid, dont care type
	QuestTrackIN[QuestSlotIn].NodeHandler.TaskID = -1;

	return true;
}

bool UQuestManager::IsAnyNodeHandlingOperation(int QuestSlotIn, TArray<FQuestTrack>& QuestTrackIN)
{
	//If invalid return false
	if (!QuestTrackIN.IsValidIndex(QuestSlotIn)) return false;

	//If ID is -1 means no node is handling a operation
	if (QuestTrackIN[QuestSlotIn].NodeHandler.TaskID == -1) return false;
	
	return true;
}

bool UQuestManager::IsNodeTheOperationHandler(int QuestSlotIn, const FQuestTaskID& TaskIDIn, TArray<FQuestTrack>& QuestTrackIN)
{
	//If invalid return false
	if (!QuestTrackIN.IsValidIndex(QuestSlotIn)) return false;

	//If ID is the same as the input, then this node is handling the operation
	return (QuestTrackIN[QuestSlotIn].NodeHandler == TaskIDIn);
}

TArray<int> UQuestManager::GetActiveTasksSlotsFromTrackWithArrays(int QuestSlotIn, EQuestTaskType TaskTypeIn, const TArray<FQuestTrack>& QuestTrackIN) const
{
	TArray <int> SlotsTasks = TArray <int>();

	if (!QuestTrackIN.IsValidIndex(QuestSlotIn)) return SlotsTasks;

	for (int i = 0; i < QuestTrackIN[QuestSlotIn].ActiveTasks.Num(); i++)
	{
		if (QuestTrackIN[QuestSlotIn].ActiveTasks[i].TaskType == TaskTypeIn)
		{
			//add id to var out
			SlotsTasks.Add(QuestTrackIN[QuestSlotIn].ActiveTasks[i].TaskID);
		}
	}
	return SlotsTasks;
}

TArray<int> UQuestManager::GetActiveTasksSlotsFromTrack(int QuestSlotIn, EQuestTaskType TaskTypeIn) const
{	
	return  GetActiveTasksSlotsFromTrackWithArrays(QuestSlotIn, TaskTypeIn, QuestsTrack); //This is only for quests
}

void UQuestManager::HandleQuestFailTrackOnAccept(const FQuestRep& QuestRepIn, bool IsDialogIn, TArray<FQuestFailTrack>& QuestsFailTrackIN)
{
	//Handle fail track for accept
	//Is a quest and tracks quest fails or is a dialog and track fail dialogs
	if ((TrackFailQuests && !IsDialogIn)  || (TrackFailDialogs && IsDialogIn))
	{
		//This assumes that the quest is not failed anymore.
		//Remove from fail track
		RemoveQuestFromFailTrackWithArrays(QuestRepIn, QuestsFailTrackIN);
	}

	return;
}

bool UQuestManager::AddQuestToFailtrack(const FQuestRep& QuestRepIn, TArray<FQuestFailTrack>& QuestsFailTrackIN)
{
	int Position = GetQuestFailtrackIndex(QuestRepIn, QuestsFailTrackIN);

	//valid cannot add
	if (!QuestsFailTrackIN.IsValidIndex(Position))
	{
		//generate track
		FQuestFailTrack LocalFailTrack = FQuestFailTrack();
		LocalFailTrack.Quest = QuestRepIn;
		LocalFailTrack.FailDate = FDateTime::UtcNow();

		//add
		QuestsFailTrackIN.Add(LocalFailTrack);
		return true;
	}
	//if already is failed cannot add	
	return false;
}

bool UQuestManager::RemoveQuestFromFailTrack(bool IsDialogIN, const FQuestRep& QuestRepIn)
{
	//This is called for the tasks when ending.
	bool trackresult = false;

	if (IsDialogIN)
	{
		//Dialogs array configuration
		trackresult = RemoveQuestFromFailTrackWithArrays(QuestRepIn, DialogsFailTrack);
	}
	else
	{
		//Quests array configuration
		trackresult = RemoveQuestFromFailTrackWithArrays(QuestRepIn, QuestsFailTrack);
	}

	return trackresult;
}

bool UQuestManager::RemoveQuestFromFailTrackWithArrays(const FQuestRep& QuestRepIn, TArray<FQuestFailTrack>& QuestsFailTrackIN)
{
	int Position = GetQuestFailtrackIndex(QuestRepIn, QuestsFailTrackIN);

	//valid can remove
	if (QuestsFailTrackIN.IsValidIndex(Position))
	{
		//remove and end
		QuestsFailTrackIN.RemoveAt(Position);
		return true;
	}

	return false;
}

int UQuestManager::GetQuestFailtrackIndex(const FQuestRep& QuestRepIn, const TArray<FQuestFailTrack>& QuestsFailTrackIN) const
{
	int Position = -1;

	for (int i = 0; i < QuestsFailTrackIN.Num(); i++)
	{
		//Is this quest?
		if (QuestsFailTrackIN[i].Quest == QuestRepIn)
		{
			//update and break
			Position = i;
			break;
		}
	}
	return Position;
}

bool UQuestManager::IsQuestFailed(bool IsDialogIN, const FQuestRep& QuestRepIn) const
{
	bool trackresult = false;

	if (IsDialogIN)
	{
		//Dialogs array configuration
		trackresult = IsQuestFailedWithArrays(QuestRepIn, IsDialogIN, DialogsFailTrack);
	}
	else
	{
		//Quests array configuration
		trackresult = IsQuestFailedWithArrays(QuestRepIn, IsDialogIN, QuestsFailTrack);
	}

	return trackresult;
}

bool UQuestManager::IsQuestFailedWithArrays(const FQuestRep& QuestRepIn, bool IsDialogIn, const TArray<FQuestFailTrack>& QuestsFailTrackIN) const
{
	bool Isfailed = false;
	float MaxFailTime = 0;

	for (int i = 0; i < QuestsFailTrackIN.Num(); i++)
	{
		//Is this quest?
		if (QuestsFailTrackIN[i].Quest == QuestRepIn)
		{
			//Determine fail time
			if (IsDialogIn)
			{
				MaxFailTime = MaxDialogFailTime;
			}
			else
			{
				MaxFailTime = MaxQuestFailTime;
			}

			//If the time to track is infinite the quest is failed then.
			if (MaxFailTime == 0)
			{
				//is forced to fail forever on 0
				Isfailed = true;
				break;
			}

			//Even if the quest is in the track, it does not mean it is failed if the data expired
			FTimespan TimeSpanFail = TimeSpanFail.FromHours(MaxFailTime);
			FTimespan TimeSpanElapsed = FDateTime::UtcNow() - QuestsFailTrackIN[i].FailDate;
			//Time elapsed of fail
			if (TimeSpanElapsed >= TimeSpanFail)
			{
				//if the time elapsed is higher than the max, this quest is still in track but should be removed upon accept
				Isfailed = false;
				break;
			}
			else
			{
				//if found but the time is not met it means that the quest is failed
				//update and break. This quest is in track and rightfully so.
				Isfailed = true;
				break;
			}
		}
	}

	return Isfailed;
}

bool UQuestManager::HasQuestFailTimeSpan(bool IsDialogIN, const FQuestRep& QuestRepIn, const FTimespan& QuestFailTimeIn) const
{
	bool trackresult = false;

	if (IsDialogIN)
	{
		//Dialogs array configuration
		trackresult = HasQuestFailTimeSpanWithArrays(QuestRepIn, QuestFailTimeIn, DialogsFailTrack);
	}
	else
	{
		//Quests array configuration
		trackresult = HasQuestFailTimeSpanWithArrays(QuestRepIn, QuestFailTimeIn, QuestsFailTrack);
	}

	return trackresult;
}

bool UQuestManager::HasQuestFailTimeSpanWithArrays(const FQuestRep& QuestRepIn, const FTimespan& QuestFailTimeIn, const TArray<FQuestFailTrack>& QuestsFailTrackIN) const
{
	int Position = GetQuestFailtrackIndex(QuestRepIn, QuestsFailTrackIN);

	//valid ?
	if (QuestsFailTrackIN.IsValidIndex(Position))
	{
		//check if meets time span provided
		//Time elapsed is higher or equal than the prvided
		return ((QuestsFailTrackIN[Position].FailDate - FDateTime::UtcNow()) >= QuestFailTimeIn);
	}

	return false;
}

bool UQuestManager::UpdateAmmountTaskTrack(int QuestSlotIn, bool IsDialogTaskIN, const FQuest& QuestIn, int TaskIN, int AmountIN, TArray<FQuestRep>& QuestsInventoryIN, TArray<FQuestTrack>& QuestsTrackIN, TArray<FQuestObjectTrack>& ObjectsTrackIN, TArray<FCompletedQuestTrack>& CompletedQuestsTrackIN, TArray<FQuestFailTrack>& QuestsFailTrackIN)
{
	//Vars
	FQuestTaskID TaskID;
	TaskID.TaskID = TaskIN;
	TaskID.TaskType = EQuestTaskType::Task;

	//If not active then leave (mb is already completed and dont want to retrigger completion)
	if (GetNodeStateInTrackWithArrays(QuestSlotIn, TaskID, QuestsTrackIN) != EQuestTaskState::Active) return false;

	//Update track amount
	int NewAmountLocal = QuestsTrackIN[QuestSlotIn].TasksTracks[TaskIN].CurrentAmount + AmountIN;
	
	//clamp
	int MaxClamp = QuestIn.Tasks[TaskIN].Payload.TimesToCheckTask;

	//If amount to check is less or 0, then force it to 1.
	if (MaxClamp <= 0) MaxClamp = 1;

	NewAmountLocal = FMath::Clamp(NewAmountLocal, 0, MaxClamp);
	
	bool result = false;	
	
	//Update task in questtrack
	QuestsTrackIN[QuestSlotIn].TasksTracks[TaskIN].CurrentAmount = NewAmountLocal;

	Client_QTaskUpdatedTrackAmount.Broadcast(QuestSlotIn, TaskID, IsDialogTaskIN);

	//completed?
	if (NewAmountLocal >= QuestIn.Tasks[TaskIN].Payload.TimesToCheckTask)
	{
		//CompleteLogic
		result = true;
		CompleteNode(QuestSlotIn, IsDialogTaskIN, QuestIn, TaskID, QuestsInventoryIN, QuestsTrackIN, ObjectsTrackIN, CompletedQuestsTrackIN, QuestsFailTrackIN);
	}	

	return result;
}

bool UQuestManager::HandleBaseTaskSpawn(int QuestSlotIN, const FQuest& QuestIn, int TaskSlotIN, bool IsDialogTaskIN, TArray<FQuestObjectTrack>& ObjectsTrackIN, bool IsLoading)
{
	//If object already exist dont do anything
	if (!ObjectsTrackIN.IsValidIndex(QuestSlotIN)) return false;
	if (!ObjectsTrackIN[QuestSlotIN].ObjectTasksTrack.IsValidIndex(TaskSlotIN))return false;

	//If a task is respawned for this, it will be not null, but is pending kill?
	if (ObjectsTrackIN[QuestSlotIN].ObjectTasksTrack[TaskSlotIN].BaseTaskReference != nullptr)
	{
		if (!IsValid(ObjectsTrackIN[QuestSlotIN].ObjectTasksTrack[TaskSlotIN].BaseTaskReference))
		{
			ObjectsTrackIN[QuestSlotIN].ObjectTasksTrack[TaskSlotIN].BaseTaskReference = nullptr;
			//Log
			UE_LOG(QuestEditorLog, Log, TEXT("UQuestManager::HandleBaseTaskSpawn - Task %i of quest %i is pending kill and respawned correctly."), TaskSlotIN, QuestSlotIN);
		}
		else
		{
			//Log
			UE_LOG(QuestEditorLog, Warning, TEXT("UQuestManager::HandleBaseTaskSpawn - Task %i of quest %i is beign respawned with a already valid reference in track. Operation cancelled"), TaskSlotIN, QuestSlotIN);
			return false;
		}
	}		

	//Obtain payload	
	FQuestTaskPayload PayloadLocal = QuestIn.Tasks[TaskSlotIN].Payload;
	
	//Obtain class to spawn
	TSoftClassPtr<UBaseQuestTask> TaskClassLocal = QuestIn.Tasks[TaskSlotIN].TaskClass;

	//Class null returns false
	if (TaskClassLocal == nullptr) return false;

	//Load class from soft ref
	TSubclassOf <UBaseQuestTask> LoadedClass;
	LoadedClass = TaskClassLocal.LoadSynchronous();

	//Spawn base task
	UBaseQuestTask* MyBaseQuestTask = NewObject<UBaseQuestTask>(GetOwner(), LoadedClass.Get());

	//if invalid return
	if (!MyBaseQuestTask) return false;

	//Update track reference
	ObjectsTrackIN[QuestSlotIN].ObjectTasksTrack[TaskSlotIN].BaseTaskReference = MyBaseQuestTask;

	//Start base task functions
	if (IsLoading) MyBaseQuestTask->LoadQuestTask(QuestSlotIN, TaskSlotIN, IsDialogTaskIN, GetOwner(), this, PayloadLocal);
	else MyBaseQuestTask->StartQuestTask(QuestSlotIN, TaskSlotIN, IsDialogTaskIN, GetOwner(), this, PayloadLocal);

	return true;
}

bool UQuestManager::HandleBaseTaskSave(int QuestSlotIN, int TaskSlotIN, TArray<FQuestObjectTrack>& ObjectsTrackIN)
{
	//Task invalid slots leave
	if (!ObjectsTrackIN.IsValidIndex(QuestSlotIN)) return false;
	if (!ObjectsTrackIN[QuestSlotIN].ObjectTasksTrack.IsValidIndex(TaskSlotIN)) return false;

	//Not valid leave
	if (ObjectsTrackIN[QuestSlotIN].ObjectTasksTrack[TaskSlotIN].BaseTaskReference == nullptr) return false;

	//if pending dont do anything
	if (IsValid(ObjectsTrackIN[QuestSlotIN].ObjectTasksTrack[TaskSlotIN].BaseTaskReference))
	{
		//destroy
		ObjectsTrackIN[QuestSlotIN].ObjectTasksTrack[TaskSlotIN].BaseTaskReference->SaveQuestTask();
	}

	return true;
}

bool UQuestManager::HandleAllTasksSave(TArray<FQuestObjectTrack>& ObjectsTrackIN)
{
	//For all quests
	for (int i = 0; i < ObjectsTrackIN.Num(); i++)
	{
		//Could only destroy actives, but this ensures that there is never a loose object flying around :D 
		for (int j = 0; j < ObjectsTrackIN[i].ObjectTasksTrack.Num(); j++)
		{
			HandleBaseTaskSave(i, j, ObjectsTrackIN);
		}
	}
	return true;
}

bool UQuestManager::HandleBaseTaskDestroy(int QuestSlotIN, int TaskSlotIN, TArray<FQuestObjectTrack>& ObjectsTrackIN)
{
	//Task invalid slots leave
	if (!ObjectsTrackIN.IsValidIndex(QuestSlotIN)) return false;
	if (!ObjectsTrackIN[QuestSlotIN].ObjectTasksTrack.IsValidIndex(TaskSlotIN)) return false;

	//Not valid leave
	if (ObjectsTrackIN[QuestSlotIN].ObjectTasksTrack[TaskSlotIN].BaseTaskReference == nullptr) return false;
		
	//if pending dont do anything
	if (!IsValid(ObjectsTrackIN[QuestSlotIN].ObjectTasksTrack[TaskSlotIN].BaseTaskReference))
	{
		//destroy
		ObjectsTrackIN[QuestSlotIN].ObjectTasksTrack[TaskSlotIN].BaseTaskReference->DestroyQuestTask();
		ObjectsTrackIN[QuestSlotIN].ObjectTasksTrack[TaskSlotIN].BaseTaskReference->MarkAsGarbage();
	}
	
	return true;
}

bool UQuestManager::DestroyAllRemainingBaseTasksObjectsForQuest(int QuestSlotIN, TArray<FQuestObjectTrack>& ObjectsTrackIN)
{
	if (!ObjectsTrackIN.IsValidIndex(QuestSlotIN)) return false;

	//Could only destroy actives, but this ensures that there is never a loose object flying around :D 
	for (int i = 0; i < ObjectsTrackIN[QuestSlotIN].ObjectTasksTrack.Num(); i++)
	{
		HandleBaseTaskDestroy(QuestSlotIN, i, ObjectsTrackIN);
	}

	return true;
}

bool UQuestManager::DestroyAllRemainingBaseTasksObjects(TArray<FQuestObjectTrack>& ObjectsTrackIN)
{
	for (int i = 0; i < ObjectsTrackIN.Num(); i++)
	{
		DestroyAllRemainingBaseTasksObjectsForQuest(i, ObjectsTrackIN);
	}
	return true;
}

bool UQuestManager::GenerateEmptyObjectsTrack(TArray<FQuestRep>& QuestInventoryIN, TArray<FQuestObjectTrack>& ObjectsTrackIN)
{
	TArray <FQuest> QuestInvLocal = ConvertQuestsInventoryRep(QuestInventoryIN);

	for (int i = 0; i < QuestInvLocal.Num(); i++)
	{
		GenerateEmptyObjectsTrackForQuest(QuestInvLocal[i], ObjectsTrackIN);
	}
	return true;
}

bool UQuestManager::GenerateEmptyObjectsTrackForQuest(const FQuest& QuestIN, TArray<FQuestObjectTrack>& ObjectsTrackIN)
{
	//--------------------------------------------------Tasks Objects local track
	TArray <FTaskObjectTrack> QuestObjectTrack;
	QuestObjectTrack.Empty();
	QuestObjectTrack.SetNum(QuestIN.Tasks.Num());

	for (int i = 0; i < QuestIN.Tasks.Num(); i++)
	{
		QuestObjectTrack[i].BaseTaskReference = nullptr;
	}
	
	//Objects Track for a single quest
	FQuestObjectTrack QuestObjectTrackInterno;
	QuestObjectTrackInterno.ObjectTasksTrack = QuestObjectTrack;

	//add
	ObjectsTrackIN.Add(QuestObjectTrackInterno);
	
	return true;
}

bool UQuestManager::ReduceQuestSlotForTaskObjectsAfterRemove(int QuestSlotIn, int TaskSlotIn, int RemovedQuestSlotIn, TArray<FQuestObjectTrack>& ObjectsTrackIN)
{
	//Quest slot of task needs to be higher than the removed one to be lowered in value. Lower and equal quests are unaffected
	if (QuestSlotIn <= RemovedQuestSlotIn) return false;
	
	//Not valid object leave
	if (ObjectsTrackIN[QuestSlotIn].ObjectTasksTrack[TaskSlotIn].BaseTaskReference == nullptr) return false;

	//if pending dont do anything
	if (IsValid(ObjectsTrackIN[QuestSlotIn].ObjectTasksTrack[TaskSlotIn].BaseTaskReference))
	{
		//Quest Slot is bigger than the one removed? Then needs to lower the Slot in 1.
		ObjectsTrackIN[QuestSlotIn].ObjectTasksTrack[TaskSlotIn].BaseTaskReference->ReduceQuestSlot();
	}

	return true;
}

bool UQuestManager::ReduceAllQuestSlotsObjectsAfterRemove(int RemovedQuestSlotIn, TArray<FQuestObjectTrack>& ObjectsTrackIN)
{
	for (int i = 0; i < ObjectsTrackIN.Num(); i++)
	{
		//For all tasks of the quest.
		for (int j = 0; j < ObjectsTrackIN[i].ObjectTasksTrack.Num(); j++)
		{
			//Reduce all slots in 1. (checks are performed inside function)
			ReduceQuestSlotForTaskObjectsAfterRemove(i, j, RemovedQuestSlotIn, ObjectsTrackIN);
		}		
	}

	return true;
}

bool UQuestManager::SetTaskCurrentPayload(int QuestSlotIN, int TaskSlotIn, bool IsDialogTaskIn, const FQuestTaskPayload& NewPayloadIn)
{
	//Valid
	if (IsDialogTaskIn)
	{
		//Valid check
		if (!DialogsTrack.IsValidIndex(QuestSlotIN)) return false;
		if (!DialogsTrack[QuestSlotIN].TasksTracks.IsValidIndex(TaskSlotIn)) return false;

		//Update payload
		DialogsTrack[QuestSlotIN].TasksTracks[TaskSlotIn].CurrentPayload = NewPayloadIn;
		return true;
	}
	else
	{
		//Valid check
		if (!QuestsTrack.IsValidIndex(QuestSlotIN)) return false;
		if (!QuestsTrack[QuestSlotIN].TasksTracks.IsValidIndex(TaskSlotIn)) return false;

		//Update payload
		QuestsTrack[QuestSlotIN].TasksTracks[TaskSlotIn].CurrentPayload = NewPayloadIn;
		return true;
	}
}

FQuestTaskPayload UQuestManager::GetTaskCurrentPayload(int QuestSlotIN, int TaskSlotIn, bool IsDialogTaskIn)
{
	//Valid
	if (IsDialogTaskIn)
	{
		//Valid check
		if (!DialogsTrack.IsValidIndex(QuestSlotIN)) return FQuestTaskPayload();
		if (!DialogsTrack[QuestSlotIN].TasksTracks.IsValidIndex(TaskSlotIn)) return FQuestTaskPayload();

		//payload
		return DialogsTrack[QuestSlotIN].TasksTracks[TaskSlotIn].CurrentPayload;
	}
	else
	{
		//Valid check
		if (!QuestsTrack.IsValidIndex(QuestSlotIN)) return FQuestTaskPayload();
		if (!QuestsTrack[QuestSlotIN].TasksTracks.IsValidIndex(TaskSlotIn)) return FQuestTaskPayload();

		//payload
		return QuestsTrack[QuestSlotIN].TasksTracks[TaskSlotIn].CurrentPayload;
	}
}

FQuestTaskPayload UQuestManager::GetTaskInitialPayload(int QuestSlotIN, int TaskSlotIn, bool IsDialogTaskIn)
{
	FQuest QuestLocal = FQuest();

	//Valid
	if (IsDialogTaskIn)
	{
		//Valid check
		if (!DialogsInventory.IsValidIndex(QuestSlotIN)) return FQuestTaskPayload();

		QuestLocal = UQuestsFunctionLibrary::QuestRepToQuest(DialogsInventory[QuestSlotIN]);

		if (!QuestLocal.Tasks.IsValidIndex(TaskSlotIn)) return FQuestTaskPayload();

		//payload
		return QuestLocal.Tasks[TaskSlotIn].Payload;
	}
	else
	{
		//Valid check
		if (!QuestInventory.IsValidIndex(QuestSlotIN)) return FQuestTaskPayload();

		QuestLocal = UQuestsFunctionLibrary::QuestRepToQuest(QuestInventory[QuestSlotIN]);

		if (!QuestLocal.Tasks.IsValidIndex(TaskSlotIn)) return FQuestTaskPayload();

		//payload
		return QuestLocal.Tasks[TaskSlotIn].Payload;
	}
}

TArray<int> UQuestManager::GetActiveTasksOfClass(int QuestIN, bool IsDialogIn, const TSoftClassPtr<UBaseQuestTask>& TaskClassIn)
{	
	TArray<int> ArraySlotsLocal = TArray<int>();
	TArray<int> ActiveTasks = TArray<int>();
		
	if (IsDialogIn)
	{
		if (!DialogsTrack.IsValidIndex(QuestIN)) return TArray<int>();
		ActiveTasks = GetActiveTasksSlotsFromTrackWithArrays(QuestIN, EQuestTaskType::Task, DialogsTrack);
	}
	else
	{
		if (!QuestsTrack.IsValidIndex(QuestIN)) return TArray<int>();
		ActiveTasks = GetActiveTasksSlotsFromTrackWithArrays(QuestIN, EQuestTaskType::Task, QuestsTrack);
	}
	
	FQuest QuestLocal = GetQuestsInSlot(EQuestSelectedType::Active, QuestIN, IsDialogIn);

	for (int i = 0; i < ActiveTasks.Num(); i++)
	{
		if (QuestLocal.Tasks[ActiveTasks[i]].TaskClass == TaskClassIn)
		{
			ArraySlotsLocal.Add(ActiveTasks[i]);
		}
	}
	return ArraySlotsLocal;
}

bool UQuestManager::HandleTaskEnd(int QuestIN, int TaskIN, bool IsDialogTaskIN, bool ResultIN, int AmountIN)
{
	//This is called for the tasks when ending.
	bool trackresult = false;

	if (IsDialogTaskIN)
	{
		//Dialogs array configuration
		trackresult = HandleTaskEndWithArrays(QuestIN, TaskIN, IsDialogTaskIN, ResultIN, AmountIN, DialogsInventory, DialogsTrack, LocalObjectsDialogsTrack, CompletedDialogsTrack, DialogsFailTrack);
	}
	else
	{
		//Quests array configuration
		trackresult = HandleTaskEndWithArrays(QuestIN, TaskIN, IsDialogTaskIN, ResultIN, AmountIN, QuestInventory, QuestsTrack, LocalObjectsQuestsTrack, CompletedQuestsTrack, QuestsFailTrack);
	}

	//Update talk texts. This is a separated execution from the complete from talk and sometimes it will rekiere an update
	if (trackresult) OnNPCTalkTextsChanged.Broadcast();

	return trackresult;
}

bool UQuestManager::HandleTaskEndWithArrays(int QuestIN, int TaskIN, bool IsDialogTaskIN, bool ResultIN, int AmountIN, TArray<FQuestRep>& QuestsInventoryIN, TArray<FQuestTrack>& QuestsTrackIN, TArray<FQuestObjectTrack>& ObjectsTrackIN, TArray<FCompletedQuestTrack>& CompletedQuestsTrackIN, TArray<FQuestFailTrack>& QuestsFailTrackIN)
{
	if (!QuestsInventoryIN.IsValidIndex(QuestIN))
	{
		UE_LOG(QuestEditorLog, Log, TEXT("UQuestManager::HandleTaskEndWithArrays - Quest %i is invalid."), QuestIN);
		return false;
	}		

	//This is called for the tasks when ending.
	bool trackresult = false;

	FQuest QuestLocal = UQuestsFunctionLibrary::QuestRepToQuest(QuestsInventoryIN[QuestIN]);

	//Positive result, updates track.
	if (ResultIN)
	{
		//Log
		UE_LOG(QuestEditorLog, Log, TEXT("UQuestManager::HandleTaskEndWithArrays - Task %i of quest %i ended with true and %i amount value and updates amount in track."), TaskIN, QuestIN, AmountIN);

		//Handle complete node
		trackresult = UpdateAmmountTaskTrack(QuestIN, IsDialogTaskIN, QuestLocal, TaskIN, AmountIN, QuestsInventoryIN, QuestsTrackIN, ObjectsTrackIN, CompletedQuestsTrackIN, QuestsFailTrackIN);
	}
	//If ends with false the task fails
	else
	{
		//Log
		UE_LOG(QuestEditorLog, Log, TEXT("UQuestManager::HandleTaskEndWithArrays - Task %i of quest %i ended with false value and fails."), TaskIN, QuestIN);

		//Fail node
		FQuestTaskID NodeInvolved;
		NodeInvolved.TaskType = EQuestTaskType::Task;
		NodeInvolved.TaskID = TaskIN;
		FailNode(QuestIN, QuestLocal, IsDialogTaskIN, NodeInvolved, QuestsTrackIN, ObjectsTrackIN);

		//check if the call happens while a operation is beign handled
		//Can only act if no operation is beign handled
		if (!IsAnyNodeHandlingOperation(QuestIN, QuestsTrackIN))
		{
			//Handle quest fail if needed
			if (QuestShouldFail(QuestIN, QuestsTrackIN))
			{
				HandleQuestFail(QuestIN, QuestLocal, IsDialogTaskIN, QuestsInventoryIN, QuestsTrackIN, ObjectsTrackIN, QuestsFailTrackIN);

				//Handle dialogs updating if can execute cosmetics
				if (GetNetMode() != NM_DedicatedServer)
				{
					//Force ui update. If a task fail and the talk tab is open requires a update extra
					UE_LOG(QuestEditorLog, Log, TEXT("UQuestManager::HandleTaskEndWithArrays - Force update ui."));
					OnNPCTalkTextsChanged.Broadcast();
				}
			}
			else
			{
				//Log
				UE_LOG(QuestEditorLog, Log, TEXT("UQuestManager::HandleTaskEndWithArrays - Quest %i did not fail, there are still active tasks."), QuestIN);
			}
		}
	}

	//return if the task is completed
	return trackresult;
}

bool UQuestManager::HandleTaskEnd_ResetTrackToNode(int QuestIN, const FQuestTaskID& StartNodeIn, const FQuestTaskID& EndNodeIn, bool IsDialogTaskIN, bool IncludeDisabledNodesIn)
{	
	bool succes = false;
	if (IsDialogTaskIN)
	{
		//Dialogs
		succes = HandleTaskEnd_ResetTrackToNodeWithArrays(QuestIN, IsDialogTaskIN, StartNodeIn, EndNodeIn, IncludeDisabledNodesIn, DialogsInventory, DialogsTrack, LocalObjectsDialogsTrack, CompletedDialogsTrack, DialogsFailTrack);
	}
	else
	{
		//Quests
		succes = HandleTaskEnd_ResetTrackToNodeWithArrays(QuestIN, IsDialogTaskIN, StartNodeIn, EndNodeIn, IncludeDisabledNodesIn, QuestInventory, QuestsTrack, LocalObjectsQuestsTrack, CompletedQuestsTrack, QuestsFailTrack);
	}

	//Update talk texts. This is a separated execution from the complete from talk and sometimes it will rekiere an update
	if (succes)
	{
		/*
		This is a quality of life. It tries to set the previous text to the now active.
		If you do a circular connection that resets inmmediately to the node that is up(superior), it will look like u are not clicking anything, cuz it resets so fast, to the same node that u commited.
		I would add a talk node in between to avoid this behaviour, but the possibility is covered anyways.
		*/
		//Current node is the one reseted to? This means that u pressed a talktext and set it to current, then it becomes active and u no longer see the talk option because is already selected.
		//for this to happen u need to make a circular conection immediate to the node it resets.
		if (ActiveTalkText.QuestSlot == QuestIN && ActiveTalkText.TaskSlot == EndNodeIn.TaskID)
		{
			//Is the current NPC valid. This is done on client only so the server would not follow this path. Thats why i can get the NPC like this.
			if (GetCurrentNPC())
			{
				//Find the last talktext node completed
				int LastCompleted = 0;
				if (IsDialogTaskIN)
				{
					//Dialogs
					LastCompleted = GetLastCompletedTalkTask(QuestIN, DialogsTrack);
				}
				else
				{
					//Quests
					LastCompleted = GetLastCompletedTalkTask(QuestIN, QuestsTrack);
				}

				//Set it to current talktext
				SetActiveTalkText(GetTalkTextInfoForTalkTask(GetCurrentNPC(), QuestIN, LastCompleted, IsDialogTaskIN));
			}			
		}

		//Must always notify talk text changes after node reset
		OnNPCTalkTextsChanged.Broadcast();
	}	
	
	return succes;
}

bool UQuestManager::HandleTaskEnd_ResetTrackToNodeWithArrays(int QuestIN, bool IsDialogTaskIN, const FQuestTaskID& StartNodeIn, const FQuestTaskID& EndNodeIn, bool IncludeDisabledNodesIn, TArray<FQuestRep>& QuestsInventoryIN, TArray<FQuestTrack>& QuestsTrackIN, TArray<FQuestObjectTrack>& ObjectsTrackIN, TArray<FCompletedQuestTrack>& CompletedQuestsTrackIN, TArray<FQuestFailTrack>& QuestsFailTrackIN)
{
	if (!QuestsInventoryIN.IsValidIndex(QuestIN))
	{
		UE_LOG(QuestEditorLog, Log, TEXT("UQuestManager::HandleTaskEnd_ResetTrackToNodeWithArrays - Quest %i is invalid."), QuestIN);
		return false;
	}

	FQuest QuestLocal = UQuestsFunctionLibrary::QuestRepToQuest(QuestsInventoryIN[QuestIN]);

	//------------------------------------------------------------------
	//---------------------Intermediate nodes handle
	//------------------------------------------------------------------
	//are nodes not valid?
	if (!UQuestsFunctionLibrary::IsTaskIDValidInQuest(QuestLocal, StartNodeIn)) return false;
	if (!UQuestsFunctionLibrary::IsTaskIDValidInQuest(QuestLocal, EndNodeIn)) return false;

	//Get all nodes connected between the nodes that are not locked. This DOES NOT includes the start node. Not the end node
	TArray<FQuestTaskID> NodesToLock = GetAllNotLockedNodesBetweenNodes(QuestIN, QuestLocal, StartNodeIn, EndNodeIn, IncludeDisabledNodesIn, QuestsTrackIN);

	//Lock all nodes from start to end.
	for (int i = 0; i < NodesToLock.Num(); i++)
	{
		//Lock node
		SingleNodeLock(QuestIN, QuestLocal, NodesToLock[i], IsDialogTaskIN, QuestsTrackIN, ObjectsTrackIN);
	}

	//Lock Start node
	SingleNodeLock(QuestIN, QuestLocal, StartNodeIn, IsDialogTaskIN, QuestsTrackIN, ObjectsTrackIN);

	//Lock END node 
	SingleNodeLock(QuestIN, QuestLocal, EndNodeIn, IsDialogTaskIN, QuestsTrackIN, ObjectsTrackIN);

	/*At this point all nodes involved are locked. I activate now the Start node, wich is the one that is circular connected to the other nodes (the last one commited by the player).
	The node will not complete if is not active. I do not do a full activation, i assume the node CAN activate and only set the variables needed. Avoid dispatching also.
	*/

	//Activate StartNodeIn node partially. This is not the complete activation. Only the part needed, without dispatchers, to complete the node.
	//Change state to active
	SetNodeStateInTrack(QuestIN, StartNodeIn, EQuestTaskState::Active, QuestsTrackIN);

	//Activation order
	SetNodeActivationOrderInTrack(QuestIN, StartNodeIn, GetCurrentActivationOrder(QuestIN, QuestsTrackIN) + 1, QuestsTrackIN);

	//add to active track
	AddTaskIDToActiveTrack(QuestIN, StartNodeIn, QuestsTrackIN);

	//Start node completion
	CompleteNode(QuestIN, IsDialogTaskIN, QuestLocal, StartNodeIn, QuestsInventoryIN, QuestsTrackIN, ObjectsTrackIN, CompletedQuestsTrackIN, QuestsFailTrackIN);

	//Lock the node after to be able to reset in other passes.
	SingleNodeLock(QuestIN, QuestLocal, StartNodeIn, IsDialogTaskIN, QuestsTrackIN, ObjectsTrackIN);

	return true;
}

FQuestTask UQuestManager::GetTask(EQuestSelectedType SelectType, int QuestIN, int TaskIn, bool IsDialogIn) const
{	
	FQuestTask TaskLocal = FQuestTask();
	FQuest QuestLocal = FQuest();

	//get quest
	if (IsDialogIn)
	{
		if (DialogsInventory.IsValidIndex(QuestIN))
		{
			QuestLocal = UQuestsFunctionLibrary::QuestRepToQuest(DialogsInventory[QuestIN]);
		}
	}
	else
	{
		if (QuestInventory.IsValidIndex(QuestIN))
		{
			QuestLocal = UQuestsFunctionLibrary::QuestRepToQuest(QuestInventory[QuestIN]);
		}
	}

	if (QuestLocal.Tasks.IsValidIndex(TaskIn))
	{
		TaskLocal = QuestLocal.Tasks[TaskIn];
	}
		
	return TaskLocal;
}

FTaskTrack UQuestManager::GetTaskTrack(EQuestSelectedType SelectType, int QuestIN, int TaskIn, bool IsDialogIn) const
{
	FTaskTrack TaskLocal = FTaskTrack();
	FQuestTrack TrackLocal = GetQuestTrackInSlot(SelectType, QuestIN, IsDialogIn);

	//invalid task?
	if (!TrackLocal.TasksTracks.IsValidIndex(TaskIn)) return TaskLocal;

	TaskLocal = TrackLocal.TasksTracks[TaskIn];

	return TaskLocal;
}

UBaseQuestTask* UQuestManager::GetTaskObject(int QuestIN, int TaskIn, bool IsDialogIn) const
{
	UBaseQuestTask* ObjectTask = nullptr;

	if (IsDialogIn)
	{
		//Dialog
		if (!LocalObjectsDialogsTrack.IsValidIndex(QuestIN)) return ObjectTask;
		if (!LocalObjectsDialogsTrack[QuestIN].ObjectTasksTrack.IsValidIndex(TaskIn)) return ObjectTask;
		ObjectTask = LocalObjectsDialogsTrack[QuestIN].ObjectTasksTrack[TaskIn].BaseTaskReference;
	}
	else
	{
		//Quest
		if (!LocalObjectsQuestsTrack.IsValidIndex(QuestIN)) return ObjectTask;
		if (!LocalObjectsQuestsTrack[QuestIN].ObjectTasksTrack.IsValidIndex(TaskIn)) return ObjectTask;
		ObjectTask = LocalObjectsQuestsTrack[QuestIN].ObjectTasksTrack[TaskIn].BaseTaskReference;
	}

	return ObjectTask;
}

bool UQuestManager::SetSelectedQuest(int NewSelectedQuestIN)
{
	SelectedQuestSlot = NewSelectedQuestIN;
	SelectedQuestChanged.Broadcast(NewSelectedQuestIN);
	
	return true;
}

bool UQuestManager::SetSelectedQuestCompleted(int NewSelectedQuestCompIN)
{
	SelectedCompQuest = NewSelectedQuestCompIN;
	SelectedQuestCompChanged.Broadcast(NewSelectedQuestCompIN);
	return true;
}

TArray<FQuest> UQuestManager::GetQuestsInventory() const
{
	return ConvertQuestsInventoryRep(QuestInventory);	
}

TArray<FQuestRep> UQuestManager::GetQuestsRepInventory() const
{
	return QuestInventory;
}

TArray<FQuestRep> UQuestManager::GetDialogsRepInventory() const
{
	return DialogsInventory;
}

FQuest UQuestManager::GetQuestsInSlot(EQuestSelectedType SelectType, int QuestSLot, bool IsDialogIn) const
{
	FQuest QuestLocal;	

	if (IsDialogIn)
	{
		switch (SelectType)
		{
		case EQuestSelectedType::Active:
		{
			if (DialogsInventory.IsValidIndex(QuestSLot)) QuestLocal = UQuestsFunctionLibrary::QuestRepToQuest(DialogsInventory[QuestSLot]);
		}
		break;
		case EQuestSelectedType::Completed: QuestLocal = GetCompletedQuestBySlot(QuestSLot, IsDialogIn);
			break;
		default:
			break;
		}
	}
	else
	{
		switch (SelectType)
		{
		case EQuestSelectedType::Active:
		{		
			if (QuestInventory.IsValidIndex(QuestSLot)) QuestLocal = UQuestsFunctionLibrary::QuestRepToQuest(QuestInventory[QuestSLot]);
		}
		break;
		case EQuestSelectedType::Completed: QuestLocal = GetCompletedQuestBySlot(QuestSLot, IsDialogIn);
			break;
		default:
			break;
		}
	}
	
	return QuestLocal;
}

FQuestTrack UQuestManager::GetQuestTrackInSlot(EQuestSelectedType SelectType, int QuestSLot, bool IsDialogIn) const
{
	FQuestTrack QuestTrackLocal = FQuestTrack();

	if (IsDialogIn)
	{
		switch (SelectType)
		{
		case EQuestSelectedType::Active: if (DialogsTrack.IsValidIndex(QuestSLot)) QuestTrackLocal = DialogsTrack[QuestSLot];
			break;
		case EQuestSelectedType::Completed: if (CompletedDialogsTrack.IsValidIndex(QuestSLot)) QuestTrackLocal = CompletedDialogsTrack[QuestSLot].QuestTrack;
			break;
		default:
			break;
		}
	}
	else
	{
		switch (SelectType)
		{
		case EQuestSelectedType::Active: if (QuestsTrack.IsValidIndex(QuestSLot)) QuestTrackLocal = QuestsTrack[QuestSLot];
			break;
		case EQuestSelectedType::Completed: if (CompletedQuestsTrack.IsValidIndex(QuestSLot)) QuestTrackLocal = CompletedQuestsTrack[QuestSLot].QuestTrack;
			break;
		default:
			break;
		}
	}
	
	return QuestTrackLocal;
}

FQuest UQuestManager::GetSelectedQuests(EQuestSelectedType SelectType) const
{ 
	FQuest QuestLocal;
	
	switch (SelectType)
	{
	case EQuestSelectedType::Active: if (QuestInventory.IsValidIndex(SelectedQuestSlot)) QuestLocal = UQuestsFunctionLibrary::QuestRepToQuest(QuestInventory[SelectedQuestSlot]);
		break;
	case EQuestSelectedType::Completed: QuestLocal = GetCompletedQuestBySlot(SelectedCompQuest, false);
		break;
	default:
		break;

	}

	return QuestLocal;
}

FQuestTrack UQuestManager::GetSelectedQuestsTrack(EQuestSelectedType SelectType) const
{
	FQuestTrack TrackLocal;

	switch (SelectType)
	{
	case EQuestSelectedType::Active: if (QuestsTrack.IsValidIndex(SelectedQuestSlot)) TrackLocal = QuestsTrack[SelectedQuestSlot];
		break;
	case EQuestSelectedType::Completed: if (CompletedQuestsTrack.IsValidIndex(SelectedCompQuest)) TrackLocal = CompletedQuestsTrack[SelectedCompQuest].QuestTrack;
		break;
	default:
		break;
	}

	return TrackLocal;
}

int UQuestManager::GetSelectedQuestsSlot(EQuestSelectedType SelectType) const
{
	//Default is 0
	int Slot = 0;

	switch (SelectType)
	{
	case EQuestSelectedType::Active: Slot = SelectedQuestSlot;
		break;
	case EQuestSelectedType::Completed: Slot = SelectedCompQuest;
		break;
	default: 
		break;
	}

	return Slot;
}

TArray<FCompletedQuestTrack> UQuestManager::GetCompletedQuestsTrack() const
{
	return	CompletedQuestsTrack;
}

TArray<FQuestTrack> UQuestManager::GetQuestTracks() const
{
	return QuestsTrack;
}

bool UQuestManager::SetActiveTalkText(const FTalkTextInfo& TalkTextIn)
{
	//Set talktext
	ActiveTalkText = TalkTextIn;

	//A bit redundant since u know exactly when setting this via the function.
	ActiveTalkTextChanged.Broadcast(TalkTextIn);

	return true;
}

FTalkTextInfo UQuestManager::GetActiveTalkText() const
{
	return ActiveTalkText;
}

int UQuestManager::FindActiveTalkTextIndex(const TArray<FTalkTextInfo>& AllInfoIn) const
{
	int index = -1;
	for (int i = 0; i < AllInfoIn.Num(); i++)
	{
		//is this talk and quest slot?
		if (AllInfoIn[i] == ActiveTalkText)
		{
			index = i;
			break;
		}
	}
	return index;
}

AActor* UQuestManager::GetCurrentNPC() const
{
	return CurrentNPC;
}
