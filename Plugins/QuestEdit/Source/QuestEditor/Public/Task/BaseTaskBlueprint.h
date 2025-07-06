//Copyright 2020 Marchetti S. Alfredo I. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/Blueprint.h"
#include "UObject/ObjectMacros.h"

#include "BaseTaskBlueprint.generated.h"

/**
 * Blueprint for editing BaseQuestTask Objects
 * This is the basic object for any task for quests. 
 * The execution logic must always end with a call to the EndTask Function unless is Activate Condition Only.
 */

UCLASS()
class QUESTEDITOR_API UBaseTaskBlueprint : public UBlueprint 
{
	GENERATED_BODY()
		
public:
	UBaseTaskBlueprint(const FObjectInitializer& ObjectInitializer);

};

/**
 * Blueprint for editing BaseQuestTask_SelectRewardOnNPC Objects
 * This is the basic object for any NPC reward task for quests.
 * StringA on the payload should always be the NPC, and a NPC selector must be used in this field for the task info.
 */

UCLASS()
class QUESTEDITOR_API UBaseNPCRewardTaskBlueprint : public UBlueprint
{
	GENERATED_BODY()

public:
	UBaseNPCRewardTaskBlueprint(const FObjectInitializer& ObjectInitializer);

};

