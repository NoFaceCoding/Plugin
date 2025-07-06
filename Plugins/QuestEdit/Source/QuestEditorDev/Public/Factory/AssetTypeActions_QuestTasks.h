//Copyright 2020 Marchetti S. Alfredo I. All Rights Reserved.

#pragma once

#include "AssetTypeActions/AssetTypeActions_Blueprint.h"


#include "Task/BaseTaskBlueprint.h"

#include "Factory/QuestEditorWidgetBlueprint.h"




class QUESTEDITORDEV_API AssetTypeActions_QuestTasks : public FAssetTypeActions_Blueprint {
public:
	virtual FText GetName() const override { return FText::FromString("Quest Tasks"); }
	virtual uint32 GetCategories() override { return MY_QuestCategory; }
	virtual FColor GetTypeColor() const override { return FColor(150, 150, 0); }
	virtual FText GetAssetDescription(const FAssetData& AssetData) const override { return FText::FromString("Quest task objects."); }
	virtual UClass* GetSupportedClass() const override { return UBaseTaskBlueprint::StaticClass(); }

};