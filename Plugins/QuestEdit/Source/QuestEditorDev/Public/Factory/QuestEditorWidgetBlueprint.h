//Copyright 2020 Marchetti S. Alfredo I. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "Components/Widget.h"

#include "Engine/BlueprintCore.h"
#include "Engine/Blueprint.h"

#include "AssetTypeCategories.h"
#include "EditorUtilityWidgetBlueprint.h"

#include "QuestEditorWidgetBlueprint.generated.h"



 //----------------------------------------------------------------------
 //-------------------------------Quest custom category
 //----------------------------------------------------------------------

static EAssetTypeCategories::Type MY_QuestCategory;

//----------------------------------------------------------------------
//-------------------------------Blueprints
//----------------------------------------------------------------------


//This file contains blueprints for all quest editor objects. I just have diferent ones for customizing their descriptions.


/**
 *	Base object to analize payloads for errors inside the quest editor.
 */

UCLASS()
class QUESTEDITORDEV_API UQuestEditorTaskErrorBlueprint : public UBlueprint
{
	GENERATED_BODY()

};

//The talk task error bp cannot be deleted since the base talktask errors use this one, and mb some user created ones
//All new ones use the upper one. UQuestEditorTaskErrorBlueprint, no matter if task or talktask

/**
 *	Base object to analize talk tasks for errors inside the quest editor.
 */

UCLASS()
class QUESTEDITORDEV_API UQuestEditorTalkTaskErrorBlueprint : public UBlueprint
{
	GENERATED_BODY()

};

/**
 *   Base widget for in game blueprints.
 */
UCLASS(BlueprintType)
class QUESTEDITORDEV_API UBaseQuestGameWidgetBlueprint : public UWidgetBlueprint
{
	GENERATED_BODY()

};

/**
 *   Base widget for Quest Editor blueprints.
 */
UCLASS(BlueprintType)
class QUESTEDITORDEV_API UBaseQuestEditorWidgetBlueprint : public UWidgetBlueprint
{
	GENERATED_BODY()

};
