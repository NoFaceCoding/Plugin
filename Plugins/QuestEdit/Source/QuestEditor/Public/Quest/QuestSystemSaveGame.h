//Copyright 2020 Marchetti S. Alfredo I. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Quest/QuestTypes.h"
#include "QuestSystemSaveGame.generated.h"

UCLASS()
class QUESTEDITOR_API UQuestSystemSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:	
	UPROPERTY()
	TArray<FQuestRep> QuestInventorySaved;

	UPROPERTY()
	TArray<FCompletedQuestTrack> CompletedQuestsTrackSaved;
	
	UPROPERTY()
	TArray<FQuestTrack> QuestsTrackSaved;

	UPROPERTY()
	TArray<FQuestFailTrack> QuestsFailTrackSaved;

	UPROPERTY()
	TArray<FQuestRep> DialogsInventorySaved;

	UPROPERTY()
	TArray<FCompletedQuestTrack> CompletedDialogsTrackSaved;

	UPROPERTY()
	TArray<FQuestTrack> DialogsTrackSaved;

	UPROPERTY()
	TArray<FQuestFailTrack> DialogsFailTrackSaved;
};
