//Copyright 2020 Marchetti S. Alfredo I. All Rights Reserved.


#include "Factory/QuestFactory_QuestTasksErrors.h"

//The asset type categories will let us access the various asset categories inside the Editor
#include "AssetTypeCategories.h"

//Category is in here
#include "Factory/QuestEditorWidgetBlueprint.h"

//Objects tha will spawn
#include "Errors/QuestEditorTaskError.h"
#include "ErrorsTalk/QuestEditorTalkTaskError.h"


//Contains all blueprints
#include "Factory/QuestEditorWidgetBlueprint.h"

//To create tasks you need
#include "UObject/Interface.h"
#include "Misc/MessageDialog.h"
#include "Modules/ModuleManager.h"
#include "Kismet2/KismetEditorUtilities.h"
#include "ClassViewerModule.h"
#include "Kismet2/SClassPickerDialog.h"
#include "ClassViewerFilter.h"




//----------------------------------------------------------------------
//------------------------------QuestFactory_QuestTasks
//----------------------------------------------------------------------

class FQuestTasksErrorsClassFilter : public IClassViewerFilter
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

bool UQuestFactory_QuestTasksErrors::ConfigureProperties()
{
	ParentClass = nullptr;

	// Load the classviewer module to display a class picker
	FClassViewerModule& ClassViewerModule = FModuleManager::LoadModuleChecked<FClassViewerModule>("ClassViewer");

	// Fill in options
	FClassViewerInitializationOptions Options;
	Options.Mode = EClassViewerMode::ClassPicker;
	Options.DisplayMode = EClassViewerDisplayMode::TreeView;

	//Classes to select
	Options.ExtraPickerCommonClasses.Add(UQuestEditorTaskError::StaticClass());
	Options.ExtraPickerCommonClasses.Add(UQuestEditorTalkTaskError::StaticClass());

	//Filter
	TSharedPtr<FQuestTasksErrorsClassFilter> Filter = MakeShareable(new FQuestTasksErrorsClassFilter);
	Options.ClassFilters.Add(Filter.ToSharedRef());

	Filter->DisallowedClassFlags = CLASS_Deprecated | CLASS_NewerVersionExists | CLASS_HideDropDown; 

	Filter->AllowedChildrenOfClasses.Add(UQuestEditorTaskError::StaticClass());
	Filter->AllowedChildrenOfClasses.Add(UQuestEditorTalkTaskError::StaticClass());


	const FText TitleText = FText::FromString("Pick Task Error object for New Object Blueprint");

	UClass* ChosenClass = nullptr;
	const bool bPressedOk = SClassPickerDialog::PickClass(TitleText, Options, ChosenClass, UObject::StaticClass());

	//Set the class of parent
	if (bPressedOk)
	{
		ParentClass = ChosenClass;
	}

	return bPressedOk;
}

bool UQuestFactory_QuestTasksErrors::ShouldShowInNewMenu() const
{
	return true;
}

FText UQuestFactory_QuestTasksErrors::GetDisplayName() const
{
	return FText::FromString("Quest Tasks Errors");
}

UQuestFactory_QuestTasksErrors::UQuestFactory_QuestTasksErrors(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bCreateNew = true;
	bEditAfterNew = true;
	//Configure the class that this factory creates
	BlueprintType = BPTYPE_Normal;
	SupportedClass = UQuestEditorTaskErrorBlueprint::StaticClass();
	ParentClass = UObject::StaticClass();
}

uint32 UQuestFactory_QuestTasksErrors::GetMenuCategories() const
{
	//Let's place this asset in the Blueprints category in the Editor
	return MY_QuestCategory;
}

UObject* UQuestFactory_QuestTasksErrors::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn, FName CallingContext)
{
	check(Class->IsChildOf(UQuestEditorTaskErrorBlueprint::StaticClass()));
	UQuestEditorTaskErrorBlueprint* NewBP = CastChecked<UQuestEditorTaskErrorBlueprint>(FKismetEditorUtilities::CreateBlueprint(ParentClass, InParent, Name, BlueprintType,
		UQuestEditorTaskErrorBlueprint::StaticClass(), UBlueprint::StaticClass(), CallingContext));
	return NewBP;
}

UObject* UQuestFactory_QuestTasksErrors::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	//Create the editor asset 
	return FactoryCreateNew(Class, InParent, Name, Flags, Context, Warn, NAME_None);
}