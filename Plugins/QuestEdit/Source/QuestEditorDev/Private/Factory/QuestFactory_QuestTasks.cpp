//Copyright 2020 Marchetti S. Alfredo I. All Rights Reserved.


#include "Factory/QuestFactory_QuestTasks.h"

//The asset type categories will let us access the various asset categories inside the Editor
#include "AssetTypeCategories.h"

//Category 
#include "Factory/QuestEditorWidgetBlueprint.h"

//Objects tha will spawn
#include "Task/BaseQuestTask.h"
#include "TaskLocation/Task_ReachLocationByName.h"
#include "TaskLocation/Task_ReachLocationByTags.h"
#include "TaskSelectReward/BaseQuestTask_SelectReward.h"
#include "TaskSelectReward/BaseQuestTask_SelectRewardOnNPC.h"
#include "TaskSelectReward/BaseQuestTask_SelectRewardOnLocation.h"
//Contains all blueprints
#include "Task/BaseTaskBlueprint.h"

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

class FQuestTasksClassFilter : public IClassViewerFilter
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

bool UQuestFactory_QuestTasks::ConfigureProperties()
{
	ParentClass = nullptr;

	// Load the classviewer module to display a class picker
	FClassViewerModule& ClassViewerModule = FModuleManager::LoadModuleChecked<FClassViewerModule>("ClassViewer");

	// Fill in options
	FClassViewerInitializationOptions Options;
	Options.Mode = EClassViewerMode::ClassPicker;
	Options.DisplayMode = EClassViewerDisplayMode::TreeView;

	//Classes to select
	Options.ExtraPickerCommonClasses.Add(UBaseQuestTask::StaticClass());
	Options.ExtraPickerCommonClasses.Add(UBaseQuestTask_SelectRewardOnNPC::StaticClass());
	Options.ExtraPickerCommonClasses.Add(UBaseQuestTask_SelectRewardOnLocation::StaticClass());
	Options.ExtraPickerCommonClasses.Add(UTask_ReachLocationByName::StaticClass());
	Options.ExtraPickerCommonClasses.Add(UTask_ReachLocationByTags::StaticClass());

	//Filter
	TSharedPtr<FQuestTasksClassFilter> Filter = MakeShareable(new FQuestTasksClassFilter);
	Options.ClassFilters.Add(Filter.ToSharedRef());

	Filter->DisallowedClassFlags = CLASS_Deprecated | CLASS_NewerVersionExists | CLASS_HideDropDown; 

	Filter->AllowedChildrenOfClasses.Add(UBaseQuestTask::StaticClass());
	Filter->AllowedChildrenOfClasses.Add(UBaseQuestTask_SelectReward::StaticClass());
	Filter->AllowedChildrenOfClasses.Add(UBaseQuestTask_SelectRewardOnNPC::StaticClass());
	Filter->AllowedChildrenOfClasses.Add(UBaseQuestTask_SelectRewardOnLocation::StaticClass());
	Filter->AllowedChildrenOfClasses.Add(UTask_ReachLocationByName::StaticClass());
	Filter->AllowedChildrenOfClasses.Add(UTask_ReachLocationByTags::StaticClass());


	const FText TitleText = FText::FromString("Pick Task object for New Object Blueprint");

	UClass* ChosenClass = nullptr;
	const bool bPressedOk = SClassPickerDialog::PickClass(TitleText, Options, ChosenClass, UBaseQuestTask::StaticClass());

	//Set the class of parent
	if (bPressedOk)
	{
		ParentClass = ChosenClass;
	}

	return bPressedOk;
}

bool UQuestFactory_QuestTasks::ShouldShowInNewMenu() const
{
	return true;
}

FText UQuestFactory_QuestTasks::GetDisplayName() const
{
	return FText::FromString("Quest Tasks");
}

UQuestFactory_QuestTasks::UQuestFactory_QuestTasks(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bCreateNew = true;
	bEditAfterNew = true;
	//Configure the class that this factory creates
	BlueprintType = BPTYPE_Normal;
	SupportedClass = UBaseTaskBlueprint::StaticClass();
	ParentClass = UBaseQuestTask::StaticClass();
}

uint32 UQuestFactory_QuestTasks::GetMenuCategories() const
{
	//Let's place this asset in the Blueprints category in the Editor
	return MY_QuestCategory;
}

UObject* UQuestFactory_QuestTasks::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn, FName CallingContext)
{
	check(Class->IsChildOf(UBaseTaskBlueprint::StaticClass()));
	UBaseTaskBlueprint* NewBP = CastChecked<UBaseTaskBlueprint>(FKismetEditorUtilities::CreateBlueprint(ParentClass, InParent, Name, BlueprintType,
		UBaseTaskBlueprint::StaticClass(), UBlueprintGeneratedClass::StaticClass(), CallingContext));
	return NewBP;
}

UObject* UQuestFactory_QuestTasks::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	//Create the editor asset 
	return FactoryCreateNew(Class, InParent, Name, Flags, Context, Warn, NAME_None);
}