//Copyright 2020 Marchetti S. Alfredo I. All Rights Reserved.

#pragma once

#include "AssetTypeActions/AssetTypeActions_Blueprint.h"




#include "Factory/QuestEditorWidgetBlueprint.h"




class QUESTEDITORDEV_API AssetTypeActions_QuestTasksErrors : public FAssetTypeActions_Blueprint {
public:
	virtual FText GetName() const override { return FText::FromString("Quest Tasks Errors"); }
	virtual uint32 GetCategories() override { return MY_QuestCategory; }
	virtual FColor GetTypeColor() const override { return FColor(200, 50, 0); }
	virtual FText GetAssetDescription(const FAssetData& AssetData) const override { return FText::FromString("Quest task error objetcs."); }
	virtual UClass* GetSupportedClass() const override { return UQuestEditorTaskErrorBlueprint::StaticClass(); }

};
