//Copyright 2020 Marchetti S. Alfredo I. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Quest/QuestTypes.h"
#include "NPCManager.generated.h"


//------------------------------------------DISPATCHERS DEFINITIONS-----------------------------------------------------


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FNoParamsDelegateNPCManager);




UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class QUESTEDITOR_API UNPCManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UNPCManager();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void Serialize(FArchive& Ar) override;


public:
	//For replication
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

	//------------------------------------------DISPATCHERS------------------------------------------------------
	//------------------------------------------Notify
	/*Called when the NPC is updated on repnotify.*/
	UPROPERTY(BlueprintAssignable, Category = "Notify")
		FNoParamsDelegateNPCManager NPCUpdated;

	//------------------------------------------PUBLIC EVENTS------------------------------------------------------
	
public:
	/**
	*	Changes the Npc information and load all the variables needed.
	*/
	UFUNCTION(Server, WithValidation, Reliable, BlueprintCallable, Category = "Server Events")
		void ROS_StartNPCManager(const FDataTableRowHandle& NPCIn);
		

	//------------------------------------------VARIABLES------------------------------------------------------
protected:
	
	//------------------------------------------NPC
	UPROPERTY(ReplicatedUsing = OnRep_NPCDef)
		FNPCQuestDefRep NPCQuestDefRep = FNPCQuestDefRep();
	UFUNCTION()
		void OnRep_NPCDef();


	//---------------------------------------Manager variables

	/*If true, the starting will be handled automatically using NPCInfo data on Begin play*/
	UPROPERTY(EditAnywhere, Category = "NPC Start")
		bool HandleStartAutomatically = true;

	/**
	*	Npc def to start manager automatically.
	*	DEPRECATED - This variable should not be available to set anymore. It is hidden now from bps. Deleting it could cause data loss.
	*/
	UPROPERTY()
		FNPCQuestDefRep NPCInfo = FNPCQuestDefRep(); 

	/*The NPC handle*/
	UPROPERTY(EditAnywhere, Category = "NPC Start")
		FDataTableRowHandle NPC = FDataTableRowHandle();

	//------------------------------------------FUNCTIONS------------------------------------------------------	
	//-----------------------------------------Manager	

	UFUNCTION()
		void  StartManagerFromDef(const FDataTableRowHandle& NPCIn);
public:
	/**
	*	Changes the npc datatable handle dinamically.
	*	Safe to call from construct, before loading is completed, use ROS_StartNPCManager otherwise.
	*/
	UFUNCTION(BlueprintCallable, Category = "NPC Start")
		void SetManagerNPC(const FDataTableRowHandle& NPCIn);

	//------------------------------------------Variables get
	/*Get NPC quests.*/
	UFUNCTION(BlueprintCallable, Category = "Variables")
		TArray <FQuest>  GetNPCQuests() const;

	UFUNCTION(BlueprintCallable, Category = "Variables")
		TArray <FQuestRep>  GetNPCQuestsRep() const;

	/*Get NPC dialogs.*/
	UFUNCTION(BlueprintCallable, Category = "Variables")
		TArray <FQuest>  GetNPCDialogs() const;

	UFUNCTION(BlueprintCallable, Category = "Variables")
		TArray <FQuestRep>  GetNPCDialogsRep() const;

	UFUNCTION(BlueprintCallable, Category = "Variables")
		FNPCQuestDefRep  GetNPCQuestDefRep() const;

	UFUNCTION(BlueprintCallable, Category = "Variables")
		FString  GetNPCName() const;
		
};
