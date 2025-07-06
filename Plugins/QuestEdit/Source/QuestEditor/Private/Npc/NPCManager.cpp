//Copyright 2020 Marchetti S. Alfredo I. All Rights Reserved.


#include "Npc/NPCManager.h"
#include "Quest/QuestsFunctionLibrary.h"
#include "Runtime/Engine/Public/Net/UnrealNetwork.h"

//Log
#include "QuestEditor.h"

// Sets default values for this component's properties
UNPCManager::UNPCManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
	SetAutoActivate(true);

	// ...	
}

// Called when the game starts

void UNPCManager::BeginPlay()
{
	Super::BeginPlay();
	// ...	

	//If load is handled independently, load game
	if (HandleStartAutomatically)
	{
		//Only server handles NPC start
		if (GetNetMode() == NM_ListenServer || GetNetMode() == NM_Standalone || GetNetMode() == NM_DedicatedServer)
		{
			//Start manager
			StartManagerFromDef(NPC);
		}
	}
}

void UNPCManager::Serialize(FArchive& Ar)
{
	Super::Serialize(Ar);	

	if (Ar.IsLoading() || Ar.IsSaving())
	{
		//The new variable is not set?
		if (NPC.RowName == "None" || NPC.DataTable == nullptr)
		{
			//Something to fix?
			if (NPCInfo.NPCDefDTName != "None" || NPCInfo.Datatable != nullptr)
			{
				//Update new
				UDataTable* DT;
				DT = UQuestsFunctionLibrary::GetDTFromPath(NPCInfo.Datatable);
				NPC.DataTable = DT;
				NPC.RowName = NPCInfo.NPCDefDTName;

				//Delete old
				NPCInfo.NPCDefDTName = "None";
				NPCInfo.Datatable = nullptr;


				UE_LOG(QuestEditorLog, Warning, TEXT("UNPCManager::Serialize - NPC manager still using  deprecated variable. Please save the NPC actor that contains the NPCManager."));
			}
		}		
	}
}

//Replicated
void UNPCManager::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	//Repnotifys
	DOREPLIFETIME_CONDITION_NOTIFY(UNPCManager, NPCQuestDefRep, COND_None, REPNOTIFY_Always);
}

//Functions RepNotify
void UNPCManager::OnRep_NPCDef()
{
	if (GetNetMode() != NM_DedicatedServer) NPCUpdated.Broadcast();
}

void UNPCManager::StartManagerFromDef(const FDataTableRowHandle& NPCQuestDefIN)
{
	//Set Npc Def from the datatable row
	NPCQuestDefRep.Datatable = NPCQuestDefIN.DataTable;
	NPCQuestDefRep.NPCDefDTName = NPCQuestDefIN.RowName;
}

void UNPCManager::SetManagerNPC(const FDataTableRowHandle& NPCIn)
{
	//set info
	NPC = NPCIn;
}

TArray<FQuest> UNPCManager::GetNPCQuests() const
{
	//Get NPC info
	FNPCQuestDef NPCDefLocal = UQuestsFunctionLibrary::NPCQuestDefRepToNPCQuestDef(NPCQuestDefRep);

	TArray <FQuest> QuestsLocal = TArray <FQuest>();

	for (int i = 0; i < NPCDefLocal.Quests.Num(); i++)
	{
		QuestsLocal.Add(UQuestsFunctionLibrary::QuestRepToQuest(NPCDefLocal.Quests[i]));
	}

	return QuestsLocal;
}

TArray<FQuestRep> UNPCManager::GetNPCQuestsRep() const
{
	//Get NPC info
	FNPCQuestDef NPCDefLocal = UQuestsFunctionLibrary::NPCQuestDefRepToNPCQuestDef(NPCQuestDefRep);

	return NPCDefLocal.Quests;
}

TArray<FQuest> UNPCManager::GetNPCDialogs() const
{
	//Get NPC info
	FNPCQuestDef NPCDefLocal = UQuestsFunctionLibrary::NPCQuestDefRepToNPCQuestDef(NPCQuestDefRep);

	TArray <FQuest> QuestsLocal = TArray <FQuest>();
	
	for (int i = 0; i < NPCDefLocal.Dialogs.Num(); i++)
	{
		QuestsLocal.Add(UQuestsFunctionLibrary::QuestRepToQuest(NPCDefLocal.Dialogs[i]));
	}

	return QuestsLocal;
}

TArray<FQuestRep> UNPCManager::GetNPCDialogsRep() const
{
	//Get NPC info
	FNPCQuestDef NPCDefLocal = UQuestsFunctionLibrary::NPCQuestDefRepToNPCQuestDef(NPCQuestDefRep);

	return NPCDefLocal.Dialogs;
}

FNPCQuestDefRep UNPCManager::GetNPCQuestDefRep() const
{
	return NPCQuestDefRep;
}

FString UNPCManager::GetNPCName() const
{	
	FNPCQuestDef NPCDefLocal = UQuestsFunctionLibrary::NPCQuestDefRepToNPCQuestDef(NPCQuestDefRep);
	return NPCDefLocal.NPCName;
}

//Events
void UNPCManager::ROS_StartNPCManager_Implementation(const FDataTableRowHandle& NPCIn)
{
	StartManagerFromDef(NPCIn);
	return;
}

bool UNPCManager::ROS_StartNPCManager_Validate(const FDataTableRowHandle& NPCIn)
{
	return true;
}
