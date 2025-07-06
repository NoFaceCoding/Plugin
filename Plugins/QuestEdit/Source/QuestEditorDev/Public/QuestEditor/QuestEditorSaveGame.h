//Copyright 2020 Marchetti S. Alfredo I. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Quest/QuestTypes.h"
#include "QuestEditor/QuestTypesDev.h"
#include "QuestEditorSaveGame.generated.h"

/**
 *		Savegame object for the Quest Editor.
 */

UCLASS()
class QUESTEDITORDEV_API UQuestEditorSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	//------------------------------------------Quest editor save game variables

	UPROPERTY()
		EQuestEditor_EditMode CurrentEditorMode;
		
	UPROPERTY()
		int UndoQuestIndexSaved;

	UPROPERTY()
		int UndoDialogIndexSaved;

	UPROPERTY()
		float ViewportScaleSaved;

	UPROPERTY()
		TArray<FQuest> PreviousQuests;
	
	UPROPERTY()
		TArray<FQuest> UndoHistoryQuests;

	UPROPERTY()
		TArray<FQuest> PreviousDialogs;
	
	UPROPERTY()
		TArray<FQuest> UndoHistoryDialogs;
};
