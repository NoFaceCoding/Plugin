//Copyright 2020 Marchetti S. Alfredo I. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EditorSubsystem.h"

#include "QuestEditorSubsystem.generated.h"

class UBaseQuestEditor;

UCLASS()
class QUESTEDITORDEV_API UQuestEditorSubsystem : public UEditorSubsystem
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "Quest Editor")
	void SetQuestEditorReference(UBaseQuestEditor* InQuestEditorReference);

	UFUNCTION(BlueprintCallable, Category = "Quest Editor")
	UBaseQuestEditor* GetQuestEditorReference() const;

protected:
	UPROPERTY()
	UBaseQuestEditor* QuestEditorReference;

};
