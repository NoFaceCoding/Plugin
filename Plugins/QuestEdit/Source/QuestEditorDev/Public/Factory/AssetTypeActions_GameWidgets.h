//Copyright 2020 Marchetti S. Alfredo I. All Rights Reserved.

#pragma once

#include "AssetTypeActions/AssetTypeActions_Blueprint.h"



#include "Factory/QuestEditorWidgetBlueprint.h"



class QUESTEDITORDEV_API AssetTypeActions_GameWidgets : public FAssetTypeActions_Base {
public:
	virtual FText GetName() const override { return FText::FromString("Quest Game Widgets"); }
	virtual uint32 GetCategories() override { return MY_QuestCategory; }
	virtual FColor GetTypeColor() const override { return FColor(250, 20, 250); }
	virtual FText GetAssetDescription(const FAssetData& AssetData) const override { return FText::FromString("Widgets for the quests in game."); }
	virtual UClass* GetSupportedClass() const override { return UBaseQuestGameWidgetBlueprint::StaticClass(); }
		
	//editor
	virtual void OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<class IToolkitHost> EditWithinLevelEditor = TSharedPtr<IToolkitHost>()) override;

}; 

