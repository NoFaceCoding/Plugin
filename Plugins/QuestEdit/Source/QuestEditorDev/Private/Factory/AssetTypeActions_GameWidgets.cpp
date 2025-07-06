#include "Factory/AssetTypeActions_GameWidgets.h"


#include "Misc/MessageDialog.h"
#include "WidgetBlueprintEditor.h"

#include "Factory/QuestEditorWidgetBlueprint.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////// FAssetTypeActions_EditorUtilityWidget    //Copied from there   
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#define LOCTEXT_NAMESPACE "AssetTypeActions_GameWidgets"



void AssetTypeActions_GameWidgets::OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<class IToolkitHost> EditWithinLevelEditor)
{
	EToolkitMode::Type Mode = EditWithinLevelEditor.IsValid() ? EToolkitMode::WorldCentric : EToolkitMode::Standalone;

	for (UObject* Object : InObjects)
	{
		UBlueprint* Blueprint = Cast<UBlueprint>(Object);
		if (Blueprint && Blueprint->SkeletonGeneratedClass && Blueprint->GeneratedClass)
		{
			TSharedRef< FWidgetBlueprintEditor > NewBlueprintEditor(new FWidgetBlueprintEditor());

			TArray<UBlueprint*> Blueprints;
			Blueprints.Add(Blueprint);
			NewBlueprintEditor->InitWidgetBlueprintEditor(EToolkitMode::Standalone, nullptr, Blueprints, true);
		}
		else
		{
			FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("FailedToLoadWidgetBlueprint", "Widget could not be loaded because it derives from an invalid class.\nCheck to make sure the parent class for this blueprint hasn't been removed!"));
		}
	}
}

#undef LOCTEXT_NAMESPACE

