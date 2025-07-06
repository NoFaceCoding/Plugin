//Copyright 2020 Marchetti S. Alfredo I. All Rights Reserved.


#include "Factory/QuestFactory_QuestEditorWidgets.h"

//The asset type categories will let us access the various asset categories inside the Editor
#include "AssetTypeCategories.h"


//////////////////////////////////////////////////////////////
/////////////////To create widgets
//////////////////////////////////////////////////////////////

//SelectorCustomPayload
#include "CustomPayload/CustomPayloadSelectorWidget.h"
//Selectors
#include "Selectors/BaseFloatSelectorWidget.h"
#include "Selectors/BaseObjectClassSelectorWidget.h"
#include "Selectors/BaseStringSelectorWidget.h"
#include "Selectors/BaseTagSelectorWidget.h"
#include "Selectors/BaseVectorSelectorWidget.h"
//Preview Tasks
#include "Preview/BaseTaskContentPreviewWidget.h"
#include "Preview/BaseTaskTitlePreviewWidget.h"

//Selectors talk Task
#include "TalkTaskCustomization/BaseTalkTaskSelectorWidget.h"
//Preview Talk Tasks
#include "TalkTaskCustomization/BaseTalkTaskContentPreviewWidget.h"


//Tooltips
#include "Tooltip/BaseTooltipContentWidget.h"

//////////////////////////////////////////////////////////////
//Contains all widgets blueprints and cat
#include "Factory/QuestEditorWidgetBlueprint.h"


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
//------------------------------UQuestFactory_QuestEditorWidgets
//----------------------------------------------------------------------

class FEditorWidgetClassFilter : public IClassViewerFilter
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

bool UQuestFactory_QuestEditorWidgets::ConfigureProperties()
{
	ParentClass = nullptr;

	// Load the classviewer module to display a class picker
	FClassViewerModule& ClassViewerModule = FModuleManager::LoadModuleChecked<FClassViewerModule>("ClassViewer");

	// Fill in options
	FClassViewerInitializationOptions Options;
	Options.Mode = EClassViewerMode::ClassPicker;
	Options.DisplayMode = EClassViewerDisplayMode::TreeView;

	//Classes to select
	Options.ExtraPickerCommonClasses.Add(UCustomPayloadSelectorWidget::StaticClass());

	Options.ExtraPickerCommonClasses.Add(UBaseFloatSelectorWidget::StaticClass());
	Options.ExtraPickerCommonClasses.Add(UBaseObjectClassSelectorWidget::StaticClass());
	Options.ExtraPickerCommonClasses.Add(UBaseStringSelectorWidget::StaticClass());
	Options.ExtraPickerCommonClasses.Add(UBaseTagSelectorWidget::StaticClass());
	Options.ExtraPickerCommonClasses.Add(UBaseVectorSelectorWidget::StaticClass());

	Options.ExtraPickerCommonClasses.Add(UBaseTaskContentPreviewWidget::StaticClass());
	Options.ExtraPickerCommonClasses.Add(UBaseTaskTitlePreviewWidget::StaticClass());

	Options.ExtraPickerCommonClasses.Add(UBaseTalkTaskSelectorWidget::StaticClass());
	Options.ExtraPickerCommonClasses.Add(UBaseTalkTaskContentPreviewWidget::StaticClass());

	Options.ExtraPickerCommonClasses.Add(UBaseTooltipContentWidget::StaticClass());


	//Filter
	TSharedPtr<FEditorWidgetClassFilter> Filter = MakeShareable(new FEditorWidgetClassFilter);
	Options.ClassFilters.Add(Filter.ToSharedRef());

	Filter->DisallowedClassFlags = CLASS_Deprecated | CLASS_NewerVersionExists | CLASS_HideDropDown; // CLASS_Abstract |

	Filter->AllowedChildrenOfClasses.Add(UCustomPayloadSelectorWidget::StaticClass());

	Filter->AllowedChildrenOfClasses.Add(UBaseFloatSelectorWidget::StaticClass());
	Filter->AllowedChildrenOfClasses.Add(UBaseObjectClassSelectorWidget::StaticClass());
	Filter->AllowedChildrenOfClasses.Add(UBaseStringSelectorWidget::StaticClass());
	Filter->AllowedChildrenOfClasses.Add(UBaseTagSelectorWidget::StaticClass());
	Filter->AllowedChildrenOfClasses.Add(UBaseVectorSelectorWidget::StaticClass());

	Filter->AllowedChildrenOfClasses.Add(UBaseTaskContentPreviewWidget::StaticClass());
	Filter->AllowedChildrenOfClasses.Add(UBaseTaskTitlePreviewWidget::StaticClass());

	Filter->AllowedChildrenOfClasses.Add(UBaseTalkTaskSelectorWidget::StaticClass());
	Filter->AllowedChildrenOfClasses.Add(UBaseTalkTaskContentPreviewWidget::StaticClass());

	Filter->AllowedChildrenOfClasses.Add(UBaseTooltipContentWidget::StaticClass());


	const FText TitleText = FText::FromString("Pick Quest Editor Widget for New Widget Blueprint");

	UClass* ChosenClass = nullptr;
	const bool bPressedOk = SClassPickerDialog::PickClass(TitleText, Options, ChosenClass, UUserWidget::StaticClass());

	//Set the class of parent
	if (bPressedOk)
	{
		ParentClass = ChosenClass;
	}

	return bPressedOk;
}

UQuestFactory_QuestEditorWidgets::UQuestFactory_QuestEditorWidgets(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bCreateNew = true;
	bEditAfterNew = true;
	//Configure the class that this factory creates
	//BlueprintType = BPTYPE_Normal;
	SupportedClass = UWidgetBlueprint::StaticClass();
	ParentClass = UUserWidget::StaticClass();
}


bool UQuestFactory_QuestEditorWidgets::ShouldShowInNewMenu() const
{
	return true;
}

FText UQuestFactory_QuestEditorWidgets::GetDisplayName() const
{
	return FText::FromString("Quest Editor Widgets");
}

uint32 UQuestFactory_QuestEditorWidgets::GetMenuCategories() const
{
	//Let's place this asset in the Blueprints category in the Editor
	return MY_QuestCategory;
}

UObject* UQuestFactory_QuestEditorWidgets::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn, FName CallingContext)
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

UObject* UQuestFactory_QuestEditorWidgets::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	//create the editor asset 
	return FactoryCreateNew(Class, InParent, Name, Flags, Context, Warn, NAME_None);
}
