//Copyright 2020 Marchetti S. Alfredo I. All Rights Reserved.


#include "Factory/QuestFactory_GameWidgets.h"

//The asset type categories will let us access the various asset categories inside the Editor
#include "AssetTypeCategories.h"

//Category and widgets is in here
#include "Factory/QuestEditorWidgetBlueprint.h"

//Objects tha will spawn
#include "TaskSelectReward/BaseQuestRewardTab.h"
#include "TaskSelectReward/BaseQuestRewardSlot.h"





//To create widget you need
#include "UObject/Interface.h"
#include "Misc/MessageDialog.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetBlueprintGeneratedClass.h"
#include "WidgetBlueprint.h"
#include "Kismet2/KismetEditorUtilities.h"
#include "Modules/ModuleManager.h"

#include "Blueprint/WidgetTree.h"
#include "UMGEditorProjectSettings.h"
#include "ClassViewerModule.h"
#include "Kismet2/SClassPickerDialog.h"
#include "ClassViewerFilter.h"

#include "Components/CanvasPanel.h"


//----------------------------------------------------------------------
//------------------------------QuestFactory_Game
//----------------------------------------------------------------------

class FWidgetClassFilter : public IClassViewerFilter
{
public:
	/** All children of these classes will be included unless filtered out by another setting. */
	TSet <const UClass*> AllowedChildrenOfClasses;

	/** Disallowed class flags. */
	EClassFlags DisallowedClassFlags;

	virtual bool IsClassAllowed(const FClassViewerInitializationOptions& InInitOptions, const UClass* InClass, TSharedRef< FClassViewerFilterFuncs > InFilterFuncs) override
	{
		return !InClass->HasAnyClassFlags(DisallowedClassFlags)
			&& InFilterFuncs->IfInChildOfClassesSet(AllowedChildrenOfClasses, InClass) != EFilterReturn::Failed;
	}

	virtual bool IsUnloadedClassAllowed(const FClassViewerInitializationOptions& InInitOptions, const TSharedRef< const IUnloadedBlueprintData > InUnloadedClassData, TSharedRef< FClassViewerFilterFuncs > InFilterFuncs) override
	{
		return !InUnloadedClassData->HasAnyClassFlags(DisallowedClassFlags)
			&& InFilterFuncs->IfInChildOfClassesSet(AllowedChildrenOfClasses, InUnloadedClassData) != EFilterReturn::Failed;
	}
};

bool UQuestFactory_GameWidgets::ConfigureProperties()
{
	ParentClass = nullptr;

	// Load the classviewer module to display a class picker
	FClassViewerModule& ClassViewerModule = FModuleManager::LoadModuleChecked<FClassViewerModule>("ClassViewer");

	// Fill in options
	FClassViewerInitializationOptions Options;
	Options.Mode = EClassViewerMode::ClassPicker;
	Options.DisplayMode = EClassViewerDisplayMode::TreeView;

	//Classes to select
	Options.ExtraPickerCommonClasses.Add(UBaseQuestRewardSlot::StaticClass());
	Options.ExtraPickerCommonClasses.Add(UBaseQuestRewardTab::StaticClass());


	//Filter
	TSharedPtr<FWidgetClassFilter> Filter = MakeShareable(new FWidgetClassFilter);
	Options.ClassFilters.Add(Filter.ToSharedRef());

	Filter->DisallowedClassFlags = CLASS_Deprecated | CLASS_NewerVersionExists | CLASS_HideDropDown; // CLASS_Abstract |

	//Filter->AllowedChildrenOfClasses.Add(UUserWidget::StaticClass());

	Filter->AllowedChildrenOfClasses.Add(UBaseQuestRewardSlot::StaticClass());
	Filter->AllowedChildrenOfClasses.Add(UBaseQuestRewardTab::StaticClass());


	const FText TitleText = FText::FromString("Pick Quest Game Widget for New Widget Blueprint");

	UClass* ChosenClass = nullptr;
	const bool bPressedOk = SClassPickerDialog::PickClass(TitleText, Options, ChosenClass, UUserWidget::StaticClass());

	//Set the class of parent
	if (bPressedOk)
	{
		ParentClass = ChosenClass;
	}

	return bPressedOk;
}

UQuestFactory_GameWidgets::UQuestFactory_GameWidgets(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bCreateNew = true;
	bEditAfterNew = true;
	//Configure the class that this factory creates
	//BlueprintType = BPTYPE_Normal;
	SupportedClass = UWidgetBlueprint::StaticClass();
	ParentClass = UUserWidget::StaticClass();
}


bool UQuestFactory_GameWidgets::ShouldShowInNewMenu() const
{
	return true;
}

FText UQuestFactory_GameWidgets::GetDisplayName() const
{
	return FText::FromString("Game Widgets");
}

uint32 UQuestFactory_GameWidgets::GetMenuCategories() const
{
	//Let's place this asset in the Blueprints category in the Editor
	return MY_QuestCategory;
}

UObject* UQuestFactory_GameWidgets::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn, FName CallingContext)
{	
	//check(Class->IsChildOf(UBaseTooltipContentWidgetBlueprint::StaticClass()));
	UWidgetBlueprint* NewBP = Cast<UWidgetBlueprint>(FKismetEditorUtilities::CreateBlueprint(ParentClass, InParent, Name, BlueprintType,
		UWidgetBlueprint::StaticClass(), UWidgetBlueprint::StaticClass(), CallingContext));


	// Create a CanvasPanel to use as the default root widget
	if (NewBP->WidgetTree->RootWidget == nullptr)
	{
		UWidget* Root = NewBP->WidgetTree->ConstructWidget<UCanvasPanel>(UCanvasPanel::StaticClass());
		NewBP->WidgetTree->RootWidget = Root;
	}
 
	return NewBP;
}

UObject* UQuestFactory_GameWidgets::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	//create the editor asset 
	return FactoryCreateNew(Class, InParent, Name, Flags, Context, Warn, NAME_None);
}
