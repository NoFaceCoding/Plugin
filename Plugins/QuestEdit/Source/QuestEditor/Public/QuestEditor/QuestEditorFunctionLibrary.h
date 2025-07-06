//Copyright 2020 Marchetti S. Alfredo I. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Quest/QuestTypes.h"
#include "QuestEditorFunctionLibrary.generated.h"


/**
 * 
 */
UCLASS()
class QUESTEDITOR_API UQuestEditorFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	   
		/*Gets Quest row from the quest DT path*/
		UFUNCTION(BlueprintCallable, Category = "QuestEditor")
			static FQuest GetDTQuestRowFromPath(FName RowName, TSoftObjectPtr <UDataTable> DTPath);

		/*Gets dialog row from the dialogs DT and converts it to quest struct*/
		UFUNCTION(BlueprintCallable, Category = "QuestEditor")
			static FQuest GetDTDialogRowFromPath(FName RowName, TSoftObjectPtr <UDataTable> DTPath);
		
		/*Converts a quest structure to a dialog structure.*/
		UFUNCTION()
			static FDialog ConvertQuestToDialog(const FQuest& QuestIn);
	
		/*Converts a dialog structure to a quest structure.*/
		UFUNCTION()
			static FQuest ConvertDialogToQuest(const FDialog& DialogIn);

		/**
		*	Gets the total amount of quest start nodes.
		*	All nodes that dont have superior nodes are quest starts.
		*/
		UFUNCTION(BlueprintCallable, Category = "QuestEditor-Node")
			static int GetAmountOfQuestStarts(const FQuest& QuestIn);

		/**
		*	Gets the total amount of quest end nodes.
		*	All nodes that dont have inferior nodes are quest starts.
		*/
		UFUNCTION(BlueprintCallable, Category = "QuestEditor-Node")
			static int GetAmountOfQuestEnds(const FQuest& QuestIn);

		/**
		*	Gets all tasks of a quest that are quest ends.
		*	Quest end tasks have no inferior nodes.
		*/
		UFUNCTION(BlueprintCallable, Category = "QuestEditor-Node")
			static TArray <FQuestTaskID> GetQuestEndTasks(const FQuest& QuestIn);

		/**
		*	Gets all tasks IDs of a quest.
		*/
		UFUNCTION(BlueprintCallable, Category = "QuestEditor-Node")
			static TArray <FQuestTaskID> GetQuestTasksIDs(const FQuest& QuestIn);
	
};
