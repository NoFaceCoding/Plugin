//Copyright 2020 Marchetti S. Alfredo I. All Rights Reserved.

#pragma once

#include "Stats/Stats.h"

//Groups
DECLARE_STATS_GROUP(TEXT("QuestEditor"), STATGROUP_QuestEditor, STATCAT_Advanced);

//Stats
DECLARE_CYCLE_STAT_EXTERN(TEXT("CompleteNode"), STAT_CompleteNode, STATGROUP_QuestEditor, );
DECLARE_CYCLE_STAT_EXTERN(TEXT("ActivateNode"), STAT_ActivateNode, STATGROUP_QuestEditor, );
