//Copyright 2020 Marchetti S. Alfredo I. All Rights Reserved.


#include "QuestEditor/QuestEditorDevFunctionLibrary.h"

#include "Task/BaseQuestTask.h"
#include "Task/Task_HasQuestCompleted.h"
#include "Task/Task_HasQuestCompletedWithEnding.h"
#include "Quest/QuestsFunctionLibrary.h"
#include "QuestEditor/QuestEditorFunctionLibrary.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/WidgetTree.h"

#include "QuestEditor/BaseQuestEditor.h"
#include "Kismet/KismetStringLibrary.h"
#include "AssetRegistry/AssetRegistryModule.h"

#include "QuestEditorDev.h"
#include "Engine/Engine.h"

#include "QuestEditorSettings.h"
#include "InputCoreTypes.h"
#include "Subsystem/QuestEditorSubsystem.h"

#include "Editor.h"

UQuestEditorSettings* UQuestEditorDevFunctionLibrary::GetQuestEditorSettings()
{
	return QuestEditorDev::Get().GetSettings();
}

FLinearColor UQuestEditorDevFunctionLibrary::GetSettings_TaskBGColor()
{
	return QuestEditorDev::Get().GetSettings()->TasksBackgroundColor;
}

FLinearColor UQuestEditorDevFunctionLibrary::GetSettings_TalkTaskBGColor()
{
	return QuestEditorDev::Get().GetSettings()->TalkTasksBackgroundColor;
}

FVector2D UQuestEditorDevFunctionLibrary::GetSettings_DetailsPanelSize()
{
	return QuestEditorDev::Get().GetSettings()->DetailsPanelSize;
}

float UQuestEditorDevFunctionLibrary::GetSettings_ArrowsSensitivity()
{
	return QuestEditorDev::Get().GetSettings()->ArrowsSensitivity;
}

float UQuestEditorDevFunctionLibrary::GetSettings_ZoomSensitivity()
{
	return QuestEditorDev::Get().GetSettings()->ZoomSensitivity;
}

int UQuestEditorDevFunctionLibrary::GetSettings_PreviousVersion()
{
	return QuestEditorDev::Get().GetSettings()->PreviousQEVersion;
}

float UQuestEditorDevFunctionLibrary::GetSettings_ArrowSize()
{
	return QuestEditorDev::Get().GetSettings()->ArrowSize;
}

float UQuestEditorDevFunctionLibrary::GetSettings_LineThickness()
{
	return QuestEditorDev::Get().GetSettings()->LineThickness;
}

FInputChord UQuestEditorDevFunctionLibrary::GetDefaultKeybindForAction(EQuestKeyBindAction ActionIn)
{
	FInputChord LocalInput = FInputChord();
	
	switch (ActionIn)
	{
	case EQuestKeyBindAction::MoveUp: LocalInput.Key = EKeys::Up;
		break;
	case EQuestKeyBindAction::MoveDown: LocalInput.Key = EKeys::Down;
		break;
	case EQuestKeyBindAction::MoveLeft: LocalInput.Key = EKeys::Left;
		break;
	case EQuestKeyBindAction::MoveRight:  LocalInput.Key = EKeys::Right;
		break;
	case EQuestKeyBindAction::ZoomIn: LocalInput.Key = EKeys::Add;
		break;
	case EQuestKeyBindAction::ZoomOut:  LocalInput.Key = EKeys::Subtract;
		break;
	case EQuestKeyBindAction::AlignNodesUp: LocalInput.Key = EKeys::W;
		break;
	case EQuestKeyBindAction::AlignNodesDown: LocalInput.Key = EKeys::S;
		break;
	case EQuestKeyBindAction::AlignNodesLeft: LocalInput.Key = EKeys::A;
		break;
	case EQuestKeyBindAction::AlignNodesRight: LocalInput.Key = EKeys::D;
		break;
	case EQuestKeyBindAction::AlignNodesUpAndEven:
	{
		LocalInput.Key = EKeys::W;
		LocalInput.bAlt = true;
	}
		break;
	case EQuestKeyBindAction::AlignNodesDownAndEven:
	{
		LocalInput.Key = EKeys::S;
		LocalInput.bAlt = true;
	}
		break;
	case EQuestKeyBindAction::AlignNodesLeftAndEven:
	{
		LocalInput.Key = EKeys::A;
		LocalInput.bAlt = true;
	}
		break;
	case EQuestKeyBindAction::AlignNodesRightAndEven:
	{
		LocalInput.Key = EKeys::D;
		LocalInput.bAlt = true;
	}
		break;
	case EQuestKeyBindAction::AlignNodesToGrid: LocalInput.Key = EKeys::Q;
		break;
	case EQuestKeyBindAction::EvenXDistribution: LocalInput.Key = EKeys::F;
		break;
	case EQuestKeyBindAction::EvenYDistribution: LocalInput.Key = EKeys::R;
		break;
	case EQuestKeyBindAction::DeleteNodes: LocalInput.Key = EKeys::Delete;
		break;
	case EQuestKeyBindAction::IterateStartNodes: LocalInput.Key = EKeys::Home;
		break;
	case EQuestKeyBindAction::IterateEndNodes: LocalInput.Key = EKeys::End;
		break;
	case EQuestKeyBindAction::IterateNodesForward: LocalInput.Key = EKeys::PageUp;
		break;
	case EQuestKeyBindAction::IterateNodesBackward: LocalInput.Key = EKeys::PageDown;
		break;
	case EQuestKeyBindAction::FindSelectedNode: LocalInput.Key = EKeys::SpaceBar;
		break;
	case EQuestKeyBindAction::CenterViewport: LocalInput.Key = EKeys::P;
		break;
	case EQuestKeyBindAction::ResetZoom: LocalInput.Key = EKeys::L;
		break;
	case EQuestKeyBindAction::Undo:
	{
		LocalInput.Key = EKeys::Z;
		LocalInput.bCtrl = true;
	}
		break;
	case EQuestKeyBindAction::Redo:
	{
		LocalInput.Key = EKeys::Y;
		LocalInput.bCtrl = true;
	}
		break;
	case EQuestKeyBindAction::CopyNodes: 
	{
		LocalInput.Key = EKeys::C;
		LocalInput.bCtrl = true;
	}
		break;
	case EQuestKeyBindAction::CutNodes:
	{
		LocalInput.Key = EKeys::X;
		LocalInput.bCtrl = true;
	}
	break;
	case EQuestKeyBindAction::PasteNodes:
	{
		LocalInput.Key = EKeys::V;
		LocalInput.bCtrl = true;
	}
	break;
	case EQuestKeyBindAction::ConnectSelectedNodes: LocalInput.Key = EKeys::C;
		break;
	case EQuestKeyBindAction::ConnectSelectedNodesAsGrid: LocalInput.Key = EKeys::V;
		break;
	case EQuestKeyBindAction::ToggleSelectedConnectionsTypes: LocalInput.Key = EKeys::Z;
		break;
	case EQuestKeyBindAction::MutuallyDisableNodes: LocalInput.Key = EKeys::M;
		break;
	case EQuestKeyBindAction::RemoveSelectedNodesConnections: LocalInput.Key = EKeys::X;
		break;
	case EQuestKeyBindAction::RemoveSelectedNodesDisableConnections: LocalInput.Key = EKeys::N;
		break;
	default:
		break;
	}

	return LocalInput;
}

TArray<FQuestEditorKeyBind> UQuestEditorDevFunctionLibrary::GetDefaultQuestEditorKeybinds()
{
	//Loop through all default keybinds and make their structure
	EQuestKeyBindAction LocaKeyBindEnum = EQuestKeyBindAction::RemoveSelectedNodesDisableConnections; //assume this is the last one

	TArray<FQuestEditorKeyBind> KeybindsOut = TArray<FQuestEditorKeyBind>();
	FQuestEditorKeyBind KeybindLocal = FQuestEditorKeyBind();

	uint8 bytes = (uint8)LocaKeyBindEnum;
	for (int i = 0; i <= bytes; i++)
	{
		//Convert int to enum
		KeybindLocal.Action = static_cast<EQuestKeyBindAction>(i);
		//Get input for action
		KeybindLocal.Input = GetDefaultKeybindForAction(KeybindLocal.Action);
		//Add to array
		KeybindsOut.Add(KeybindLocal);
	}

	return KeybindsOut;
}

bool UQuestEditorDevFunctionLibrary::GetActionForKey(FInputChord InputIn, EQuestKeyBindAction& ActionOut)
{
	bool found = false;
	EQuestKeyBindAction LocalAction = EQuestKeyBindAction::MoveUp;

	//Find the overrides and append to default. The first i find breaks
	TArray<FQuestEditorKeyBind> Kbs = QuestEditorDev::Get().GetSettings()->QuestEditorShortcuts;

	//Search untill find
	for (int i = 0; i < Kbs.Num(); i++)
	{
		//Is this the input?
		if (Kbs[i].Input == InputIn)
		{
			LocalAction = Kbs[i].Action;
			found = true;
			break;
		}
	}

	//If cannot find in the overrides, search in the defaults. I do in separated array to avoid getting defaults if i already found it.
	if (!found)
	{
		//Search untill find
		Kbs = GetDefaultQuestEditorKeybinds();
		for (int i = 0; i < Kbs.Num(); i++)
		{
			//Is this the input?
			if (Kbs[i].Input == InputIn)
			{
				LocalAction = Kbs[i].Action;
				found = true;
				break;
			}
		}
	}
	ActionOut = LocalAction;
	return found;
}

bool UQuestEditorDevFunctionLibrary::CanPerformKBAction(EQuestKeyBindAction ActionIn, bool IsHierarchyIn)
{
	if (IsHierarchyIn)
	{
		//Only this are allowed in hierarchy.
		return (ActionIn == EQuestKeyBindAction::ZoomIn || ActionIn == EQuestKeyBindAction::ZoomOut || ActionIn == EQuestKeyBindAction::IterateEndNodes || ActionIn == EQuestKeyBindAction::ResetZoom
			|| ActionIn == EQuestKeyBindAction::IterateNodesBackward || ActionIn == EQuestKeyBindAction::IterateNodesForward || ActionIn == EQuestKeyBindAction::IterateStartNodes
			|| ActionIn == EQuestKeyBindAction::CenterViewport || ActionIn == EQuestKeyBindAction::FindSelectedNode);
	}
	
	//If not hierarchy, all are allowed
	return true;
}

TArray<FString> UQuestEditorDevFunctionLibrary::GetNPCsThatCanStartQuest(const FQuestRep& QuestRepIn)
{
	FQuest QuestIn = UQuestsFunctionLibrary::QuestRepToQuest(QuestRepIn);

	TArray<FString> NPCNameSOut = TArray<FString>();

	FNPCQuestDef NPCDefLocal = FNPCQuestDef();
	
	FQuestTaskID LocalTaskID = FQuestTaskID();
	LocalTaskID.TaskType = EQuestTaskType::TalkTask;

	bool condition = false;
	//Search all talk tasks
	for (int i = 0; i < QuestIn.TalkTasks.Num(); i++)
	{
		LocalTaskID.TaskID = i;
		//Is a talk start node?
		condition = UQuestsFunctionLibrary::IsNodeStartNode(QuestIn, LocalTaskID);
		if (condition)
		{
			//Add NPC name if is unique
			NPCDefLocal = UQuestsFunctionLibrary::NPCQuestDefRepToNPCQuestDef(QuestIn.TalkTasks[i].TalkNPC);
			NPCNameSOut.AddUnique(NPCDefLocal.NPCName);
		}
	}

	return NPCNameSOut;
}

UBaseQuestEditor* UQuestEditorDevFunctionLibrary::FindQuestEditorReference(UObject* WorldContextObject)
{
	UQuestEditorSubsystem* QESubsystem = GEditor->GetEditorSubsystem<UQuestEditorSubsystem>();
	return QESubsystem->GetQuestEditorReference();

	//TArray <UUserWidget*> FoundWidgetsOutput;

	//UWidgetBlueprintLibrary::GetAllWidgetsOfClass(WorldContextObject, FoundWidgetsOutput, UBaseQuestEditor::StaticClass(), false);

	//UBaseQuestEditor* EditorRef = nullptr;
	//	
	////Find the editor that is valid now, some might be pending kill from previous openend tabs
	//for (int i = FoundWidgetsOutput.Num()-1; i >= 0; i--)
	//{
	//	if (IsValid(FoundWidgetsOutput[i]))
	//	{
	//		EditorRef = Cast <UBaseQuestEditor>(FoundWidgetsOutput[i]);			
	//		break;
	//	}
	//}	

	//return EditorRef;
}

TArray<UDataTable*> UQuestEditorDevFunctionLibrary::FindAllDatatablesOfStruct(const UScriptStruct* DTStructIn, FString FolderPath)
{
	TArray<UDataTable*> AllDTS;

	// Load the asset registry module
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked< FAssetRegistryModule >(FName("AssetRegistry"));
	IAssetRegistry& AssetRegistry = AssetRegistryModule.Get();
	// The asset registry is populated asynchronously at startup, so there's no guarantee it has finished.
	// This simple approach just runs a synchronous scan on the entire content directory.
	// Better solutions would be to specify only the path to where the relevant blueprints are,
	// or to register a callback with the asset registry to be notified of when it's finished populating.


	TArray <FString> ContentPaths;
	ContentPaths.Add(FolderPath);
	AssetRegistry.ScanPathsSynchronous(ContentPaths);

	FTopLevelAssetPath BaseClassName = UDataTable::StaticClass()->GetClassPathName();	
	TSet <FTopLevelAssetPath> DerivedNames;
	{
		TArray<FTopLevelAssetPath> BaseNames;
		BaseNames.Add(BaseClassName);

		TSet<FTopLevelAssetPath> Excluded;
		AssetRegistry.GetDerivedClassNames(BaseNames, Excluded, DerivedNames);	
	}

	FARFilter Filter;

	Filter.bRecursiveClasses = true;
	if (!FolderPath.IsEmpty())
	{
		Filter.PackagePaths.Add(*FolderPath);
	}
	Filter.bRecursivePaths = true;

	TArray< FAssetData > AssetList;
	AssetRegistry.GetAssets(Filter, AssetList);

	//Iterate over retrieved blueprint assets
	for (auto const& Asset : AssetList)
	{
		//Path
		FString StringLocal = Asset.ToSoftObjectPath().ToString();
		//Get DT
		UDataTable* DTLocal = UQuestsFunctionLibrary::GetDTFromStringPath(StringLocal);
		//Is valid DT?
		if (DTLocal)
		{
			//Need to check if is requested structure
			if (IsDTofStructType(DTLocal, DTStructIn))
			{
				//Add to array
				AllDTS.Add(UQuestsFunctionLibrary::GetDTFromStringPath(StringLocal));
			}			
		}
	}
	return AllDTS;
}


bool UQuestEditorDevFunctionLibrary::GetTaskQuestEditorInfo(TSoftClassPtr<UBaseQuestTask> TaskClassIn, TArray <UDataTable*> TaskInfoDTsIn, int& SlotTaskOut, int& SlotDTOut, FQuestTaskEditorInfo& TaskInfoOut)
{
	int TaskSlot = -1;
	int DTSlot = -1;
	FQuestTaskEditorInfo InfoOUT = FQuestTaskEditorInfo();
	bool succes = false;
	if (TaskClassIn.IsNull() || TaskInfoDTsIn.Num() <= 0)
	{
		//return fail
		SlotTaskOut = TaskSlot;
		SlotDTOut = DTSlot;
		TaskInfoOut = InfoOUT;
		UE_LOG(QuestEditorDevLog, Log, TEXT("UQuestEditorDevFunctionLibrary::GetTaskQuestEditorInfo - Task Is Null and cannot get the task information."));
		return false;
	}

	//-------------------------------------------------Normal cases (from datatable)
	TArray <FName> DTNames = TArray <FName>();

	for (int i = 0; i < TaskInfoDTsIn.Num(); i++)
	{
		DTNames = TaskInfoDTsIn[i]->GetRowNames();

		for (int j = 0; j < DTNames.Num(); j++)
		{

			FQuestTaskEditorInfo* InfoPointer = TaskInfoDTsIn[i]->FindRow<FQuestTaskEditorInfo>(DTNames[j], FString(), false);

			if (InfoPointer)
			{
				InfoOUT = *InfoPointer;
				//same class. check by asset name
				if (InfoOUT.TaskClass == TaskClassIn)
				{
					TaskSlot = j;
					DTSlot = i;
					succes = true;
					break;
				}
			}			
		}
		//if found a task already break first loop
		if (succes == true) break;
	}
	//Update vars out
	SlotTaskOut = TaskSlot;
	SlotDTOut = DTSlot;
	TaskInfoOut = InfoOUT;
	return succes;
}

bool UQuestEditorDevFunctionLibrary::IsDTofStructType(UDataTable* DT, const UScriptStruct* DTStructIn)
{
	return (DT->GetRowStruct() == DTStructIn);
}

void UQuestEditorDevFunctionLibrary::PrintQuestEditorDevLog(const FString& StringIn)
{
	UE_LOG(QuestEditorDevLog, Log, TEXT("%s."), *StringIn);
	return;
}

FQuestRep UQuestEditorDevFunctionLibrary::FindQuestInDatatables(const FString& QuestNameIN, TArray<UDataTable*> DTArrayIn)
{
	if (QuestNameIN == FString()) return FQuestRep();

	TArray <FName, TInlineAllocator<10>> RowNames;
	FQuest* QuestPointer = nullptr;
	FQuest QuestLocal = FQuest();
	FQuestRep QuestRepOut = FQuestRep();
	bool ShouldBreak = false;

	//Iterate through DT
	for (int i = 0; i < DTArrayIn.Num(); i++)
	{
		//Get row names
		RowNames = DTArrayIn[i]->GetRowNames();

		//Iterate through DT names
		for (int j = 0; j < RowNames.Num(); j++)
		{
			QuestPointer = DTArrayIn[i]->FindRow<FQuest>(RowNames[j], FString(), false);
			if (QuestPointer != nullptr)
			{
				QuestLocal = *QuestPointer;
				//Is the same name? If you have 2 quests with same name this may not work properly
				if (QuestLocal.QuestName.ToString() == QuestNameIN)
				{
					//Create the rep variable
					QuestRepOut.QuestDTName = RowNames[j];
					QuestRepOut.Datatable = DTArrayIn[i]->GetPathName();
					ShouldBreak = true;
					break;
				}
			}
		}
		if (ShouldBreak == true) break;
	}

	return QuestRepOut;
}

FDialogRep UQuestEditorDevFunctionLibrary::FindDialogInDatatables(const FString& DialogNameIN, TArray<UDataTable*> DTArrayIn)
{
	TArray <FName, TInlineAllocator<10>> RowNames;
	FDialog* DialogPointer = nullptr;
	FDialog DialogLocal = FDialog();
	FDialogRep DialogRepOut = FDialogRep();
	bool ShouldBreak = false;

	//Iterate through DT
	for (int i = 0; i < DTArrayIn.Num(); i++)
	{
		//Get row names
		RowNames = DTArrayIn[i]->GetRowNames();

		//Iterate through DT names
		for (int j = 0; j < RowNames.Num(); j++)
		{
			DialogPointer = DTArrayIn[i]->FindRow<FDialog>(RowNames[j], FString(), false);
			if (DialogPointer != nullptr)
			{
				DialogLocal = *DialogPointer;
				//Is the same name? If you have 2 quests with same name this may not work properly
				if (DialogLocal.DialogEditorName == DialogNameIN)
				{
					//Create the rep variable
					DialogRepOut.DialogsDTName = RowNames[j];
					DialogRepOut.Datatable = DTArrayIn[i]->GetPathName();
					ShouldBreak = true;
					break;
				}
			}
		}
		if (ShouldBreak == true) break;
	}

	return DialogRepOut;
}

FNPCQuestDefRep UQuestEditorDevFunctionLibrary::FindNPCInDatatables(const FString& NPCNameIN, TArray<UDataTable*> DTArrayIn)
{
	TArray <FName, TInlineAllocator<10>> RowNames;
	FNPCQuestDef* NPCPointer = nullptr;
	FNPCQuestDef NPCLocal = FNPCQuestDef();
	FNPCQuestDefRep NPCRepOut = FNPCQuestDefRep();
	bool ShouldBreak = false;

	//Iterate through DT
	for (int i = 0; i < DTArrayIn.Num(); i++)
	{
		//Get row names
		RowNames = DTArrayIn[i]->GetRowNames();

		//Iterate through DT names
		for (int j = 0; j < RowNames.Num(); j++)
		{
			NPCPointer = DTArrayIn[i]->FindRow<FNPCQuestDef>(RowNames[j], FString(), false);
			if (NPCPointer != nullptr)
			{
				NPCLocal = *NPCPointer;
				//Is the same name? If you have 2 quests with same name this may not work properly
				if (NPCLocal.NPCName == NPCNameIN)
				{
					//Create the rep variable
					NPCRepOut.NPCDefDTName = RowNames[j];
					NPCRepOut.Datatable = DTArrayIn[i]->GetPathName();
					ShouldBreak = true;
					break;
				}
			}
		}
		if (ShouldBreak == true) break;
	}

	return NPCRepOut;
}

bool UQuestEditorDevFunctionLibrary::IsQuestInDatatables(const FString& QuestNameIn, TArray<UDataTable*> DTArrayIn)
{
	TArray <FName, TInlineAllocator<10>> RowNames;
	FQuest* QuestPointer = nullptr;
	FQuest QuestLocal = FQuest();

	bool FoundQuest = false;

	//Iterate through DT
	for (int i = 0; i < DTArrayIn.Num(); i++)
	{
		//Get row names
		RowNames = DTArrayIn[i]->GetRowNames();

		//Iterate through DT names
		for (int j = 0; j < RowNames.Num(); j++)
		{
			QuestPointer = DTArrayIn[i]->FindRow<FQuest>(RowNames[j], FString(), false);
			if (QuestPointer != nullptr)
			{
				QuestLocal = *QuestPointer;
				//Is the same name? If you have 2 quests with same name this may not work properly
				if (QuestLocal.QuestName.ToString() == QuestNameIn)
				{					
					FoundQuest = true;
					break;
				}
			}
		}
		if (FoundQuest == true) break;
	}

	return FoundQuest;
}

bool UQuestEditorDevFunctionLibrary::IsWidgetFullyVisible(UWidget* WidgetIn)
{
	bool resultVisibility = true;

	UWidget* hierarchyWidget = WidgetIn;
	while (hierarchyWidget)
	{
		// WARNING: don't use outerWidget->IsVisible() since that uses inner slate widget which might not yet be initialized here!
		// GetVisibility uses the UMG defined visibility which is exactly what we want and need here.
		bool isCurrentVisible = hierarchyWidget->GetVisibility() == ESlateVisibility::Visible ||
			hierarchyWidget->GetVisibility() == ESlateVisibility::HitTestInvisible ||
			hierarchyWidget->GetVisibility() == ESlateVisibility::SelfHitTestInvisible;

		resultVisibility = resultVisibility && isCurrentVisible;

		if (!resultVisibility)
		{
			break;
		}

		UWidget* nextHierarchyWidget = hierarchyWidget->GetParent();

		// if outerWidget is nullptr it means we reached the root container of the current UserWidget...
		if (!nextHierarchyWidget)
		{
			//...Get the WidgetTree...
			if (UWidgetTree* currentWidgetTree = Cast<UWidgetTree>(hierarchyWidget->GetOuter()))
			{
				//...and the corresponding parent UserWidget...
				if (UWidget* currentUserWidget = Cast<UWidget>(currentWidgetTree->GetOuter()))
				{
					//...and take it as UWidget to consider in next loop
					nextHierarchyWidget = currentUserWidget;
				}
			}
		}

		hierarchyWidget = nextHierarchyWidget;
	}

	return resultVisibility;
}

FString UQuestEditorDevFunctionLibrary::GetQuestNodeVisualString(FQuestNode QuestNodeIn, bool UseNPCDialog)
{
	FQuest QuestLocal = UQuestsFunctionLibrary::QuestRepToQuest(QuestNodeIn.Quest);
	
	return GetTaskIDVisualString(QuestLocal, QuestNodeIn.Node, UseNPCDialog);
}

FString UQuestEditorDevFunctionLibrary::GetTaskIDVisualString(const FQuest& QuestIn, const FQuestTaskID& TaskIDIn, bool UseNPCDialog)
{
	FString StringOut = FString();
	FString StringAux = FString();

	//------------------------Quest name first string
	//String name for quest goes first and for both
	//StringOut = QuestLocal.QuestName;
	//Gets first 25 chars only of quest name
	//if (StringOut.Len() > 25)
	//{
	//	StringOut.RemoveAt(25, 1000);
	//	//Append to show cutting
	//	StringOut.Append("...");
	//}

	//------------------------Task Name second string
	switch (TaskIDIn.TaskType)
	{
	case EQuestTaskType::Task:
	{
		//Task name the name in editor. This avoids going for the info
		if (QuestIn.Tasks.IsValidIndex(TaskIDIn.TaskID))
		{
			StringAux = QuestIn.Tasks[TaskIDIn.TaskID].TaskClass.GetAssetName();			
			StringAux = TruncateStringToLen(StringAux, 25);
			StringOut = "Task " + FString::FromInt(TaskIDIn.TaskID) + " - " + StringAux;
		}
	}
	break;
	case EQuestTaskType::TalkTask:
	{
		//Task name is what player says for talk tasks
		if (QuestIn.TalkTasks.IsValidIndex(TaskIDIn.TaskID))
		{
			if (UseNPCDialog)
			{
				StringAux = QuestIn.TalkTasks[TaskIDIn.TaskID].NPCResponse.ToString();
			}
			else
			{
				StringAux = QuestIn.TalkTasks[TaskIDIn.TaskID].PlayerDialog.ToString();
			}
			
			StringAux = TruncateStringToLen(StringAux, 25);
			StringOut = "Talk Task " + FString::FromInt(TaskIDIn.TaskID) + " - " + StringAux;
		}
	}
	break;
	default:
		break;
	}

	return StringOut;
}

void UQuestEditorDevFunctionLibrary::GetQuestNodeSeparatedStrings(FQuestNode QuestNodeIn, bool UseNPCDialog, FString& QuestStringOut, FString& TaskStringOut, FString& TaskTypeStringOut)
{
	//Find quest of node
	FQuest QuestLocal = UQuestsFunctionLibrary::QuestRepToQuest(QuestNodeIn.Quest);

	//Find strings
	GetTaskIDSeparatedStrings(QuestLocal, QuestNodeIn.Node, UseNPCDialog, QuestStringOut, TaskStringOut, TaskTypeStringOut);
	
	return;
}

void UQuestEditorDevFunctionLibrary::GetTaskIDSeparatedStrings(const FQuest& QuestIn, const FQuestTaskID& TaskIDIn, bool UseNPCDialog, FString& QuestStringOut, FString& TaskStringOut, FString& TaskTypeStringOut)
{
	FString StringOut = FString();
	FString StringAux = FString();
	FString StringAux2 = FString();
	//------------------------Quest name first string
	//String name for quest goes first and for both
	StringOut = QuestIn.QuestName.ToString();

	StringOut = TruncateStringToLen(StringOut, 25);

	//------------------------Task Name second string
	switch (TaskIDIn.TaskType)
	{
	case EQuestTaskType::Task:
	{
		//Task name the name in editor. This avoids going for the info
		if (QuestIn.Tasks.IsValidIndex(TaskIDIn.TaskID))
		{
			StringAux = QuestIn.Tasks[TaskIDIn.TaskID].TaskClass.GetAssetName();
			StringAux = TruncateStringToLen(StringAux, 25);
			StringAux2 = "Task " + FString::FromInt(TaskIDIn.TaskID);
		}
	}
	break;
	case EQuestTaskType::TalkTask:
	{
		//Task name is what player says for talk tasks
		if (QuestIn.TalkTasks.IsValidIndex(TaskIDIn.TaskID))
		{
			if (UseNPCDialog)
			{
				StringAux = QuestIn.TalkTasks[TaskIDIn.TaskID].NPCResponse.ToString();
			}
			else
			{
				StringAux = QuestIn.TalkTasks[TaskIDIn.TaskID].PlayerDialog.ToString();
			}		
			StringAux = TruncateStringToLen(StringAux, 25);
			StringAux2 = "Talk Task " + FString::FromInt(TaskIDIn.TaskID);
		}
	}
	break;
	default:
		break;
	}

	//Out variables
	QuestStringOut = StringOut;
	TaskStringOut = StringAux;
	TaskTypeStringOut = StringAux2;

	return;
}

FString UQuestEditorDevFunctionLibrary::TruncateStringToLen(FString StringIn, int MaxLen)
{
	FString StringAux = FString();
	StringAux = StringIn;
	if (StringIn.Len() > MaxLen)
	{
		StringAux = UKismetStringLibrary::GetSubstring(StringAux, 0, MaxLen);
		//Append to show cutting
		StringAux.Append("...");
	}

	return StringAux;
}

void UQuestEditorDevFunctionLibrary::SeparateTasksByType(const TArray<FQuestTaskID>& AllTasksIn, TArray<FQuestTaskID>& TasksOut, TArray<FQuestTaskID>& TalkTasksOut)
{
	TArray<FQuestTaskID, TInlineAllocator<5>> TasksLocal;
	TArray<FQuestTaskID, TInlineAllocator<5>> TalkTasksLocal;

	for (int i = 0; i < AllTasksIn.Num(); i++)
	{
		if (AllTasksIn[i].TaskType == EQuestTaskType::Task)
		{
			TasksLocal.Add(AllTasksIn[i]);
		}
		else
		{
			TalkTasksLocal.Add(AllTasksIn[i]);
		}
	}

	TasksOut = TasksLocal;
	TalkTasksOut = TalkTasksLocal;
}

ENodeSelectionType UQuestEditorDevFunctionLibrary::GetSelectionType(const TArray <FQuestTaskID>& AllTasksIn, const TArray <FQuestTaskID>& AllTalkTasksIn)
{
	//Nothing selected
	if (AllTasksIn.Num() == 0 && AllTalkTasksIn.Num() == 0) return ENodeSelectionType::NoSelection;
		
	//More than 1 talk task
	if (AllTasksIn.Num() == 0 && AllTalkTasksIn.Num() >1 ) return ENodeSelectionType::MultiTalkTask;

	//1 talk task
	if (AllTasksIn.Num() == 0 && AllTalkTasksIn.Num() == 1) return ENodeSelectionType::TalkTask;

	//More than 1  task
	if (AllTalkTasksIn.Num() == 0 && AllTasksIn.Num() > 1) return ENodeSelectionType::MultiTask;

	//1  task
	if (AllTalkTasksIn.Num() == 0 && AllTasksIn.Num() == 1) return ENodeSelectionType::Task;

	//Multinode
	if (AllTalkTasksIn.Num() > 0 && AllTasksIn.Num() > 0) return ENodeSelectionType::MultiNode;
	
	//Should not get here
	return ENodeSelectionType::NoSelection;
}

FQuestTask UQuestEditorDevFunctionLibrary::GetRepresentingTask(const TArray<FQuestTask>& AllTasksIn)
{
	//One talk task only?
	if (AllTasksIn.Num() == 1) return AllTasksIn[0];

	//Null array?
	if (AllTasksIn.Num() == 0) return FQuestTask();
	   
	//The first task is the one used as base
	FQuestTask RepresentingTask = AllTasksIn[0];

	bool MultipleTasks = false;

	bool MultiplePayloadAmount = false;
	bool MultiplePayloadClass = false;
	bool MultiplePayloadIgnoredTags = false;
	bool MultiplePayloadRequiredTags = false;
	bool MultiplePayloadStringA = false;
	bool MultiplePayloadStringB = false;
	bool MultiplePayloadTimesToCheck = false;
	bool MultiplePayloadVector = false;

	bool MultipleTaskShortDesc = false;
	bool MultipleInverseCondition = false;

	int CustomPayloadNum = 0;

	//Loop starts in 1, avoids first task
	for (int i = 1; i < AllTasksIn.Num(); i++)
	{
		//Task
		if (MultipleTasks == false)
		{
			//Is diferent in at least one?			
			if (RepresentingTask.TaskClass != AllTasksIn[i].TaskClass)
			{
				//---------------------------------------------Diferent tasks
				//Set a default one to represent
				//If tasks are multiple, i dont care about other variables.
				MultipleTasks = true;
				//Break immediately
				RepresentingTask = FQuestTask();
				break;
			}
			else
			{
				//---------------------------------------------Equal tasks
				//Check custom payload
				//This only needed for when tasks are equal. If not cannot do anything
				
				//Get the higher num of the paylods involved
				if (RepresentingTask.CustomPayloadData.Num() < AllTasksIn[i].CustomPayloadData.Num()) CustomPayloadNum = AllTasksIn[i].CustomPayloadData.Num();
				else CustomPayloadNum = RepresentingTask.CustomPayloadData.Num();
								
				//Check if all are equal, and if not, set to null
				//iterate true max 
				for (int j = 0; j < CustomPayloadNum; j++)
				{
					//Is is not valid slot in the current payload must add one
					if (!RepresentingTask.CustomPayloadData.IsValidIndex(j))
					{
						//Adds null string. Cuz i am starting for loop at 0, this should be fine as a resize
						RepresentingTask.CustomPayloadData.Add(FString());
					}
					//If is not valid slot in the looping payload must set to null
					else if (!AllTasksIn[i].CustomPayloadData.IsValidIndex(j))
					{
						RepresentingTask.CustomPayloadData[j] = FString();
					}
					//If both are valid, check if are eqaul
					else if (RepresentingTask.CustomPayloadData[j] != AllTasksIn[i].CustomPayloadData[j])
					{
						//Null string for this payload data
						RepresentingTask.CustomPayloadData[j] = FString();
					}
				}
			}
		}

		//Payload amount
		if (MultiplePayloadAmount == false)
		{
			//Is diferent in at least one?
			if (RepresentingTask.Payload.AmountInvolved != AllTasksIn[i].Payload.AmountInvolved)
			{
				//Set a default one to represent
				MultiplePayloadAmount = true;
				RepresentingTask.Payload.AmountInvolved = 0;
			}
		}

		//Payload class
		if (MultiplePayloadClass == false)
		{
			//Is diferent in at least one?
			if (RepresentingTask.Payload.ClassInvolved != AllTasksIn[i].Payload.ClassInvolved)
			{
				//Set a default one to represent
				MultiplePayloadClass = true;
				RepresentingTask.Payload.ClassInvolved = nullptr;
			}
		}

		//Ignored tags
		if (MultiplePayloadIgnoredTags == false)
		{
			//Is diferent in at least one?
			if (RepresentingTask.Payload.IgnoredTags != AllTasksIn[i].Payload.IgnoredTags)
			{
				//Set a default one to represent
				MultiplePayloadIgnoredTags = true;
				RepresentingTask.Payload.IgnoredTags.Reset();
			}
		}

		//Req tags
		if (MultiplePayloadRequiredTags == false)
		{
			//Is diferent in at least one?
			if (RepresentingTask.Payload.RequiredTags != AllTasksIn[i].Payload.RequiredTags)
			{
				//Set a default one to represent
				MultiplePayloadRequiredTags = true;
				RepresentingTask.Payload.RequiredTags.Reset();
			}
		}

		//String A
		if (MultiplePayloadStringA == false)
		{
			//Is diferent in at least one?
			if (RepresentingTask.Payload.StringA != AllTasksIn[i].Payload.StringA)
			{
				//Set a default one to represent
				MultiplePayloadStringA = true;
				RepresentingTask.Payload.StringA = FString();
			}
		}

		//String B
		if (MultiplePayloadStringB == false)
		{
			//Is diferent in at least one?
			if (RepresentingTask.Payload.StringB != AllTasksIn[i].Payload.StringB)
			{
				//Set a default one to represent
				MultiplePayloadStringB = true;
				RepresentingTask.Payload.StringB = FString();
			}
		}

		//Times To check
		if (MultiplePayloadTimesToCheck == false)
		{
			//Is diferent in at least one?
			if (RepresentingTask.Payload.TimesToCheckTask != AllTasksIn[i].Payload.TimesToCheckTask)
			{
				//Set a default one to represent
				MultiplePayloadTimesToCheck = true;
				RepresentingTask.Payload.TimesToCheckTask = 0;
			}
		}

		//Vector
		if (MultiplePayloadVector == false)
		{
			//Is diferent in at least one?
			if (RepresentingTask.Payload.VectorInvolved != AllTasksIn[i].Payload.VectorInvolved)
			{
				//Set a default one to represent
				MultiplePayloadVector = true;
				RepresentingTask.Payload.VectorInvolved = FVector::ZeroVector;
			}
		}

		//ShortDesc
		if (MultipleTaskShortDesc == false)
		{
			//Is diferent in at least one?
			if (RepresentingTask.TaskShortDesc.ToString() != AllTasksIn[i].TaskShortDesc.ToString())
			{
				//Set a default one to represent
				MultipleTaskShortDesc = true;
				RepresentingTask.TaskShortDesc = FText();
			}
		}

		//ShortDesc
		if (MultipleInverseCondition == false)
		{
			//Is diferent in at least one?
			if (RepresentingTask.InverseActivateCondition != AllTasksIn[i].InverseActivateCondition)
			{
				//Set a default one to represent
				MultipleInverseCondition = true;
				RepresentingTask.InverseActivateCondition = false;
			}
		}		
	}

	return RepresentingTask;
}

FQuestTalkTask UQuestEditorDevFunctionLibrary::GetRepresentingTalkTask(const TArray<FQuestTalkTask>& AllTalkTasksIn)
{
	//One talk task only?
	if (AllTalkTasksIn.Num() == 1) return AllTalkTasksIn[0];

	//Null array?
	if (AllTalkTasksIn.Num() == 0) return FQuestTalkTask();
		
	//The first task is the one used as base
	FQuestTalkTask RepresentingTask = AllTalkTasksIn[0];
	
	bool MultipleNpcDef = false;
	bool MultiplePlayerSay = false;
	bool MultipleNpcResponse = false;
	bool MultipleNpcCue = false;
	bool MultiplePlayerCue = false;
	bool MultipleTaskShortDesc = false;

	//Loop starts in 1, avoids first task
	for (int i = 1; i < AllTalkTasksIn.Num(); i++)
	{
		//NpcDef 
		if (MultipleNpcDef == false)
		{
			//Is diferent in at least one?
			if (RepresentingTask.TalkNPC != AllTalkTasksIn[i].TalkNPC)
			{
				//Set a default one to represent
				MultipleNpcDef = true;
				RepresentingTask.TalkNPC = FNPCQuestDefRep();
			}
		}

		//PlayerDialog
		if (MultiplePlayerSay == false)
		{
			//Is diferent in at least one?
			if (RepresentingTask.PlayerDialog.ToString() != AllTalkTasksIn[i].PlayerDialog.ToString())
			{
				//Set a default one to represent
				MultiplePlayerSay = true;
				RepresentingTask.PlayerDialog = FText();
			}
		}
		
		//NPCResponse
		if (MultipleNpcResponse == false)
		{
			//Is diferent in at least one?
			if (RepresentingTask.NPCResponse.ToString() != AllTalkTasksIn[i].NPCResponse.ToString())
			{
				//Set a default one to represent
				MultipleNpcResponse = true;
				RepresentingTask.NPCResponse = FText();
			}
		}

		//NPCCue
		if (MultipleNpcCue == false)
		{
			//Is diferent in at least one?
			if (RepresentingTask.NPCSoundCue != AllTalkTasksIn[i].NPCSoundCue)
			{
				//Set a default one to represent
				MultipleNpcCue = true;
				RepresentingTask.NPCSoundCue = nullptr;
			}
		}

		//PlayerCue
		if (MultiplePlayerCue == false)
		{
			//Is diferent in at least one?
			if (RepresentingTask.PlayerSoundCue != AllTalkTasksIn[i].PlayerSoundCue)
			{
				//Set a default one to represent
				MultiplePlayerCue = true;
				RepresentingTask.PlayerSoundCue = nullptr;
			}
		}

		//Short Desc
		if (MultipleTaskShortDesc == false)
		{
			//Is diferent in at least one?
			if (RepresentingTask.TaskShortDesc.ToString() != AllTalkTasksIn[i].TaskShortDesc.ToString())
			{
				//Set a default one to represent
				MultipleTaskShortDesc = true;
				RepresentingTask.TaskShortDesc = FText();
			}
		}

		//If already found all multiple, just stops.
		if (!MultipleNpcDef && !MultiplePlayerSay && !MultipleNpcResponse && !MultipleNpcCue && !MultiplePlayerCue && !MultipleTaskShortDesc) break;
	}

	return RepresentingTask;
}

TArray <FQuestTalkTask> UQuestEditorDevFunctionLibrary::GetTalkTasksFromIDArray(const FQuest& QuestIn, const TArray<FQuestTaskID>& TalkTasksIn)
{
	TArray <FQuestTalkTask> TasksOut;

	for (int i = 0; i < TalkTasksIn.Num(); i++)
	{
		//Is a talk task (should bem check anyways)
		if (TalkTasksIn[i].TaskType == EQuestTaskType::TalkTask)
		{
			if (QuestIn.TalkTasks.IsValidIndex(TalkTasksIn[i].TaskID))
			{
				//Add task to out array
				TasksOut.Add(QuestIn.TalkTasks[TalkTasksIn[i].TaskID]);
			}
		}		
	}	
	
	return TasksOut;
}

TArray<FQuestTask> UQuestEditorDevFunctionLibrary::GetTasksFromIDArray(const FQuest& QuestIn, const TArray<FQuestTaskID>& TasksIn)
{
	TArray <FQuestTask> TasksOut;

	for (int i = 0; i < TasksIn.Num(); i++)
	{
		//Is a talk task (should bem check anyways)
		if (TasksIn[i].TaskType == EQuestTaskType::Task)
		{
			if (QuestIn.Tasks.IsValidIndex(TasksIn[i].TaskID))
			{
				//Add task to out array
				TasksOut.Add(QuestIn.Tasks[TasksIn[i].TaskID]);
			}
		}
	}

	return TasksOut;
}

bool UQuestEditorDevFunctionLibrary::ConvertDialogsToQuest(TSoftObjectPtr<UDataTable> DialogDatatableIn, TSoftObjectPtr<UDataTable> QuestDatatableIn)
{
	//Valid DTs
	UDataTable* DTDialog = UQuestsFunctionLibrary::GetDTFromPath(DialogDatatableIn);	
	if (!IsDTofStructType(DTDialog, FDialog::StaticStruct())) return false;
	
	UDataTable* DTQuest = UQuestsFunctionLibrary::GetDTFromPath(QuestDatatableIn);
	if (!IsDTofStructType(DTQuest, FQuest::StaticStruct())) return false;
	
	TArray<FName> NamesDialogs = DTDialog->GetRowNames();
	FDialog DialogLocal = FDialog();
	FQuest QuestLocal = FQuest();

	//Exist?
	for (int i = 0; i < NamesDialogs.Num(); i++)
	{
		//Get row
		FDialog* DialogPointer = DTDialog->FindRow<FDialog>(NamesDialogs[i], FString(), true);
		
		if (DialogPointer != nullptr)
		{
			DialogLocal = *DialogPointer;

			//Convert dialog to Quest
			QuestLocal = UQuestEditorFunctionLibrary::ConvertDialogToQuest(DialogLocal);

			//Clear Localization IDs
			QuestLocal.UniqueID = -1;

			//Add to Quest DT
			SaveQuestRowToDT(QuestLocal, QuestDatatableIn);
		}
	}
		
	return true;
}

bool UQuestEditorDevFunctionLibrary::SaveQuestRowToDT(const FQuest& QuestIn, TSoftObjectPtr <UDataTable> DatatableIn)
{
	//Get quest dt
	UDataTable* DT = UQuestsFunctionLibrary::GetDTFromPath(DatatableIn);

	if (!DT)
	{
		UE_LOG(QuestEditorDevLog, Error, TEXT("UQuestEditorDevFunctionLibrary::SaveQuestRowToDT - Could not find the datatable: %s. Save Quest cancelled"), *DatatableIn.ToString());
		return false;
	}

	UE_LOG(QuestEditorDevLog, Log, TEXT("UQuestEditorDevFunctionLibrary::SaveQuestRowToDT ------------- Start Save Operation for quest: %s, with Quest Editor Unique ID: %i -----------"),
		*QuestIn.QuestName.ToString(), QuestIn.UniqueID);

	FQuest QuestLocal;

	int QuestPositionInDT = -1;
	FName OldRowName = FName("Quest");
	TArray<FName> Names = DT->GetRowNames();

	//Exist?
	for (int i = 0; i < Names.Num(); i++)
	{
		FQuest* QuestPointer = DT->FindRow<FQuest>(Names[i], FString(), true);
		if (QuestPointer != nullptr)
		{
			QuestLocal = *QuestPointer;
			//Chek if its same quest by name
			if (QuestLocal.QuestName.ToString() == QuestIn.QuestName.ToString())
			{
				QuestPositionInDT = i;
				OldRowName = Names[i];
				break;
			}
		}		
	}

	bool ShouldResetID = false;
	FQuest FinalQuest = QuestIn;

	//delete old
	if (QuestPositionInDT != -1)
	{
		//Is the ID not set? Is a old quest that is not corrected in the DT.
		if (QuestLocal.UniqueID == -1)
		{
			ShouldResetID = true;
			UE_LOG(QuestEditorDevLog, Log, TEXT("UQuestEditorDevFunctionLibrary::SaveQuestRowToDT - Quest: %s is in the datatable with invalid Unique ID. Request new ID."), *FinalQuest.QuestName.ToString());
		}
		//Quest In DT has a ID that is set and does not match quest in editor? Preserve DT ID.
		else if ((QuestLocal.UniqueID != -1) && (QuestLocal.UniqueID != FinalQuest.UniqueID))
		{			
			UE_LOG(QuestEditorDevLog, Log, TEXT("UQuestEditorDevFunctionLibrary::SaveQuestRowToDT - Quest: %s in DT has a different valid ID (DT UniqueID: %i) than the Editor Quest (Editor UniqueID: %i). "), 
				*FinalQuest.QuestName.ToString(), QuestLocal.UniqueID, FinalQuest.UniqueID);
			FinalQuest.UniqueID = QuestLocal.UniqueID;
		}
		//The quest ID is not valid? request a new one.
		else if (!IsQuestUniqueIDValid(DatatableIn, FinalQuest))
		{
			ShouldResetID = true;
			UE_LOG(QuestEditorDevLog, Log, TEXT("UQuestEditorDevFunctionLibrary::SaveQuestRowToDT - Quest: %s is in the datatable with invalid Unique ID. Request new ID."), *FinalQuest.QuestName.ToString());
		}

		//Quest exists, delete old one
		DT->RemoveRow(OldRowName);

		UE_LOG(QuestEditorDevLog, Log, TEXT("UQuestEditorDevFunctionLibrary::SaveQuestRowToDT - Quest: %s is already in the datatable in position %i with row name %s. Removed previous version."), *FinalQuest.QuestName.ToString(), QuestPositionInDT, *OldRowName.ToString());
	}
	else
	{
		//If creating a new row, handle UniqueID
		ShouldResetID = true;
		UE_LOG(QuestEditorDevLog, Log, TEXT("UQuestEditorDevFunctionLibrary::SaveQuestRowToDT - Quest: %s is not in the datatable. Request new ID."), *FinalQuest.QuestName.ToString());
	}

	//Handle Quest ID
	if (ShouldResetID)
	{
		FinalQuest = HandleQuestUniqueID(FinalQuest, DT);		
	}
	else
	{
		UE_LOG(QuestEditorDevLog, Log, TEXT("UQuestEditorDevFunctionLibrary::SaveQuestRowToDT - Quest: %s,  Unique ID: %i does not need a new ID. Skipped"), *FinalQuest.QuestName.ToString(), FinalQuest.UniqueID);
	}

	//Handle tasks IDs
	if (AreQuestTasksMissingIDs(FinalQuest))
	{
		SetAllUniqueIDsInQuest(FinalQuest);
	}

	//Add row
	AddQuestRowToDT(FinalQuest, OldRowName, DatatableIn, false);

	return true;
}

bool UQuestEditorDevFunctionLibrary::SaveDialogRowToDT(const FQuest& QuestIn, TSoftObjectPtr <UDataTable> DatatableIn)
{
	//Get quest dt
	UDataTable* DT = UQuestsFunctionLibrary::GetDTFromPath(DatatableIn);

	if (!DT)
	{
		UE_LOG(QuestEditorDevLog, Error, TEXT("UQuestEditorDevFunctionLibrary::SaveDialogRowToDT - Could not find the datatable: %s. Save Dialog cancelled"), *DatatableIn.ToString());
		return false;
	}

	UE_LOG(QuestEditorDevLog, Log, TEXT("UQuestEditorDevFunctionLibrary::SaveDialogRowToDT ------------- Start Save Operation for Dialog: %s, with Quest Editor Unique ID: %i -----------"),
		*QuestIn.QuestName.ToString(), QuestIn.UniqueID);

	FDialog DialogLocal;

	int QuestPositionInDT = -1;
	FName OldRowName = FName("Dialog");
	TArray<FName> Names = DT->GetRowNames();

	//Exist?
	for (int i = 0; i < Names.Num(); i++)
	{
		FDialog* QuestPointer = DT->FindRow<FDialog>(Names[i], FString(), true);
		if (QuestPointer != nullptr)
		{
			DialogLocal = *QuestPointer;
			//Check if its same dialog
			if (DialogLocal.DialogEditorName == QuestIn.QuestName.ToString())
			{
				QuestPositionInDT = i;
				OldRowName = Names[i];
				break;
			}
		}
	}

	bool ShouldResetID = false;
	FQuest FinalQuest = QuestIn;

	//delete old
	if (QuestPositionInDT != -1)
	{
		//Is the ID not set? Is a old quest that is not corrected in the DT.
		if (DialogLocal.UniqueID == -1)
		{
			ShouldResetID = true;
			UE_LOG(QuestEditorDevLog, Log, TEXT("UQuestEditorDevFunctionLibrary::SaveDialogRowToDT - Dialog: %s is in the datatable with invalid Unique ID. Request new ID."), *FinalQuest.QuestName.ToString());
		}
		//DialogLocal In DT has a ID that is set and does not match quest in editor? Preserve DT ID.
		else if ((DialogLocal.UniqueID != -1) && (DialogLocal.UniqueID != FinalQuest.UniqueID))
		{
			UE_LOG(QuestEditorDevLog, Log, TEXT("UQuestEditorDevFunctionLibrary::SaveDialogRowToDT - Dialog: %s in DT has a different valid ID (DT UniqueID: %i) than the Editor Dialog (Editor UniqueID: %i). "),
				*FinalQuest.QuestName.ToString(), DialogLocal.UniqueID, FinalQuest.UniqueID);
			FinalQuest.UniqueID = DialogLocal.UniqueID;
		}
		//The quest ID is not valid? request a new one.
		else if (!IsDialogUniqueIDValid(DatatableIn, FinalQuest))
		{
			ShouldResetID = true;
			UE_LOG(QuestEditorDevLog, Log, TEXT("UQuestEditorDevFunctionLibrary::SaveQuestRowToDT - Dialog: %s is in the datatable with invalid Unique ID. Request new ID."), *FinalQuest.QuestName.ToString());
		}

		//Quest exists, delete old one
		DT->RemoveRow(OldRowName);

		UE_LOG(QuestEditorDevLog, Log, TEXT("UQuestEditorDevFunctionLibrary::SaveDialogRowToDT - Dialog: %s is already in the datatable in position %i with row name %s. Removed previous version."), *FinalQuest.QuestName.ToString(), QuestPositionInDT, *OldRowName.ToString());
	}
	else
	{
		//If creating a new row, handle UniqueID
		ShouldResetID = true;
		UE_LOG(QuestEditorDevLog, Log, TEXT("UQuestEditorDevFunctionLibrary::SaveDialogRowToDT - Dialog: %s is not in the datatable. Request new ID."), *FinalQuest.QuestName.ToString());
	}

	//Handle Unique ID
	if (ShouldResetID)
	{
		FinalQuest = HandleDialogUniqueID(FinalQuest, DT);
	}

	//Handle tasks IDs
	if (AreQuestTasksMissingIDs(FinalQuest))
	{
		SetAllUniqueIDsInQuest(FinalQuest);
	}

	//Add row
	AddDialogRowToDT(FinalQuest, OldRowName, DatatableIn, false);

	return true;
}

bool UQuestEditorDevFunctionLibrary::SaveNPCRowToDT(const FNPCQuestDef& NPCInfoIn, TSoftObjectPtr <UDataTable> DatatableIn)
{
	//Get quest dt
	UDataTable* DT = UQuestsFunctionLibrary::GetDTFromPath(DatatableIn);
	
	if (!DT)
	{
		UE_LOG(QuestEditorDevLog, Error, TEXT("UQuestEditorDevFunctionLibrary::SaveNPCRowToDT - Could not find the datatable: %s. Save NPC cancelled"), *DatatableIn.ToString());
		return false;
	}

	FNPCQuestDef NPCLocal;

	int QuestPositionInDT = -1;
	FName OldRowName = FName("NPC");
	TArray<FName> Names = DT->GetRowNames();

	//Exist?
	for (int i = 0; i < Names.Num(); i++)
	{
		FNPCQuestDef* NPCPointer = DT->FindRow<FNPCQuestDef>(Names[i], FString(), true);
		if (NPCPointer != nullptr)
		{
			NPCLocal = *NPCPointer;
			//Chek if same npc
			if (NPCLocal.NPCName == NPCInfoIn.NPCName)
			{
				QuestPositionInDT = i;
				OldRowName = Names[i];
				break;
			}
		}
	}

	//delete old
	if (QuestPositionInDT != -1)
	{
		DT->RemoveRow(OldRowName);
	}

	//Add row
	AddNPCInfoRowToDT(NPCInfoIn, OldRowName, false, DatatableIn);

	return true;
}

bool UQuestEditorDevFunctionLibrary::AddQuestToNPC(const FQuestRep& QuestRepIn, FName NPCNameIn, TSoftObjectPtr <UDataTable> DatatableIn)
{
	//Get quest dt
	UDataTable* DT = UQuestsFunctionLibrary::GetDTFromPath(DatatableIn);

	if (!DT)
	{
		UE_LOG(QuestEditorDevLog, Error, TEXT("UQuestEditorDevFunctionLibrary::AddQuestToNPC - Could not find the datatable: %s. Add quest cancelled"), *DatatableIn.ToString());
		return false;
	}

	//Names DT
	TArray<FName> Names = DT->GetRowNames();

	bool succes = false;
	//Is npc in this dt?
	if (Names.Contains(NPCNameIn))
	{
		//search npc struc
		FNPCQuestDef* NPCPointer = DT->FindRow<FNPCQuestDef>(NPCNameIn, FString(), true);
		FNPCQuestDef NPCLocal;
		if (NPCPointer != nullptr)
		{
			//NPC def
			NPCLocal = *NPCPointer;

			//If the quest is alredy added dont do anything
			if (!NPCLocal.Quests.Contains(QuestRepIn))
			{
				//Modify NPC
				NPCLocal.Quests.Add(QuestRepIn);

				//Remove NPC Old
				DT->RemoveRow(NPCNameIn);

				//Add row with modifications
				DT->AddRow(NPCNameIn, NPCLocal);

				//Mark dirty
				DT->MarkPackageDirty();
				UE_LOG(QuestEditorDevLog, Log, TEXT("UQuestEditorDevFunctionLibrary::AddQuestToNPC - Quest %s added to NPC %s in DTPath: %s."), *QuestRepIn.QuestDTName.ToString(), *NPCNameIn.ToString(), *DatatableIn.ToString());
				succes = true;
			}
		}
	}

	return succes;
}

bool UQuestEditorDevFunctionLibrary::AddDialogsToNPC(const FQuestRep& QuestRepIn, FName NPCNameIn, TSoftObjectPtr <UDataTable> DatatableIn)
{
	//Get quest dt
	UDataTable* DT = UQuestsFunctionLibrary::GetDTFromPath(DatatableIn);

	if (!DT)
	{
		UE_LOG(QuestEditorDevLog, Error, TEXT("UQuestEditorDevFunctionLibrary::AddDialogsToNPC - Could not find the datatable: %s. Add dialog cancelled"), *DatatableIn.ToString());
		return false;
	}

	//Names DT
	TArray<FName> Names = DT->GetRowNames();

	bool succes = false;
	//Is npc in this dt?
	if (Names.Contains(NPCNameIn))
	{
		//search npc struc
		FNPCQuestDef* NPCPointer = DT->FindRow<FNPCQuestDef>(NPCNameIn, FString(), true);
		FNPCQuestDef NPCLocal;
		if (NPCPointer != nullptr)
		{
			//NPC def
			NPCLocal = *NPCPointer;

			//If the quest is alredy added dont do anything
			if (!NPCLocal.Dialogs.Contains(QuestRepIn))
			{
				//Modify NPC
				NPCLocal.Dialogs.Add(QuestRepIn);

				//Remove NPC Old
				DT->RemoveRow(NPCNameIn);

				//Add row with modifications
				DT->AddRow(NPCNameIn, NPCLocal);

				//Mark dirty
				DT->MarkPackageDirty();
				UE_LOG(QuestEditorDevLog, Log, TEXT("UQuestEditorDevFunctionLibrary::AddDialogsToNPC - Dialog %s added to NPC %s in DTPath: %s."), *QuestRepIn.QuestDTName.ToString(), *NPCNameIn.ToString(), *DatatableIn.ToString());
				succes = true;
			}
		}
	}

	return succes;
}

bool UQuestEditorDevFunctionLibrary::AddNPCInfoRowToDT(const FNPCQuestDef& NPCInfoIn, FName InRowName, bool UseAutoRowName, TSoftObjectPtr <UDataTable> DatatableIn)
{
	//Get dt
	UDataTable* DT = UQuestsFunctionLibrary::GetDTFromPath(DatatableIn);

	if (!DT)
	{
		UE_LOG(QuestEditorDevLog, Error, TEXT("UQuestEditorDevFunctionLibrary::AddNPCInfoRowToDT - Could not find the datatable: %s. NPC save cancelled"), *DatatableIn.ToString());
		return false;
	}

	//DT Names
	const TArray<FName> ExisitngNames = DT->GetRowNames();
	FName NewName;

	bool ForceRandomName = false;

	if (InRowName == FName() && !UseAutoRowName) ForceRandomName = true;
	//If no UseAutoRowName
	else if (!UseAutoRowName)
	{
		//Search for a not taken name
		if (!ExisitngNames.Contains(InRowName)) NewName = InRowName;
		else ForceRandomName = true;
	}

	if (UseAutoRowName || ForceRandomName)
	{
		//Force name in datatable?
		if (QuestEditorDev::Get().GetSettings()->UseNPCNameAsRowName)
		{
			NewName = FName(NPCInfoIn.NPCName);
		}
		else
		{
			//Generate valid name
			//Add a new row to the datatable with a standar unused name
			NewName = DataTableUtils::MakeValidName(TEXT("Npc"));

			while (ExisitngNames.Contains(NewName))
			{
				NewName.SetNumber(NewName.GetNumber() + 1);
			}

			//Add a part of the NPC name to the row name.
			FString LocalName = NewName.ToString();
			FString NPCGameName = NPCInfoIn.NPCName;
			NPCGameName.RemoveSpacesInline();
			if (NPCGameName.Len() > 15)
			{
				NPCGameName = NPCGameName.Left(15);
			}
			LocalName = LocalName + "_" + NPCGameName;
			NewName = FName(LocalName);
		}		
	}

	DT->AddRow(NewName, NPCInfoIn);

	UE_LOG(QuestEditorDevLog, Log, TEXT("UQuestEditorDevFunctionLibrary::AddNPCInfoRowToDT -NPC %s added to DT in DTPath: %s with row name: %s"), *NPCInfoIn.NPCName, *DatatableIn.ToString(), *NewName.ToString());

	DT->MarkPackageDirty();

	return true;
}

bool UQuestEditorDevFunctionLibrary::AddQuestRowToDT(const FQuest& QuestIn, FName InRowName, TSoftObjectPtr <UDataTable> DatatableIn, bool UseAutoRowName)
{
	//Get dt
	UDataTable* DT = UQuestsFunctionLibrary::GetDTFromPath(DatatableIn);

	if (!DT)
	{
		UE_LOG(QuestEditorDevLog, Error, TEXT("UQuestEditorDevFunctionLibrary::AddQuestRowToDT - Could not find the datatable: %s. Quest save cancelled"), *DatatableIn.ToString());
		return false;
	}

	//DT Names
	const TArray<FName> ExisitngNames = DT->GetRowNames();
	FName NewName;

	bool ForceRandomName = false;

	if (InRowName == FName() && !UseAutoRowName) ForceRandomName = true;
	//If no UseAutoRowName
	else if (!UseAutoRowName)
	{
		//Search for a not taken name
		if (!ExisitngNames.Contains(InRowName)) NewName = InRowName;
		else ForceRandomName = true;
	}

	if (UseAutoRowName || ForceRandomName)
	{
		//Force name in datatable?
		if (QuestEditorDev::Get().GetSettings()->UseQuestNameAsRowName)
		{
			NewName = FName(QuestIn.QuestName.ToString());
		}
		else
		{
			//Generate valid name
			//Add a new row to the datatable with a standar unused name
			NewName = DataTableUtils::MakeValidName(TEXT("Quest"));

			while (ExisitngNames.Contains(NewName))
			{
				NewName.SetNumber(NewName.GetNumber() + 1);
			}
		}		
	}

	//Localization adjustment
	FQuest LocalQuest = GenerateQuestLocalizationKeys(QuestIn, DT);

	DT->AddRow(NewName, LocalQuest);
	UE_LOG(QuestEditorDevLog, Log, TEXT("UQuestEditorDevFunctionLibrary::AddQuestRowToDT -Quest %s (Unique ID: %i) added to DT in DTPath: %s with row name: %s"), *LocalQuest.QuestName.ToString(), LocalQuest.UniqueID, *DatatableIn.ToString(), *NewName.ToString());
	DT->MarkPackageDirty();

	return true;
}

bool UQuestEditorDevFunctionLibrary::AddDialogRowToDT(const FQuest& QuestIn, FName InRowName, TSoftObjectPtr <UDataTable> DatatableIn, bool UseAutoRowName)
{
	//Get dt
	UDataTable* DT = UQuestsFunctionLibrary::GetDTFromPath(DatatableIn);

	if (!DT)
	{
		UE_LOG(QuestEditorDevLog, Error, TEXT("UQuestEditorDevFunctionLibrary::AddDialogRowToDT - Could not find the datatable: %s. Add Dialog cancelled"), *DatatableIn.ToString());
		return false;
	}

	//DT Names
	const TArray<FName> ExisitngNames = DT->GetRowNames();
	FName NewName;

	bool ForceRandomName = false;

	if (InRowName == FName() & !UseAutoRowName) ForceRandomName = true;
	//If no UseAutoRowName
	else if (!UseAutoRowName)
	{
		//Search for a not taken name
		if (!ExisitngNames.Contains(InRowName)) NewName = InRowName;
		else ForceRandomName = true;
	}

	if (UseAutoRowName || ForceRandomName)
	{
		//Generate valid name
		//Add a new row to the datatable with a standar unused name
		NewName = DataTableUtils::MakeValidName(TEXT("Dialog"));

		while (ExisitngNames.Contains(NewName))
		{
			NewName.SetNumber(NewName.GetNumber() + 1);
		}
	}

	//Converts quest into a dialog struct
	FDialog DialogLocal = UQuestEditorFunctionLibrary::ConvertQuestToDialog(QuestIn);

	//Localization IDs
	DialogLocal = GenerateDialogLocalizationKeys(DialogLocal, DatatableIn);

	DT->AddRow(NewName, DialogLocal);
	UE_LOG(QuestEditorDevLog, Log, TEXT("UQuestEditorDevFunctionLibrary::AddQuestRowToDT - Dialog %s added to DT in DTPath: %s with row name: %s"), *DialogLocal.DialogEditorName, *DatatableIn.ToString(), *NewName.ToString());
	DT->MarkPackageDirty();

	return true;
}

FQuest UQuestEditorDevFunctionLibrary::HandleQuestUniqueID(const FQuest& QuestIn, TSoftObjectPtr<UDataTable> DatatableIn)
{
	//Find ID
	int CurrentID = GetQuestMaxUniqueIDInDT(DatatableIn);

	//Set ID for quest
	FQuest LocalQuest = QuestIn;
	LocalQuest.UniqueID = CurrentID + 1;

	UE_LOG(QuestEditorDevLog, Log, TEXT("UQuestEditorDevFunctionLibrary::HandleQuestUniqueID - New Unique ID: %i assigned to quest: %s."), LocalQuest.UniqueID, *LocalQuest.QuestName.ToString());

	return LocalQuest;
}

FQuest UQuestEditorDevFunctionLibrary::HandleDialogUniqueID(const FQuest& QuestIn, TSoftObjectPtr<UDataTable> DatatableIn)
{
	//Find ID in dialogs DTs.
	int CurrentID = GetDialogMaxUniqueIDInDT(DatatableIn);

	//Set ID for dialog in quest
	FQuest LocalQuest = QuestIn;
	LocalQuest.UniqueID = CurrentID + 1;

	UE_LOG(QuestEditorDevLog, Log, TEXT("UQuestEditorDevFunctionLibrary::HandleQuestUniqueID - New Unique ID: %i assigned to Dialog: %s."), LocalQuest.UniqueID, *LocalQuest.QuestName.ToString());

	return LocalQuest;
}

FQuest UQuestEditorDevFunctionLibrary::GenerateQuestLocalizationKeys(const FQuest& QuestIn, TSoftObjectPtr<UDataTable> DatatableIn)
{
	FQuest LocalQuestLocalized = QuestIn;

	FString LocalID = FString();
	FText LocalText = FText();

	if (!QuestIn.QuestName.IsFromStringTable())
	{
		//Deterministic key for name
		LocalID = "QN" + FString::FromInt(QuestIn.UniqueID);
		LocalText = LocalQuestLocalized.QuestName;
		LocalText = FText::ChangeKey(DatatableIn->GetName(), LocalID, LocalText);
		LocalQuestLocalized.QuestName = LocalText;
	}

	if (!QuestIn.QuestDescription.IsFromStringTable())
	{
		//Desc deterministic key
		LocalID = "QD" + FString::FromInt(QuestIn.UniqueID);
		LocalText = LocalQuestLocalized.QuestDescription;
		LocalText = FText::ChangeKey(DatatableIn->GetName(), LocalID, LocalText);
		LocalQuestLocalized.QuestDescription = LocalText;
	}

	//Talk tasks
	for (int i = 0; i < LocalQuestLocalized.TalkTasks.Num(); i++)
	{
		LocalID = "QN" + FString::FromInt(QuestIn.UniqueID) + "TT" + FString::FromInt(LocalQuestLocalized.TalkTasks[i].UniqueID);

		if (!QuestIn.TalkTasks[i].PlayerDialog.IsFromStringTable())
		{
			LocalText = LocalQuestLocalized.TalkTasks[i].PlayerDialog;
			LocalText = FText::ChangeKey(DatatableIn->GetName(), "TPD" + LocalID, LocalText);
			LocalQuestLocalized.TalkTasks[i].PlayerDialog = LocalText;
		}

		if (!QuestIn.TalkTasks[i].NPCResponse.IsFromStringTable())
		{
			LocalText = LocalQuestLocalized.TalkTasks[i].NPCResponse;
			LocalText = FText::ChangeKey(DatatableIn->GetName(), "TNR" + LocalID, LocalText);
			LocalQuestLocalized.TalkTasks[i].NPCResponse = LocalText;
		}

		if (!QuestIn.TalkTasks[i].TaskShortDesc.IsFromStringTable())
		{
			LocalText = LocalQuestLocalized.TalkTasks[i].TaskShortDesc;
			LocalText = FText::ChangeKey(DatatableIn->GetName(), "TSD" + LocalID, LocalText);
			LocalQuestLocalized.TalkTasks[i].TaskShortDesc = LocalText;
		}		
	}

	//Tasks
	for (int i = 0; i < LocalQuestLocalized.Tasks.Num(); i++)
	{
		LocalID = "QN" + FString::FromInt(QuestIn.UniqueID) + "T" + FString::FromInt(LocalQuestLocalized.Tasks[i].UniqueID);

		if (!QuestIn.Tasks[i].TaskShortDesc.IsFromStringTable())
		{
			LocalText = LocalQuestLocalized.Tasks[i].TaskShortDesc;
			LocalText = FText::ChangeKey(DatatableIn->GetName(), "TSS" + LocalID, LocalText);
			LocalQuestLocalized.Tasks[i].TaskShortDesc = LocalText;
		}
	}

	UE_LOG(QuestEditorDevLog, Log, TEXT("UQuestEditorDevFunctionLibrary::GenerateQuestLocalizationKeys - Localization Keys generated for Quest: %s with Unique ID: %i"), 
		*LocalQuestLocalized.QuestName.ToString(), LocalQuestLocalized.UniqueID);

	return LocalQuestLocalized;
}

FDialog UQuestEditorDevFunctionLibrary::GenerateDialogLocalizationKeys(const FDialog& DialogIn, TSoftObjectPtr<UDataTable> DatatableIn)
{
	FDialog LocalDialogLocalized = DialogIn;

	FString LocalID = FString();
	FText LocalText = FText();

	//Dialogs
	for (int i = 0; i < LocalDialogLocalized.Dialogs.Num(); i++)
	{
		LocalID = "DN" + FString::FromInt(LocalDialogLocalized.UniqueID) + "TT" + FString::FromInt(LocalDialogLocalized.Dialogs[i].UniqueID);

		LocalText = LocalDialogLocalized.Dialogs[i].PlayerDialog;
		LocalText = FText::ChangeKey(DatatableIn->GetName(), "DPD" + LocalID, LocalText);
		LocalDialogLocalized.Dialogs[i].PlayerDialog = LocalText;

		LocalText = LocalDialogLocalized.Dialogs[i].NPCResponse;
		LocalText = FText::ChangeKey(DatatableIn->GetName(), "DNR" + LocalID, LocalText);
		LocalDialogLocalized.Dialogs[i].NPCResponse = LocalText;

		LocalText = LocalDialogLocalized.Dialogs[i].TaskShortDesc;
		LocalText = FText::ChangeKey(DatatableIn->GetName(), "DSD" + LocalID, LocalText);
		LocalDialogLocalized.Dialogs[i].TaskShortDesc = LocalText;
	}

	return LocalDialogLocalized;
}

int UQuestEditorDevFunctionLibrary::GetTaskMaxUniqueID(const FQuest& QuestIn, EQuestTaskType TaskType)
{
	int LocalMaxID = -1;

	switch (TaskType)
	{
	case EQuestTaskType::Task:

		for (int i = 0; i < QuestIn.Tasks.Num(); i++)
		{
			if (QuestIn.Tasks[i].UniqueID > LocalMaxID)
			{
				//update max
				LocalMaxID = QuestIn.Tasks[i].UniqueID;
			}
		}

		break;
	case EQuestTaskType::TalkTask:

		for (int i = 0; i < QuestIn.TalkTasks.Num(); i++)
		{
			if (QuestIn.TalkTasks[i].UniqueID > LocalMaxID)
			{
				//update max
				LocalMaxID = QuestIn.TalkTasks[i].UniqueID;
			}
		}

		break;
	default:
		break;
	}

	return LocalMaxID;
}

int UQuestEditorDevFunctionLibrary::GetQuestMaxUniqueIDInDT(TSoftObjectPtr<UDataTable> DatatableIn)
{
	//Get dt
	UDataTable* DT = UQuestsFunctionLibrary::GetDTFromPath(DatatableIn);

	if (!DT)
	{
		UE_LOG(QuestEditorDevLog, Error, TEXT("UQuestEditorDevFunctionLibrary::GetQuestMaxUniqueIDInDT - Could not find the datatable: %s. "), *DatatableIn.ToString());
		return -1;
	}

	FQuest QuestLocal;

	int CurrenUniqueID = -1;
	TArray<FName> Names = DT->GetRowNames();
	FQuest* QuestPointer = nullptr;

	//Exist?
	for (int i = 0; i < Names.Num(); i++)
	{
		QuestPointer = DT->FindRow<FQuest>(Names[i], FString(), true);
		if (QuestPointer != nullptr)
		{
			QuestLocal = *QuestPointer;
			
			//Chek if its same quest by name
			if (QuestLocal.UniqueID > CurrenUniqueID)
			{
				//Update ID
				CurrenUniqueID = QuestLocal.UniqueID;
			}
		}
	}
		
	return CurrenUniqueID;
}

int UQuestEditorDevFunctionLibrary::GetDialogMaxUniqueIDInDT(TSoftObjectPtr<UDataTable> DatatableIn)
{
	//Get dt
	UDataTable* DT = UQuestsFunctionLibrary::GetDTFromPath(DatatableIn);

	if (!DT)
	{
		UE_LOG(QuestEditorDevLog, Error, TEXT("UQuestEditorDevFunctionLibrary::GetQuestMaxUniqueIDInDT - Could not find the datatable: %s. "), *DatatableIn.ToString());
		return -1;
	}

	FDialog StructLocal;

	int CurrenUniqueID = -1;
	TArray<FName> Names = DT->GetRowNames();

	FDialog* StructPointer = nullptr;

	//Exist?
	for (int i = 0; i < Names.Num(); i++)
	{
		StructPointer = DT->FindRow<FDialog>(Names[i], FString(), true);

		if (StructPointer != nullptr)
		{
			StructLocal = *StructPointer;

			//Chek if its same quest by name
			if (StructLocal.UniqueID > CurrenUniqueID)
			{
				//Update ID
				CurrenUniqueID = StructLocal.UniqueID;
			}
		}
	}

	return CurrenUniqueID;
}

bool UQuestEditorDevFunctionLibrary::IsQuestUniqueIDValid(TSoftObjectPtr<UDataTable> DatatableIn, const FQuest& QuestIn)
{
	//The default id is valid always
	if (QuestIn.UniqueID == -1) return true;

	//Get dt
	UDataTable* DT = UQuestsFunctionLibrary::GetDTFromPath(DatatableIn);

	if (!DT)
	{
		UE_LOG(QuestEditorDevLog, Error, TEXT("UQuestEditorDevFunctionLibrary::IsQuestUniqueIDValid - Could not find the datatable: %s. "), *DatatableIn.ToString());
		return true; //avoid regenerating ID if DT is invalid
	}

	FQuest QuestLocal;

	bool IsValid = true;

	TArray<FName> Names = DT->GetRowNames();
	FQuest* QuestPointer = nullptr;

	for (int i = 0; i < Names.Num(); i++)
	{
		QuestPointer = DT->FindRow<FQuest>(Names[i], FString(), true);
		
		if (QuestPointer != nullptr)
		{
			QuestLocal = *QuestPointer;

			//If it finds a quest with different name and same unique ID then the ID is considered invalid
			if (QuestLocal.UniqueID == QuestIn.UniqueID && QuestLocal.QuestName.ToString() == QuestIn.QuestName.ToString())
			{
				//Id invalid
				IsValid = false;
				break;
			}
		}
	}

	return IsValid;
}

bool UQuestEditorDevFunctionLibrary::IsDialogUniqueIDValid(TSoftObjectPtr<UDataTable> DatatableIn, const FQuest& QuestIn)
{
	//The default id is valid always
	if (QuestIn.UniqueID == -1) return true;

	//Get dt
	UDataTable* DT = UQuestsFunctionLibrary::GetDTFromPath(DatatableIn);

	if (!DT)
	{
		UE_LOG(QuestEditorDevLog, Error, TEXT("UQuestEditorDevFunctionLibrary::IsQuestUniqueIDValid - Could not find the datatable: %s. "), *DatatableIn.ToString());
		return true; //avoid regenerating ID if DT is invalid
	}

	FDialog DialogLocal;

	bool IsValid = true;

	TArray<FName> Names = DT->GetRowNames();
	FDialog* DialogPointer = nullptr;

	for (int i = 0; i < Names.Num(); i++)
	{
		DialogPointer = DT->FindRow<FDialog>(Names[i], FString(), true);

		if (DialogPointer != nullptr)
		{
			DialogLocal = *DialogPointer;

			//If it finds a quest with different name and same unique ID then the ID is considered invalid
			if (DialogLocal.UniqueID == QuestIn.UniqueID && DialogLocal.DialogEditorName == QuestIn.QuestName.ToString())
			{
				//Id invalid
				IsValid = false;
				break;
			}
		}
	}

	return IsValid;
}

void UQuestEditorDevFunctionLibrary::SetAllUniqueIDsInQuest(UPARAM(ref)FQuest& QuestIn)
{
	//------------------Talk tasks
	int CurrentMaxID = GetTaskMaxUniqueID(QuestIn, EQuestTaskType::TalkTask);
	
	for (int i = 0; i < QuestIn.TalkTasks.Num(); i++)
	{
		//Is not set?
		if (QuestIn.TalkTasks[i].UniqueID == -1)
		{
			//Calculate next value
			CurrentMaxID++;
			//update with the next value
			QuestIn.TalkTasks[i].UniqueID = CurrentMaxID;
		}
	}
	
	//------------------Tasks
	CurrentMaxID = GetTaskMaxUniqueID(QuestIn, EQuestTaskType::Task);

	for (int i = 0; i < QuestIn.Tasks.Num(); i++)
	{
		//Is not set?
		if (QuestIn.Tasks[i].UniqueID == -1)
		{
			//Calculate next value
			CurrentMaxID++;
			//update with the next value
			QuestIn.Tasks[i].UniqueID = CurrentMaxID;
		}
	}

	return;
}

bool UQuestEditorDevFunctionLibrary::AreQuestTasksMissingIDs(const FQuest& QuestIn)
{
	bool AreMissing = false;

	//------------------Talk tasks
	for (int i = 0; i < QuestIn.TalkTasks.Num(); i++)
	{
		//Is not set?
		if (QuestIn.TalkTasks[i].UniqueID == -1)
		{
			AreMissing = true;
			break;
		}
	}

	//If found a missing one, no need to keep looking
	if (AreMissing) return AreMissing;

	//------------------Tasks
	for (int i = 0; i < QuestIn.Tasks.Num(); i++)
	{
		//Is not set?
		if (QuestIn.Tasks[i].UniqueID == -1)
		{
			AreMissing = true;
			break;
		}
	}	
	
	return AreMissing;
}

bool UQuestEditorDevFunctionLibrary::CreateDisableLinkBetweenNodes(UPARAM(ref)FQuest& QuestIn, FQuestTaskID DisablerNodeIn, FQuestTaskID DisabledNodeIn)
{
	//Cannot disable self
	if (UQuestsFunctionLibrary::EqualQuestTaskID(DisablerNodeIn, DisabledNodeIn))
	{
		UE_LOG(QuestEditorDevLog, Log, TEXT("UQuestEditorDevFunctionLibrary::CreateDisableLinkBetweenNodes - Create disable link cancelled. Disabled and disabler are same node"));
		return false;
	}

	if (!UQuestsFunctionLibrary::IsTaskIDValidInQuest(QuestIn, DisablerNodeIn))
	{
		UE_LOG(QuestEditorDevLog, Warning, TEXT("UQuestEditorDevFunctionLibrary::CreateDisableLinkBetweenNodes - Disabler Node is invalid."));
		return false;
	}

	if (!UQuestsFunctionLibrary::IsTaskIDValidInQuest(QuestIn, DisabledNodeIn))
	{
		UE_LOG(QuestEditorDevLog, Warning, TEXT("UQuestEditorDevFunctionLibrary::CreateDisableLinkBetweenNodes - Disabled Node is invalid."));
		return false;
	}

	//-----------------NODE Disabler
	switch (DisablerNodeIn.TaskType)
	{
	case EQuestTaskType::Task:
	{
		QuestIn.Tasks[DisablerNodeIn.TaskID].DisabledNodesOnComplete.AddUnique(DisabledNodeIn);
	}

	break;
	case EQuestTaskType::TalkTask:
	{
		QuestIn.TalkTasks[DisablerNodeIn.TaskID].DisabledNodesOnComplete.AddUnique(DisabledNodeIn);
	}
	break;
	default:
		break;
	}

	return true;
}

bool UQuestEditorDevFunctionLibrary::DestroyDisableLinkBetweenNodes(UPARAM(ref)FQuest& QuestIn, FQuestTaskID DisablerNodeIn, FQuestTaskID DisabledNodeIn)
{
	//Cannot disable self
	if (DisablerNodeIn == DisabledNodeIn)
	{
		UE_LOG(QuestEditorDevLog, Warning, TEXT("UQuestEditorDevFunctionLibrary::DestroyDisableLinkBetweenNodes - Attempted to break a Disable link between the same Node!."));
		return false;
	}

	if (!UQuestsFunctionLibrary::IsTaskIDValidInQuest(QuestIn, DisablerNodeIn))
	{
		UE_LOG(QuestEditorDevLog, Warning, TEXT("UQuestEditorDevFunctionLibrary::DestroyDisableLinkBetweenNodes - Disabler Node is invalid."));
		return false;
	}

	if (!UQuestsFunctionLibrary::IsTaskIDValidInQuest(QuestIn, DisabledNodeIn))
	{
		UE_LOG(QuestEditorDevLog, Warning, TEXT("UQuestEditorDevFunctionLibrary::DestroyDisableLinkBetweenNodes - Disabled Node is invalid."));
		return false;
	}

	//-----------------NODE Disabler
	switch (DisablerNodeIn.TaskType)
	{
	case EQuestTaskType::Task:
	{
		QuestIn.Tasks[DisablerNodeIn.TaskID].DisabledNodesOnComplete.Remove(DisabledNodeIn);
	}

	break;
	case EQuestTaskType::TalkTask:
	{
		QuestIn.TalkTasks[DisablerNodeIn.TaskID].DisabledNodesOnComplete.Remove(DisabledNodeIn);
	}
	break;
	default:
		break;
	}

	return true;
}

TArray<FQuestTaskID> UQuestEditorDevFunctionLibrary::GetDisabledNodesOfNode(const FQuest& QuestIn, FQuestTaskID Node)
{
	if (!UQuestsFunctionLibrary::IsTaskIDValidInQuest(QuestIn, Node))
	{
		UE_LOG(QuestEditorDevLog, Warning, TEXT("UQuestEditorDevFunctionLibrary::GetDisabledNodesOfNode - Node is invalid."));
		return TArray<FQuestTaskID>();
	}

	switch (Node.TaskType)
	{
	case EQuestTaskType::Task: return QuestIn.Tasks[Node.TaskID].DisabledNodesOnComplete;
		break;
	case EQuestTaskType::TalkTask:return QuestIn.TalkTasks[Node.TaskID].DisabledNodesOnComplete;
		break;
	default:
		break;
	}
	return TArray<FQuestTaskID>();
}

TArray<FQuestTaskID> UQuestEditorDevFunctionLibrary::GetDisablerNodesOfNode(const FQuest& QuestIn, FQuestTaskID Node)
{
	if (!UQuestsFunctionLibrary::IsTaskIDValidInQuest(QuestIn, Node))
	{
		UE_LOG(QuestEditorDevLog, Warning, TEXT("UQuestEditorDevFunctionLibrary::GetDisablerNodesOfNode - Node is invalid."));
		return TArray<FQuestTaskID>();
	}

	FQuestTaskID IDLocal;
	TArray<FQuestTaskID> DisablersNodes;
	DisablersNodes.Empty();

	//-----------------------------------Tasks
	IDLocal.TaskType = EQuestTaskType::Task;
	for (int i = 0; i < QuestIn.Tasks.Num(); i++)
	{
		if (QuestIn.Tasks[i].DisabledNodesOnComplete.Contains(Node))
		{
			IDLocal.TaskID = i;
			DisablersNodes.Add(IDLocal);
		}
	}

	//-----------------------------------Talk Tasks
	IDLocal.TaskType = EQuestTaskType::TalkTask;
	for (int i = 0; i < QuestIn.TalkTasks.Num(); i++)
	{
		if (QuestIn.TalkTasks[i].DisabledNodesOnComplete.Contains(Node))
		{
			IDLocal.TaskID = i;
			DisablersNodes.Add(IDLocal);
		}
	}

	return DisablersNodes;
}

bool UQuestEditorDevFunctionLibrary::RemoveTaskByID(UPARAM(ref)FQuest& QuestIn, FQuestTaskID TaskID)
{
	if (!UQuestsFunctionLibrary::IsTaskIDValidInQuest(QuestIn, TaskID))
	{
		UE_LOG(QuestEditorDevLog, Warning, TEXT("UQuestEditorDevFunctionLibrary::RemoveTaskByID - Failed to remove task ID. Node is invalid."));
		return false;
	}
	   
	//------------------------------------------------------------------------------------------------------------------------------------------------
	//------------------------------------------------Remove this node from superior and inferior nodes that reference it
	//------------------------------------------------------------------------------------------------------------------------------------------------
	//Remove superior and inferior nodes connections for this node
	TArray <FQuestTaskLink> LinksLocal = TArray <FQuestTaskLink>();
	
	TArray <FQuestTaskID> InferiorNodes = UQuestsFunctionLibrary::GetNodeInferiorNodesInQuest(QuestIn, TaskID);

	//Delete from inferior nodes in the superior array.
	for (int i = 0; i < InferiorNodes.Num(); i++)
	{
		switch (InferiorNodes[i].TaskType)
		{
		case EQuestTaskType::Task:			
		{

			//No valid index?
			if (QuestIn.Tasks.IsValidIndex(InferiorNodes[i].TaskID))
			{
				LinksLocal = UQuestsFunctionLibrary::RemoveTaskIDFromLinkArray(TaskID, QuestIn.Tasks[InferiorNodes[i].TaskID].SuperiorNodes);
				QuestIn.Tasks[InferiorNodes[i].TaskID].SuperiorNodes = LinksLocal;
			}
			else UE_LOG(QuestEditorDevLog, Log, TEXT("UQuestEditorDevFunctionLibrary::RemoveTaskByID -Task of ID %i is not valid index and cannot remove from Superior nodes."), InferiorNodes[i].TaskID);
		}
			break;
		case EQuestTaskType::TalkTask:			
		{
			//No valid index?
			if (QuestIn.TalkTasks.IsValidIndex(InferiorNodes[i].TaskID))
			{
				LinksLocal = UQuestsFunctionLibrary::RemoveTaskIDFromLinkArray(TaskID, QuestIn.TalkTasks[InferiorNodes[i].TaskID].SuperiorNodes);
				QuestIn.TalkTasks[InferiorNodes[i].TaskID].SuperiorNodes = LinksLocal;
			}
			else UE_LOG(QuestEditorDevLog, Log, TEXT("UQuestEditorDevFunctionLibrary::RemoveTaskByID -Talk Task of ID %i is not valid index and cannot remove from Superior nodes."), InferiorNodes[i].TaskID);

		}		
			break;
		default:
			break;
		}
	}

	TArray <FQuestTaskID> SuperiorNodes = UQuestsFunctionLibrary::GetNodeSuperiorNodesInQuest(QuestIn, TaskID);

	//Delete from superior nodes in the inferior array
	for (int i = 0; i < SuperiorNodes.Num(); i++)
	{
		switch (SuperiorNodes[i].TaskType)
		{
		case EQuestTaskType::Task:			
		{
			//No valid index?
			if (QuestIn.Tasks.IsValidIndex(SuperiorNodes[i].TaskID))
			{
				LinksLocal = UQuestsFunctionLibrary::RemoveTaskIDFromLinkArray(TaskID, QuestIn.Tasks[SuperiorNodes[i].TaskID].InferiorNodes);
				QuestIn.Tasks[SuperiorNodes[i].TaskID].InferiorNodes = LinksLocal;
			}
			else UE_LOG(QuestEditorDevLog, Log, TEXT("UQuestEditorDevFunctionLibrary::RemoveTaskByID -Task of ID %i is not valid index and cannot remove from inferior nodes."), SuperiorNodes[i].TaskID);
		}

			break;
		case EQuestTaskType::TalkTask:
		{
			//No valid index?
			if (QuestIn.TalkTasks.IsValidIndex(SuperiorNodes[i].TaskID))
			{
				LinksLocal = UQuestsFunctionLibrary::RemoveTaskIDFromLinkArray(TaskID, QuestIn.TalkTasks[SuperiorNodes[i].TaskID].InferiorNodes);
				QuestIn.TalkTasks[SuperiorNodes[i].TaskID].InferiorNodes = LinksLocal;
			}
			else UE_LOG(QuestEditorDevLog, Log, TEXT("UQuestEditorDevFunctionLibrary::RemoveTaskByID -Talk Task of ID %i is not valid index and cannot remove from inferior nodes."), SuperiorNodes[i].TaskID);
		}
			break;
		default:
			break;
		}
	}

	//------------------------------------------------------------------------------------------------------------------------------------------------
	//------------------------------------------------Remove this node from disabled nodes that reference it (must go through all)
	//------------------------------------------------------------------------------------------------------------------------------------------------

	//---------------------------------------------------------DisabledNODES
	//Disabled nodes requires to search all quest since there is no "is disabled by nodes"
	//If the node is in the array, it is removed
	for (int i = 0; i < QuestIn.Tasks.Num(); i++)
	{
		//Tasks
		QuestIn.Tasks[i].DisabledNodesOnComplete.Remove(TaskID);
	}

	for (int i = 0; i < QuestIn.TalkTasks.Num(); i++)
	{
		//TalkTasks
		QuestIn.TalkTasks[i].DisabledNodesOnComplete.Remove(TaskID);
	}

	//------------------------------------------------------------------------------------------------------------------------------------------------
	//--------------------------------Adjust all TASK IDS that are superior to this node (needs to subtract 1 for all)----------------------------------
	//------------------------------------------------------------------------------------------------------------------------------------------------
	//Search all tasks and talk tasks, if the ID is superior and same tasks type, needs to lower in 1.
	//---------------------------------------------------------Tasks
	for (int i = 0; i < QuestIn.Tasks.Num(); i++)
	{
		for (int j = 0; j < QuestIn.Tasks[i].InferiorNodes.Num(); j++)
		{
			//if same type and superior ID subtract 1
			if (QuestIn.Tasks[i].InferiorNodes[j].Node.TaskType == TaskID.TaskType && QuestIn.Tasks[i].InferiorNodes[j].Node.TaskID > TaskID.TaskID)  QuestIn.Tasks[i].InferiorNodes[j].Node.TaskID = QuestIn.Tasks[i].InferiorNodes[j].Node.TaskID - 1;
		}
		for (int j = 0; j < QuestIn.Tasks[i].SuperiorNodes.Num(); j++)
		{
			//if same type and superior ID subtract 1
			if (QuestIn.Tasks[i].SuperiorNodes[j].Node.TaskType == TaskID.TaskType && QuestIn.Tasks[i].SuperiorNodes[j].Node.TaskID > TaskID.TaskID)  QuestIn.Tasks[i].SuperiorNodes[j].Node.TaskID = QuestIn.Tasks[i].SuperiorNodes[j].Node.TaskID - 1;
		}
		for (int j = 0; j < QuestIn.Tasks[i].DisabledNodesOnComplete.Num(); j++)
		{
			//if same type and superior ID subtract 1
			if (QuestIn.Tasks[i].DisabledNodesOnComplete[j].TaskType == TaskID.TaskType && QuestIn.Tasks[i].DisabledNodesOnComplete[j].TaskID > TaskID.TaskID)  QuestIn.Tasks[i].DisabledNodesOnComplete[j].TaskID = QuestIn.Tasks[i].DisabledNodesOnComplete[j].TaskID - 1;
		}
	}

	//---------------------------------------------------------Talk Tasks
	for (int i = 0; i < QuestIn.TalkTasks.Num(); i++)
	{
		for (int j = 0; j < QuestIn.TalkTasks[i].InferiorNodes.Num(); j++)
		{
			//if same type and superior ID subtract 1
			if (QuestIn.TalkTasks[i].InferiorNodes[j].Node.TaskType == TaskID.TaskType && QuestIn.TalkTasks[i].InferiorNodes[j].Node.TaskID > TaskID.TaskID)  QuestIn.TalkTasks[i].InferiorNodes[j].Node.TaskID = QuestIn.TalkTasks[i].InferiorNodes[j].Node.TaskID - 1;
		}
		for (int j = 0; j < QuestIn.TalkTasks[i].SuperiorNodes.Num(); j++)
		{
			//if same type and superior ID subtract 1
			if (QuestIn.TalkTasks[i].SuperiorNodes[j].Node.TaskType == TaskID.TaskType && QuestIn.TalkTasks[i].SuperiorNodes[j].Node.TaskID > TaskID.TaskID)  QuestIn.TalkTasks[i].SuperiorNodes[j].Node.TaskID = QuestIn.TalkTasks[i].SuperiorNodes[j].Node.TaskID - 1;
		}
		for (int j = 0; j < QuestIn.TalkTasks[i].DisabledNodesOnComplete.Num(); j++)
		{
			//if same type and superior ID subtract 1
			if (QuestIn.TalkTasks[i].DisabledNodesOnComplete[j].TaskType == TaskID.TaskType && QuestIn.TalkTasks[i].DisabledNodesOnComplete[j].TaskID > TaskID.TaskID)  QuestIn.TalkTasks[i].DisabledNodesOnComplete[j].TaskID = QuestIn.TalkTasks[i].DisabledNodesOnComplete[j].TaskID - 1;
		}
	}
	   	  
	//----------------------------------------------------------------- 
	//-----------------Remove task from array
	//----------------------------------------------------------------- 
	//Finally the actual task is removed from the array it is in

	switch (TaskID.TaskType)
	{
	case EQuestTaskType::Task: 
		QuestIn.Tasks.RemoveAt(TaskID.TaskID);
		UE_LOG(QuestEditorDevLog, Log, TEXT("UQuestEditorDevFunctionLibrary::RemoveTaskByID - Task of ID %i removed."), TaskID.TaskID);
		break;
	case EQuestTaskType::TalkTask:
		QuestIn.TalkTasks.RemoveAt(TaskID.TaskID);
		UE_LOG(QuestEditorDevLog, Log, TEXT("UQuestEditorDevFunctionLibrary::RemoveTaskByID -Talk Task of ID %i removed."), TaskID.TaskID);
		break;
	default:
		break;
	}

	return true;
}

bool UQuestEditorDevFunctionLibrary::RemoveMultipleTasksByID(UPARAM(ref)FQuest& QuestIn, const TArray<FQuestTaskID> TaskIDs, bool UpdateConversationIDs)
{
	FQuestTaskID CurrentDeletedNode = FQuestTaskID();

	//Before delete, must order IDs from Bigger to lower.
	//This prevents from corrupting IDS data
	TArray<FQuestTaskID> TasksSorted = SortIDArrayDecreasing(TaskIDs);

	UE_LOG(QuestEditorDevLog, Log, TEXT("UQuestEditorDevFunctionLibrary::RemoveMultipleTasksByID -Sorted tasks amount: %i."), TasksSorted.Num());
	
	for (int i = 0; i < TasksSorted.Num(); i++)
	{
		UE_LOG(QuestEditorDevLog, Log, TEXT("UQuestEditorDevFunctionLibrary::RemoveMultipleTasksByID -Start Remove task or talk task ID: %i."), TasksSorted[i].TaskID);
		RemoveTaskByID(QuestIn, TasksSorted[i]);
	}

	//Update all conversations IDs
	if(UpdateConversationIDs) UpdateAllConversationsIDs(QuestIn);

	return true;
}

TArray<FQuestTaskID> UQuestEditorDevFunctionLibrary::SortIDArrayDecreasing(const TArray<FQuestTaskID> TaskIDs)
{
	TArray<FQuestTaskID> ArrayLocal = TaskIDs;

	//Sort by ID order using lambda.
	Algo::Sort(ArrayLocal, 
		[](FQuestTaskID a, FQuestTaskID b)
		{
			return a.TaskID > b.TaskID; 
		}
		);
	
	return ArrayLocal;
}

bool UQuestEditorDevFunctionLibrary::ClearAllConnections(UPARAM(ref)FQuest& QuestIn)
{
	//Clear all nodes connections, Used for debugging only
	for (int i = 0; i < QuestIn.TalkTasks.Num(); i++)
	{
		QuestIn.TalkTasks[i].InferiorNodes.Empty();
		QuestIn.TalkTasks[i].SuperiorNodes.Empty();
		QuestIn.TalkTasks[i].DisabledNodesOnComplete.Empty();
	}

	for (int i = 0; i < QuestIn.Tasks.Num(); i++)
	{
		QuestIn.Tasks[i].InferiorNodes.Empty();
		QuestIn.Tasks[i].SuperiorNodes.Empty();
		QuestIn.Tasks[i].DisabledNodesOnComplete.Empty();
	}

	return true;
}

int UQuestEditorDevFunctionLibrary::AddTaskOfType(UPARAM(ref)FQuest& QuestIn, EQuestTaskType TaskType)
{
	int Slot = -1;

	//Get ID current
	int CurrentID = GetTaskMaxUniqueID(QuestIn, TaskType);

	switch (TaskType)
	{
	case EQuestTaskType::Task:
	{
		Slot = QuestIn.Tasks.Num();
		FQuestTask TaskLocal = FQuestTask();

		//Set ID
		TaskLocal.UniqueID = CurrentID + 1;

		QuestIn.Tasks.Add(TaskLocal);
		UE_LOG(QuestEditorDevLog, Log, TEXT("UQuestEditorDevFunctionLibrary::AddTaskOfType -Task of ID %i added."), Slot);
	}
	break;
	case EQuestTaskType::TalkTask:
	{
		Slot = QuestIn.TalkTasks.Num();
		FQuestTalkTask TalkTaskLocal = FQuestTalkTask();

		//Set ID
		TalkTaskLocal.UniqueID = CurrentID + 1;

		QuestIn.TalkTasks.Add(TalkTaskLocal);
		//Update the conversation ID value to self
		QuestIn.TalkTasks[QuestIn.TalkTasks.Num() - 1].ConversationID = QuestIn.TalkTasks.Num() - 1;
		UE_LOG(QuestEditorDevLog, Log, TEXT("UQuestEditorDevFunctionLibrary::AddTaskOfType - Talk Task of ID %i added."), Slot);
	}
	break;
	default:
		break;
	}

	return Slot;
}

bool UQuestEditorDevFunctionLibrary::BreakNodesConnection(UPARAM(ref)FQuest& QuestIn, FQuestTaskID SuperiorNode, FQuestTaskID InferiorNode, bool UpdateConversationIDs)
{
	TArray <FQuestTaskLink> LinksLocal = TArray <FQuestTaskLink>();

	//-----------------NODE Inferior
	//Superior node goes as superior node of inferior nod
	switch (InferiorNode.TaskType)
	{
	case EQuestTaskType::Task:
	{
		LinksLocal = UQuestsFunctionLibrary::RemoveTaskIDFromLinkArray(SuperiorNode, QuestIn.Tasks[InferiorNode.TaskID].SuperiorNodes);
		QuestIn.Tasks[InferiorNode.TaskID].SuperiorNodes = LinksLocal;
	}

	break;
	case EQuestTaskType::TalkTask:
	{
		LinksLocal = UQuestsFunctionLibrary::RemoveTaskIDFromLinkArray(SuperiorNode, QuestIn.TalkTasks[InferiorNode.TaskID].SuperiorNodes);
		QuestIn.TalkTasks[InferiorNode.TaskID].SuperiorNodes = LinksLocal;
	}
	break;
	default:
		break;
	}

	//-----------------NODE Superior

	switch (SuperiorNode.TaskType)
	{
	case EQuestTaskType::Task:
	{
		LinksLocal = UQuestsFunctionLibrary::RemoveTaskIDFromLinkArray(InferiorNode, QuestIn.Tasks[SuperiorNode.TaskID].InferiorNodes);
		QuestIn.Tasks[SuperiorNode.TaskID].InferiorNodes = LinksLocal;
	}

	break;
	case EQuestTaskType::TalkTask:
	{
		LinksLocal = UQuestsFunctionLibrary::RemoveTaskIDFromLinkArray(InferiorNode, QuestIn.TalkTasks[SuperiorNode.TaskID].InferiorNodes);
		QuestIn.TalkTasks[SuperiorNode.TaskID].InferiorNodes = LinksLocal;
	}
	break;
	default:
		break;
	}

	//Update all conversations IDs
	if(UpdateConversationIDs) UpdateAllConversationsIDs(QuestIn);

	UE_LOG(QuestEditorDevLog, Log, TEXT("UQuestEditorDevFunctionLibrary::BreakNodesConnection - Link destroyed. Superior Node(type: %i id: %i) Inferior Node(type: %i id: %i)"), SuperiorNode.TaskID, (uint8)SuperiorNode.TaskType, InferiorNode.TaskID, (uint8)InferiorNode.TaskType);
	return true;
}

bool UQuestEditorDevFunctionLibrary::BreakAllConnectionOfNode(UPARAM(ref)FQuest& QuestIn, FQuestTaskID Node)
{
	//Not valid node?
	if (!UQuestsFunctionLibrary::IsTaskIDValidInQuest(QuestIn, Node)) return false;

	//Get node superior connections
	TArray <FQuestTaskID> NodesLocal = UQuestsFunctionLibrary::GetNodeSuperiorNodesInQuest(QuestIn, Node);
	
	//Remove all superior connections
	for (int i = 0; i < NodesLocal.Num(); i++)
	{
		BreakNodesConnection(QuestIn, NodesLocal[i], Node);
	}
		
	//Get node superior connections
	NodesLocal = UQuestsFunctionLibrary::GetNodeInferiorNodesInQuest(QuestIn, Node);

	//Remove all inferior connections
	for (int i = 0; i < NodesLocal.Num(); i++)
	{
		BreakNodesConnection(QuestIn, Node, NodesLocal[i]);
	}
	
	return true;
}

TArray <FQuestTaskID> UQuestEditorDevFunctionLibrary::BreakAllConnectionOfNodeToConnector(UPARAM(ref)FQuest& QuestIn, FQuestTaskID NodeIn, ENodeConnectorType ConnectorIn)
{
	//Not valid node?
	if (!UQuestsFunctionLibrary::IsTaskIDValidInQuest(QuestIn, NodeIn)) return TArray <FQuestTaskID>();
	
	TArray <FQuestTaskID> NodesLocal = TArray <FQuestTaskID>();

	switch (ConnectorIn)
	{
	case ENodeConnectorType::Inferior:

		NodesLocal = UQuestsFunctionLibrary::GetNodeInferiorNodesInQuest(QuestIn, NodeIn);
		for (int i = 0; i < NodesLocal.Num(); i++)
		{
			BreakNodesConnection(QuestIn, NodeIn, NodesLocal[i]);
		}

		break;
	case ENodeConnectorType::Superior:

		NodesLocal = UQuestsFunctionLibrary::GetNodeSuperiorNodesInQuest(QuestIn, NodeIn);
		for (int i = 0; i < NodesLocal.Num(); i++)
		{
			BreakNodesConnection(QuestIn, NodesLocal[i], NodeIn);
		}

		break;
	case ENodeConnectorType::Disable:

		NodesLocal = GetDisabledNodesOfNode(QuestIn, NodeIn);
		for (int i = 0; i < NodesLocal.Num(); i++)
		{
			DestroyDisableLinkBetweenNodes(QuestIn, NodeIn, NodesLocal[i]);
		}

		break;
	default:
		break;
	}
	
	//Add node in to modified
	NodesLocal.Add(NodeIn);

	//Update all conversation IDs
	UpdateAllConversationsIDs(QuestIn);

	return NodesLocal;
}

bool UQuestEditorDevFunctionLibrary::CreateNewLinkBetweenNodes(UPARAM(ref)FQuest& QuestIn, FQuestTaskID SuperiorNode, FQuestTaskID InferiorNode, ENodeLinkType LinkType, bool UpdateConversationIDs)
{
	if (!CanLinkNodes(QuestIn, SuperiorNode, InferiorNode))
	{
		UE_LOG(QuestEditorDevLog, Warning, TEXT("UQuestEditorDevFunctionLibrary::CreateNewLinkBetweenNodes - Cannot create link between nodes."));
		return false;
	}		
		
	FQuestTaskLink TaskLink = FQuestTaskLink();
	
	//req by default
	TaskLink.LinkType = LinkType;

	//-----------------NODE SUPERIOR
	//Superior node goes as superior node of inferior nod
	switch (InferiorNode.TaskType)
	{
	case EQuestTaskType::Task:
	{
		TaskLink.Node = SuperiorNode;
		QuestIn.Tasks[InferiorNode.TaskID].InferiorNodes.Add(TaskLink);
	}

	break;
	case EQuestTaskType::TalkTask:
	{
		TaskLink.Node = SuperiorNode;
		QuestIn.TalkTasks[InferiorNode.TaskID].InferiorNodes.Add(TaskLink);
	}
	break;
	default:
		break;
	}
	//--------------NODE INFERIOR
	//Inferior node goes as inferior node of the superior node

	switch (SuperiorNode.TaskType)
	{
	case EQuestTaskType::Task:
	{
		TaskLink.Node = InferiorNode;
		QuestIn.Tasks[SuperiorNode.TaskID].SuperiorNodes.Add(TaskLink);
	}

	break;
	case EQuestTaskType::TalkTask:
	{
		TaskLink.Node = InferiorNode;
		QuestIn.TalkTasks[SuperiorNode.TaskID].SuperiorNodes.Add(TaskLink);
	}
	break;
	default:
		break;
	}

	//Update conversations for all quest if needed
	if (UpdateConversationIDs) UpdateAllConversationsIDs(QuestIn);

	UE_LOG(QuestEditorDevLog, Log, TEXT("UQuestEditorDevFunctionLibrary::CreateNewLinkBetweenNodes - Link Created. Superior Node(type: %i id: %i) Inferior Node(type: %i id: %i)"), SuperiorNode.TaskID, (uint8)SuperiorNode.TaskType, InferiorNode.TaskID, (uint8)InferiorNode.TaskType);
	return true;
}

bool UQuestEditorDevFunctionLibrary::CanLinkNodes(FQuest QuestIn, FQuestTaskID SuperiorNode, FQuestTaskID InferiorNode)
{

	//Superior node goes as superior node of inferior nod
	//Inferior node goes as inferior node of the superior node

	//Same node cannot continue
	if (SuperiorNode.TaskType == InferiorNode.TaskType && SuperiorNode.TaskID == InferiorNode.TaskID) return false;
	
	//---------------------------------Nodes are valid?
	if (!UQuestsFunctionLibrary::IsTaskIDValidInQuest(QuestIn, SuperiorNode)) return false;
	if (!UQuestsFunctionLibrary::IsTaskIDValidInQuest(QuestIn, InferiorNode)) return false;

	//-----------------------------------Connection already exists?
	//if inferior node has the superior node already in the superior array
	//if superior node has the inferior node already in the inferior array

	//-----------------------------Immediate Circular conection         This is no longer needed, circular connections are allowed.
	//If inferior node has superior node as inferior and viceversa. I do not check for circular with 3 or more nodes. This is up to the quest maker
	//None of the nodes can have the other node in any of the superior on inferior arrays

	//-----------------NODE Inferior

	//if (FindNodeIDInNodeArray(UQuestsFunctionLibrary::GetNodeSuperiorNodesInQuest(QuestIn, InferiorNode), SuperiorNode) >= 0) return false;
	//if (FindNodeIDInNodeArray(UQuestsFunctionLibrary::GetNodeInferiorNodesInQuest(QuestIn, InferiorNode), SuperiorNode) >= 0) return false;

	//-----------------NODE Superior

	//if (FindNodeIDInNodeArray(UQuestsFunctionLibrary::GetNodeSuperiorNodesInQuest(QuestIn, SuperiorNode), InferiorNode) >= 0) return false;
	//if (FindNodeIDInNodeArray(UQuestsFunctionLibrary::GetNodeInferiorNodesInQuest(QuestIn, SuperiorNode), InferiorNode) >= 0) return false;

	return true;
}

bool UQuestEditorDevFunctionLibrary::CanLinkWithDisableNode(FQuest QuestIn, FQuestTaskID DisablerNode, FQuestTaskID DisabledNode)
{
	//Same nodes cannot link
	if (DisablerNode == DisabledNode) return false;

	//Disabled Node cannot be an inferior node of the disabler.
	if (FindNodeIDInNodeArray(UQuestsFunctionLibrary::GetNodeInferiorNodesInQuest(QuestIn, DisablerNode), DisabledNode) >= 0) return false;
		
	return true;
}

bool UQuestEditorDevFunctionLibrary::AreNodesLinked(FQuest QuestIn, FQuestTaskID SuperiorNode, FQuestTaskID InferiorNode)
{
	//---------------------------------Nodes are valid?
	if (!UQuestsFunctionLibrary::IsTaskIDValidInQuest(QuestIn, SuperiorNode)) return false;
	if (!UQuestsFunctionLibrary::IsTaskIDValidInQuest(QuestIn, InferiorNode)) return false;
	
	//For a conexion to exits, superior node must have the inferior node in its inferior array 
	if (FindNodeIDInNodeArray(UQuestsFunctionLibrary::GetNodeInferiorNodesInQuest(QuestIn, SuperiorNode), InferiorNode) < 0) return false;

	//and the inferior node must have the superior node in its superior array
	if (FindNodeIDInNodeArray(UQuestsFunctionLibrary::GetNodeSuperiorNodesInQuest(QuestIn, InferiorNode), SuperiorNode) < 0) return false;

	return true;
}

bool UQuestEditorDevFunctionLibrary::ChangeLinkTypeBetweenNodes(UPARAM(ref)FQuest& QuestIn, FQuestTaskID SuperiorNode, FQuestTaskID InferiorNode)
{
	//---------------------------------Nodes are valid?
	if (!UQuestsFunctionLibrary::IsTaskIDValidInQuest(QuestIn, SuperiorNode)) return false;
	if (!UQuestsFunctionLibrary::IsTaskIDValidInQuest(QuestIn, InferiorNode)) return false;

	//--------------------------------Nodes are linked?
	if (!AreNodesLinked(QuestIn, SuperiorNode, InferiorNode))
	{
		UE_LOG(QuestEditorDevLog, Warning, TEXT("UQuestEditorDevFunctionLibrary::ChangeLinkTypeBetweenNodes - Links are not linked! Operation cancelled. Superior Node(type: %i id: %i) Inferior Node(type: %i id: %i)"), (uint8)SuperiorNode.TaskType, SuperiorNode.TaskID, (uint8)InferiorNode.TaskType, InferiorNode.TaskID);
		return false;
	}
		
	//--------------------------------------Superior Node
	//superior node must have the inferior node in its inferior array 
	//Get the position of the node in the superior node
	int NodeInArrayPosition = FindNodeIDInNodeArray(UQuestsFunctionLibrary::GetNodeInferiorNodesInQuest(QuestIn, SuperiorNode), InferiorNode);
		
	switch (SuperiorNode.TaskType)
	{
	case EQuestTaskType::Task:
		//Search the current value
		if (QuestIn.Tasks[SuperiorNode.TaskID].InferiorNodes[NodeInArrayPosition].LinkType == ENodeLinkType::Optional) 
			//If is optional set to required
			QuestIn.Tasks[SuperiorNode.TaskID].InferiorNodes[NodeInArrayPosition].LinkType = ENodeLinkType::Required;
		//if is required set to optional
		else QuestIn.Tasks[SuperiorNode.TaskID].InferiorNodes[NodeInArrayPosition].LinkType = ENodeLinkType::Optional;
		break;
	case EQuestTaskType::TalkTask:
		//Search the current value
		if (QuestIn.TalkTasks[SuperiorNode.TaskID].InferiorNodes[NodeInArrayPosition].LinkType == ENodeLinkType::Optional)
			//If is optional set to required
			QuestIn.TalkTasks[SuperiorNode.TaskID].InferiorNodes[NodeInArrayPosition].LinkType = ENodeLinkType::Required;
		//if is required set to optional
		else QuestIn.TalkTasks[SuperiorNode.TaskID].InferiorNodes[NodeInArrayPosition].LinkType = ENodeLinkType::Optional;
		break;
	default:
		break;
	}
	
	//--------------------------------------Inferior Node
	//superior node must have the inferior node in its inferior array 
	//Get the position of the node in the superior node
	NodeInArrayPosition = FindNodeIDInNodeArray(UQuestsFunctionLibrary::GetNodeSuperiorNodesInQuest(QuestIn, InferiorNode), SuperiorNode);

	switch (InferiorNode.TaskType)
	{
	case EQuestTaskType::Task:
		//Search the current value
		if (QuestIn.Tasks[InferiorNode.TaskID].SuperiorNodes[NodeInArrayPosition].LinkType == ENodeLinkType::Optional)
			//If is optional set to required
			QuestIn.Tasks[InferiorNode.TaskID].SuperiorNodes[NodeInArrayPosition].LinkType = ENodeLinkType::Required;
		//if is required set to optional
		else QuestIn.Tasks[InferiorNode.TaskID].SuperiorNodes[NodeInArrayPosition].LinkType = ENodeLinkType::Optional;
		break;
	case EQuestTaskType::TalkTask:
		//Search the current value
		if (QuestIn.TalkTasks[InferiorNode.TaskID].SuperiorNodes[NodeInArrayPosition].LinkType == ENodeLinkType::Optional)
			//If is optional set to required
			QuestIn.TalkTasks[InferiorNode.TaskID].SuperiorNodes[NodeInArrayPosition].LinkType = ENodeLinkType::Required;
		//if is required set to optional
		else QuestIn.TalkTasks[InferiorNode.TaskID].SuperiorNodes[NodeInArrayPosition].LinkType = ENodeLinkType::Optional;
		break;
	default:
		break;
	}
		
	UE_LOG(QuestEditorDevLog, Log, TEXT("UQuestEditorDevFunctionLibrary::ChangeLinkTypeBetweenNodes - Link type changed. Superior Node(type: %i id: %i) Inferior Node(type: %i id: %i)"), (uint8)SuperiorNode.TaskType, SuperiorNode.TaskID, (uint8)InferiorNode.TaskType, InferiorNode.TaskID );
	
	return true;
}

int UQuestEditorDevFunctionLibrary::FindNodeIDInNodeArray(TArray<FQuestTaskID> NodeIDArray, FQuestTaskID Node)
{
	int found = -1;
	for (int i = 0; i < NodeIDArray.Num(); i++)
	{
		if (NodeIDArray[i].TaskType == Node.TaskType && NodeIDArray[i].TaskID == Node.TaskID)
		{
			found = i;
			break;
		}
	}
	return found;
}

bool UQuestEditorDevFunctionLibrary::GetNodeConnections(const FQuest& QuestIn, FQuestTaskID Node, TArray<FQuestTaskID>& SuperiorNodesOut, TArray<FQuestTaskID>& InferiorNodesOut)
{
	//Complete conex, This superior node with all his inferior nodes and this inferior node with all the superior nodes
	TArray<FQuestTaskID> SuperiorNodes;
	TArray<FQuestTaskID> InferiorNodes;

	SuperiorNodes = UQuestsFunctionLibrary::GetNodeSuperiorNodesInQuest(QuestIn, Node);

	//For each superior node adds self as inferior node
	for (int i = 0; i < SuperiorNodes.Num(); i++)
	{
		InferiorNodes.Add(Node);
	}

	//For each inferior node adds self as superior node
	TArray<FQuestTaskID> NODOSINTERNOS = UQuestsFunctionLibrary::GetNodeInferiorNodesInQuest(QuestIn, Node);
	InferiorNodes.Append(NODOSINTERNOS);

	for (int i = 0; i < NODOSINTERNOS.Num(); i++)
	{
		SuperiorNodes.Add(Node);
	}

	//Variables
	SuperiorNodesOut = SuperiorNodes;
	InferiorNodesOut = InferiorNodes;
	return true;
}

bool UQuestEditorDevFunctionLibrary::GetNodeDisableConnections(const FQuest& QuestIn, FQuestTaskID Node, TArray<FQuestTaskID>& DisablerNodesOut, TArray<FQuestTaskID>& DisabledNodesOut)
{
	FQuestTaskID IDLocal;
	TArray<FQuestTaskID> DisablerNodes;
	TArray<FQuestTaskID> DisabledNodes;

	//-----------------------------------Tasks
	IDLocal.TaskType = EQuestTaskType::Task;
	for (int i = 0; i < QuestIn.Tasks.Num(); i++)
	{
		//Node is contained on disabled nodes of this task?
		if (QuestIn.Tasks[i].DisabledNodesOnComplete.Contains(Node))
		{
			//Then this node is disabler and node In is disabled
			IDLocal.TaskID = i;
			DisablerNodes.Add(IDLocal);
			DisabledNodes.Add(Node);
		}
	}

	//-----------------------------------Talk Tasks
	IDLocal.TaskType = EQuestTaskType::TalkTask;
	for (int i = 0; i < QuestIn.TalkTasks.Num(); i++)
	{
		//Node is contained on disabled nodes of this task?
		if (QuestIn.TalkTasks[i].DisabledNodesOnComplete.Contains(Node))
		{
			//Then this node is disabler and node In is disabled
			IDLocal.TaskID = i;
			DisablerNodes.Add(IDLocal);
			DisabledNodes.Add(Node);
		}
	}

	//--------------------Disabled nodes of the node IN
	TArray<FQuestTaskID> LocalDisabledNodesOfNode = GetDisabledNodesOfNode(QuestIn, Node);

	for (int i = 0; i < LocalDisabledNodesOfNode.Num(); i++)
	{
		//Node is now disabler and all its disabled nodes make pairs.
		DisablerNodes.Add(Node);
		DisabledNodes.Add(LocalDisabledNodesOfNode[i]);

	}

	//Variables OUT
	DisablerNodesOut = DisablerNodes;
	DisabledNodesOut = DisabledNodes;
	return true;
}

bool UQuestEditorDevFunctionLibrary::GetAllNodesConnections(const FQuest& QuestIn, TArray<FQuestTaskID>& SuperiorNodesOut, TArray<FQuestTaskID>& InferiorNodesOut, TArray<ENodeLinkType>& LinkTypesOut)
{
	//Only use inferior nodes for this
	//For each node, search inferior and superiors and make a parallel array.
	TArray<FQuestTaskID> InferiorNodes;
	InferiorNodes.Empty();
	TArray<FQuestTaskID> SuperiorNodes;
	SuperiorNodes.Empty();
	FQuestTaskID IDLocal;
	TArray<ENodeLinkType> LinkTypesLocal = TArray<ENodeLinkType>();
	//-----------------------------------Tasks
	IDLocal.TaskType = EQuestTaskType::Task;
	for (int i = 0; i < QuestIn.Tasks.Num(); i++)
	{
		InferiorNodes.Append(UQuestsFunctionLibrary::ConvertLinkArrayToNodeArray(QuestIn.Tasks[i].InferiorNodes));
		for (int j = 0; j < QuestIn.Tasks[i].InferiorNodes.Num(); j++)
		{
			IDLocal.TaskID = i;
			LinkTypesLocal.Add(QuestIn.Tasks[i].InferiorNodes[j].LinkType);
			SuperiorNodes.Add(IDLocal);
		}
	}

	//-----------------------------------Talk Tasks
	IDLocal.TaskType = EQuestTaskType::TalkTask;
	for (int i = 0; i < QuestIn.TalkTasks.Num(); i++)
	{
		InferiorNodes.Append(UQuestsFunctionLibrary::ConvertLinkArrayToNodeArray(QuestIn.TalkTasks[i].InferiorNodes));
		for (int j = 0; j < QuestIn.TalkTasks[i].InferiorNodes.Num(); j++)
		{
			IDLocal.TaskID = i;
			LinkTypesLocal.Add(QuestIn.TalkTasks[i].InferiorNodes[j].LinkType);
			SuperiorNodes.Add(IDLocal);
		}
	}

	SuperiorNodesOut = SuperiorNodes;
	InferiorNodesOut = InferiorNodes;
	LinkTypesOut = LinkTypesLocal;

	return true;
}

bool UQuestEditorDevFunctionLibrary::GetAllSelectedNodesConnections(const FQuest& QuestIn, const TArray<FQuestTaskID>& SelectedNodes, TArray<FQuestTaskID>& SuperiorNodesOut, TArray<FQuestTaskID>& InferiorNodesOut, TArray<ENodeLinkType>& LinkTypesOut)
{	
	//Only use inferior nodes for this
	//For each node, search inferior and superiors and make a parallel array.
	TArray<FQuestTaskID> InferiorNodes;
	InferiorNodes.Empty();
	TArray<FQuestTaskID> SuperiorNodes;
	SuperiorNodes.Empty();
	FQuestTaskID IDLocal;
	TArray<ENodeLinkType> LinkTypesLocal = TArray<ENodeLinkType>();

	TArray<FQuestTaskID> InferiorNodesNoFilter = TArray<FQuestTaskID>();

	//----------------------------------------Inferior Nodes
	for (int i = 0; i < SelectedNodes.Num(); i++)
	{
		//For this node, get all inferior nodes
		InferiorNodesNoFilter = UQuestsFunctionLibrary::ConvertLinkArrayToNodeArray(UQuestsFunctionLibrary::GetNodeInferiorLinksInQuest(QuestIn, SelectedNodes[i]));
			
		//For inferior nodes, if they are selected, add the connection
		for (int j = 0; j < InferiorNodesNoFilter.Num(); j++)
		{
			if (SelectedNodes.Contains(InferiorNodesNoFilter[j]))
			{
				//Inferior
				InferiorNodes.Add(InferiorNodesNoFilter[j]);
				//Superior
				SuperiorNodes.Add(SelectedNodes[i]);
			}
		}	
	}

	SuperiorNodesOut = SuperiorNodes;
	InferiorNodesOut = InferiorNodes;
	LinkTypesOut = LinkTypesLocal;

	return true;
}

bool UQuestEditorDevFunctionLibrary::GetAllNodesDisablesConnections(const FQuest& QuestIn, TArray<FQuestTaskID>& DisablerNodesOut, TArray<FQuestTaskID>& DisabledNodesOut)
{
	FQuestTaskID IDLocal;
	TArray<FQuestTaskID> DisablerNodes;
	TArray<FQuestTaskID> DisabledNodes;

	//-----------------------------------Tasks
	IDLocal.TaskType = EQuestTaskType::Task;
	for (int i = 0; i < QuestIn.Tasks.Num(); i++)
	{
		DisabledNodes.Append(QuestIn.Tasks[i].DisabledNodesOnComplete);
		for (int j = 0; j < QuestIn.Tasks[i].DisabledNodesOnComplete.Num(); j++)
		{
			IDLocal.TaskID = i;
			DisablerNodes.Add(IDLocal);
		}
	}

	//-----------------------------------Talk Tasks
	IDLocal.TaskType = EQuestTaskType::TalkTask;
	for (int i = 0; i < QuestIn.TalkTasks.Num(); i++)
	{
		DisabledNodes.Append(QuestIn.TalkTasks[i].DisabledNodesOnComplete);
		for (int j = 0; j < QuestIn.TalkTasks[i].DisabledNodesOnComplete.Num(); j++)
		{
			IDLocal.TaskID = i;
			DisablerNodes.Add(IDLocal);
		}
	}

	//Variables OUT
	DisablerNodesOut = DisablerNodes;
	DisabledNodesOut = DisabledNodes;
	return true;
}

bool UQuestEditorDevFunctionLibrary::CanNodeStartSwapConnection(const FQuest& QuestIn, FQuestTaskID NodeIn, ENodeConnectorType ConnectorIn)
{
	if (!UQuestsFunctionLibrary::IsTaskIDValidInQuest(QuestIn, NodeIn)) return false;
	
	TArray <FQuestTaskID> NodeIDs = TArray <FQuestTaskID>();
	TArray <FQuestTaskLink> NodeLinks = TArray <FQuestTaskLink>();

	switch (ConnectorIn)
	{
	case ENodeConnectorType::Inferior:
		//Inferior
		NodeLinks = UQuestsFunctionLibrary::GetNodeInferiorLinksInQuest(QuestIn, NodeIn);
		//If there is something to swap it can
		return NodeLinks.Num() > 0;
		break;
	case ENodeConnectorType::Superior:
		//Superior
		NodeLinks = UQuestsFunctionLibrary::GetNodeSuperiorLinksInQuest(QuestIn, NodeIn);
		//If there is something to swap it can
		return NodeLinks.Num() > 0;
		break;
	case ENodeConnectorType::Disable:
		//Disable
		//If is disabler swap, at least has to have 1 disable connection
		NodeIDs = UQuestEditorDevFunctionLibrary::GetDisabledNodesOfNode(QuestIn, NodeIn);
		//If there is something to swap it can
		return NodeIDs.Num() > 0;
		break;
	default:
		break;
	}
	//Should never reach this part.
	return false;
}

TArray <FString> UQuestEditorDevFunctionLibrary::SwapNodeConnections(UPARAM(ref)FQuest& QuestIn, const FQuestTaskID& NodeA, const FQuestTaskID& NodeB, ENodeConnectorType ConnectorIn, bool UpdateConversationIDs)
{
	//Same nodes dont do anything
	if (NodeA == NodeB)
	{
		UE_LOG(QuestEditorDevLog, Warning, TEXT("UQuestEditorDevFunctionLibrary::SwapNodeConnections - Nodes are the same, cancelled operation."));
		return TArray <FString>();
	}

	//valid IDs
	if (!UQuestsFunctionLibrary::IsTaskIDValidInQuest(QuestIn, NodeA))
	{
		UE_LOG(QuestEditorDevLog, Warning, TEXT("UQuestEditorDevFunctionLibrary::SwapNodeConnections - NodeA is invalid."));
		return TArray <FString>();
	}

	if (!UQuestsFunctionLibrary::IsTaskIDValidInQuest(QuestIn, NodeB))
	{
		UE_LOG(QuestEditorDevLog, Warning, TEXT("UQuestEditorDevFunctionLibrary::SwapNodeConnections - NodeB is invalid."));
		return TArray <FString>();
	}

	TArray <FQuestTaskLink> NodeALinks = TArray <FQuestTaskLink>();
	TArray <FQuestTaskLink> NodeBLinks = TArray <FQuestTaskLink>();
	TArray <FQuestTaskID> NodeAIDs = TArray <FQuestTaskID>();
	TArray <FQuestTaskID> NodeBIDs = TArray <FQuestTaskID>();

	//For messages
	FString NodeAName;
	FString NodeInvolvedName;

	TArray <FString> MessagesOut = TArray <FString>();

	//Inferior and superior are switched, because inferior node button makes it superior node to other nodes and viceversa
	switch (ConnectorIn)
	{
	case ENodeConnectorType::Inferior:
		//---------------------------------------------Superior
		//If Node A is superior, we are replacing all superior links of its inferior nodes with node B
		//Get all inferior connections of node A
		NodeALinks = UQuestsFunctionLibrary::GetNodeInferiorLinksInQuest(QuestIn, NodeA);

		//Get node B inferior nodes
		NodeBLinks = UQuestsFunctionLibrary::GetNodeInferiorLinksInQuest(QuestIn, NodeB);

		//Get the node array from B
		NodeBIDs = UQuestsFunctionLibrary::ConvertLinkArrayToNodeArray(NodeBLinks);

		//Add each connection of A that is missing in node B
		for (int i = 0; i < NodeALinks.Num(); i++)
		{
			//If is not contained create the link
			if (!NodeBIDs.Contains(NodeALinks[i].Node))
			{
				//Destroy old conection with node A
				BreakNodesConnection(QuestIn, NodeA, NodeALinks[i].Node);
				//Create link between NodeB and the inferior node of A
				CreateNewLinkBetweenNodes(QuestIn, NodeALinks[i].Node, NodeB, NodeALinks[i].LinkType);
			}
		}

		//Conections that could not be set in the B node from the A node are destroyed. A message must be supplied.
		NodeALinks = UQuestsFunctionLibrary::GetNodeInferiorLinksInQuest(QuestIn, NodeA);
		for (int i = 0; i < NodeALinks.Num(); i++)
		{
			//Node A name
			switch (NodeA.TaskType)
			{
			case EQuestTaskType::Task:
				NodeAName = "Task " + FString::FromInt(NodeA.TaskID);
				break;
			case EQuestTaskType::TalkTask:
				NodeAName = "Talk Task " + FString::FromInt(NodeA.TaskID);
				break;
			default:
				break;
			}

			//Node involved name
			switch (NodeALinks[i].Node.TaskType)
			{
			case EQuestTaskType::Task:
				NodeInvolvedName = "Task " + FString::FromInt(NodeALinks[i].Node.TaskID);
				break;
			case EQuestTaskType::TalkTask:
				NodeInvolvedName = "Talk Task " + FString::FromInt(NodeALinks[i].Node.TaskID);
				break;
			default:
				break;
			}

			//Destroy all remmaining conections on node A 
			MessagesOut.Add("Link destroyed: " + NodeAName + " - " + NodeInvolvedName);		
			BreakNodesConnection(QuestIn, NodeA, NodeALinks[i].Node);
		}
		break;
	case ENodeConnectorType::Superior:
		//---------------------------------------------Inferior
		//If node A is inferior, we are replacing all inferior links of its superior nodes with node B
		//Get all inferior connections of node A
		NodeALinks = UQuestsFunctionLibrary::GetNodeSuperiorLinksInQuest(QuestIn, NodeA);

		//Get node B inferior nodes
		NodeBLinks = UQuestsFunctionLibrary::GetNodeSuperiorLinksInQuest(QuestIn, NodeB);

		//Get the node array from B
		NodeBIDs = UQuestsFunctionLibrary::ConvertLinkArrayToNodeArray(NodeBLinks);

		//Add each connection of A that is missing in node B
		for (int i = 0; i < NodeALinks.Num(); i++)
		{
			//If is not contained create the link
			if (!NodeBIDs.Contains(NodeALinks[i].Node))
			{
				//Destroy old conection with node A
				BreakNodesConnection(QuestIn, NodeALinks[i].Node, NodeA);

				//Create link between nodeB and the superior node of A
				CreateNewLinkBetweenNodes(QuestIn, NodeB, NodeALinks[i].Node, NodeALinks[i].LinkType);
			}
		}

		//Conections that could not be set in the B node from the A node are destroyed. A message must be supplied.
		NodeALinks = UQuestsFunctionLibrary::GetNodeSuperiorLinksInQuest(QuestIn, NodeA);
		for (int i = 0; i < NodeALinks.Num(); i++)
		{
			//Node A name
			switch (NodeA.TaskType)
			{
			case EQuestTaskType::Task:
				NodeAName = "Task " + FString::FromInt(NodeA.TaskID);
				break;
			case EQuestTaskType::TalkTask:
				NodeAName = "Talk Task " + FString::FromInt(NodeA.TaskID);
				break;
			default:
				break;
			}

			//Node involved name
			switch (NodeALinks[i].Node.TaskType)
			{
			case EQuestTaskType::Task:
				NodeInvolvedName = "Task " + FString::FromInt(NodeALinks[i].Node.TaskID);
				break;
			case EQuestTaskType::TalkTask:
				NodeInvolvedName = "Talk Task " + FString::FromInt(NodeALinks[i].Node.TaskID);
				break;
			default:
				break;
			}

			//Destroy all remmaining conections on node A
			MessagesOut.Add("Link destroyed: " + NodeAName + " - " + NodeInvolvedName);
			BreakNodesConnection(QuestIn, NodeA, NodeALinks[i].Node);
		}
		break;
	case ENodeConnectorType::Disable:
		//---------------------------------------------Disabler
		//Get all Disable nodes of A
		NodeAIDs = UQuestEditorDevFunctionLibrary::GetDisabledNodesOfNode(QuestIn, NodeA);

		//Get the node B disables
		NodeBIDs = UQuestEditorDevFunctionLibrary::GetDisabledNodesOfNode(QuestIn, NodeB);

		//Add each connection of A that is missing in node B
		for (int i = 0; i < NodeAIDs.Num(); i++)
		{
			//If is not contained create the link
			if (!NodeBIDs.Contains(NodeAIDs[i]))
			{
				//Destroy old conection with node A
				DestroyDisableLinkBetweenNodes(QuestIn, NodeA, NodeAIDs[i]);

				//Create link between NodeB and the inferior node of A
				CreateDisableLinkBetweenNodes(QuestIn, NodeB, NodeAIDs[i]);
			}
		}

		//Conections that could not be set in the B node from the A node are destroyed. A message must be supplied.
		NodeAIDs = GetDisabledNodesOfNode(QuestIn, NodeA);

		for (int i = 0; i < NodeAIDs.Num(); i++)
		{
			//Node A name
			switch (NodeA.TaskType)
			{
			case EQuestTaskType::Task:
				NodeAName = "Task" + FString::FromInt(NodeA.TaskID);
				break;
			case EQuestTaskType::TalkTask:
				NodeAName = "Talk Task" + FString::FromInt(NodeA.TaskID);
				break;
			default:
				break;
			}

			//Node involved name
			switch (NodeAIDs[i].TaskType)
			{
			case EQuestTaskType::Task:
				NodeInvolvedName = "Task " + FString::FromInt(NodeAIDs[i].TaskID);
				break;
			case EQuestTaskType::TalkTask:
				NodeInvolvedName = "Talk Task " + FString::FromInt(NodeAIDs[i].TaskID);
				break;
			default:
				break;
			}

			//Destroy all remmaining conections on node A
			MessagesOut.Add("Disable Link destroyed: " + NodeAName + " - " + NodeInvolvedName);
			DestroyDisableLinkBetweenNodes(QuestIn, NodeA, NodeAIDs[i]);
		}

		break;
	default:
		break;
	}

	//Update conversation IDs
	if (UpdateConversationIDs) UpdateAllConversationsIDs(QuestIn);

	return MessagesOut;	
}

void UQuestEditorDevFunctionLibrary::UpdateAllConversationsIDs(UPARAM(ref)FQuest& QuestIn)
{
	//Get all tasks Conversations Map
	TMap <FQuestTaskID, int> ConversationsMap = UQuestsFunctionLibrary::GetQuestConversationsMap(QuestIn);
	
	TArray <FQuestTaskID> Keys = TArray <FQuestTaskID>();
	ConversationsMap.GetKeys(Keys);
	/*
	for (int i = 0; i < Keys.Num(); i++)
	{
		UE_LOG(QuestEditorDevLog, Log, TEXT("UQuestEditorDevFunctionLibrary::UpdateAllConversationsIDs - Map Key Id: %i, KeyType: %i Map Value: %i."), Keys[i].TaskID, Keys[i].TaskType, ConversationsMap[Keys[i]]);
	}
	*/
	FQuestTaskID CurrentID = FQuestTaskID();
	CurrentID.TaskType = EQuestTaskType::TalkTask;
	int KeyArrayPosition = 0;

	for (int i = 0; i < QuestIn.TalkTasks.Num(); i++)
	{
		//make ID for this talk task
		CurrentID.TaskType = EQuestTaskType::TalkTask;
		CurrentID.TaskID = i;
	
		//GetTheMapKey
		KeyArrayPosition = Keys.Find(CurrentID);

		//For some reason this works and the direct map access does not.
		QuestIn.TalkTasks[i].ConversationID = ConversationsMap[Keys[KeyArrayPosition]];
	}

	return;
}

FVector2D UQuestEditorDevFunctionLibrary::GetNodeCenterPosition(const FQuest& QuestIn, const FQuestTaskID SelectedNodeIn, const FVector2D& NodeSizeIn)
{
	return GetNodePosition(QuestIn, SelectedNodeIn) + (NodeSizeIn/2);
}

FVector2D UQuestEditorDevFunctionLibrary::ConvertCenterPositionToQEPosition(const FVector2D& NodeSizeIn, const FVector2D& NodeCenterPositionIn)
{
	return NodeCenterPositionIn - (NodeSizeIn/2);
}

int UQuestEditorDevFunctionLibrary::GetPositionValue(const FQuest& QuestIn, const TArray<FQuestTaskID>& SelectedNodesIn, const TArray <FVector2D>& NodesWidgetSizeIn, bool UseY, bool CheckLower, FQuestTaskID& NodeOut)
{
	//Initial value
	int Value = 0;
	if (CheckLower)
	{
		Value = 10000000;
	}
	else
	{
		Value = -10000000;
	}
	FQuestTaskID Node;
	int CurrentValue = 0;
	FVector2D LocalVector = FVector2D();

	for (int i = 0; i < SelectedNodesIn.Num(); i++)
	{
		if (UQuestsFunctionLibrary::IsTaskIDValidInQuest(QuestIn, SelectedNodesIn[i]))
		{
			//Get center position of node to compare correctly.
			LocalVector = GetNodeCenterPosition(QuestIn, SelectedNodesIn[i], NodesWidgetSizeIn[i]);

			//Get the value
			if (UseY)
			{
				CurrentValue = LocalVector.Y;
			}
			else
			{
				CurrentValue = LocalVector.X;
			}

			//Is this new value a lower value? 
			if (CheckLower)
			{
				//Lower
				if (CurrentValue < Value) Value = CurrentValue; //UPDATE VALUE
				Node = SelectedNodesIn[i];
			}		
			else
			{
				//Higher
				if (CurrentValue > Value) Value = CurrentValue; //UPDATE VALUE
				Node = SelectedNodesIn[i];
			}
		}
	}

	//tells what node is the one with the value
	NodeOut = Node;

	return Value;
}

FVector2D UQuestEditorDevFunctionLibrary::GetNodePosition(const FQuest& QuestIn, const FQuestTaskID& NodeIn)
{
	FVector2D Position = FVector2D();

	if (UQuestsFunctionLibrary::IsTaskIDValidInQuest(QuestIn, NodeIn))
	{
		//Get value of current vector
		switch (NodeIn.TaskType)
		{
		case EQuestTaskType::Task:
			Position = QuestIn.Tasks[NodeIn.TaskID].QuestEditorPosition;
			break;
		case EQuestTaskType::TalkTask:
			Position = QuestIn.TalkTasks[NodeIn.TaskID].QuestEditorPosition;
			break;
		default:
			break;
		}
	}

	return Position;
}

TArray<FQuestTaskID> UQuestEditorDevFunctionLibrary::GetQuestStartNodes(const FQuest& QuestIn)
{
	TArray<FQuestTaskID> Nodes = TArray<FQuestTaskID>();
	FQuestTaskID NodeLocal = FQuestTaskID();

	NodeLocal.TaskType = EQuestTaskType::Task;
	for (int i = 0; i < QuestIn.Tasks.Num(); i++)
	{
		//Is start node?
		if (QuestIn.Tasks[i].SuperiorNodes.Num() == 0)
		{
			NodeLocal.TaskID = i;
			Nodes.Add(NodeLocal);
		}
	}
	
	NodeLocal.TaskType = EQuestTaskType::TalkTask;
	for (int i = 0; i < QuestIn.TalkTasks.Num(); i++)
	{
		//Is start node?
		if (QuestIn.TalkTasks[i].SuperiorNodes.Num() == 0)
		{
			NodeLocal.TaskID = i;
			Nodes.Add(NodeLocal);
		}
	}	
	
	return Nodes;
}

TArray<FQuestTaskID> UQuestEditorDevFunctionLibrary::GetQuestEndNodes(const FQuest& QuestIn)
{
	TArray<FQuestTaskID> Nodes = TArray<FQuestTaskID>();
	FQuestTaskID NodeLocal = FQuestTaskID();

	NodeLocal.TaskType = EQuestTaskType::Task;
	for (int i = 0; i < QuestIn.Tasks.Num(); i++)
	{
		//Is start node?
		if (QuestIn.Tasks[i].InferiorNodes.Num() == 0)
		{
			NodeLocal.TaskID = i;
			Nodes.Add(NodeLocal);
		}
	}

	NodeLocal.TaskType = EQuestTaskType::TalkTask;
	for (int i = 0; i < QuestIn.TalkTasks.Num(); i++)
	{
		//Is start node?
		if (QuestIn.TalkTasks[i].InferiorNodes.Num() == 0)
		{
			NodeLocal.TaskID = i;
			Nodes.Add(NodeLocal);
		}
	}

	return Nodes;
}

TArray <FQuestTaskID> UQuestEditorDevFunctionLibrary::SortNodesByEditorPositiong(const FQuest& QuestIn, const TArray<FQuestTaskID>& Nodes, bool UseY, bool UseReverseOrder)
{
	//Make a map with node and position
	TMap <FQuestTaskID, int32> TaskIDsMap;

	//Fill map	
	if (UseY)
	{
		for (int i = 0; i < Nodes.Num(); i++)
		{
			//Add each node with its position to the map
			TaskIDsMap.Add(Nodes[i], GetNodePosition(QuestIn, Nodes[i]).Y);
		}
	}
	else 
	{
		for (int i = 0; i < Nodes.Num(); i++)
		{
			//Add each node with its position to the map
			TaskIDsMap.Add(Nodes[i], GetNodePosition(QuestIn, Nodes[i]).X);
		}
	}
	
	if (UseReverseOrder)
	{
		//Sort by Y >
		TaskIDsMap.ValueSort([](int a, int b)
			{
				return a > b;
			}
		);
	}
	else
	{
		//Sort by Y <
		TaskIDsMap.ValueSort([](int a, int b)
			{
				return a < b;
			}
		);
	}	

	//Out vars
	TArray<FQuestTaskID> SortedIDs;
	TaskIDsMap.GenerateKeyArray(SortedIDs);
			   	 	
	return SortedIDs;
}

bool UQuestEditorDevFunctionLibrary::AreNodeArraysEqual(const TArray<FQuestTaskID>& NodesA, const TArray<FQuestTaskID>& NodesB)
{
	//diferent legnth diferent arrays
	if (NodesA.Num() != NodesB.Num()) return false;

	bool AreEqual = true;

	//if finds a node that is not contained in the other array they are not equal
	for (int i = 0; i < NodesA.Num(); i++)
	{
		if (!NodesB.Contains(NodesA[i]))
		{
			AreEqual = false;
			break;
		}
	}	
	
	return AreEqual;
}

TArray<FQuestTaskID> UQuestEditorDevFunctionLibrary::GetAllNodesOfSameTaskType(const FQuest& QuestIn, const FQuestTaskID NodesIn)
{
	//Node selected must be task
	if (NodesIn.TaskType == EQuestTaskType::TalkTask) return TArray<FQuestTaskID>();

	TArray<FQuestTaskID> NodesOut;
	FQuestTaskID NodeLocal = FQuestTaskID();
	NodeLocal.TaskType = EQuestTaskType::Task;

	//Search same task nodes
	for (int i = 0; i < QuestIn.Tasks.Num(); i++)
	{
		if (QuestIn.Tasks[i].TaskClass == QuestIn.Tasks[NodesIn.TaskID].TaskClass)
		{
			NodeLocal.TaskID = i;
			NodesOut.Add(NodeLocal);
		}
	}

	return NodesOut;
}

FVector2D UQuestEditorDevFunctionLibrary::GetNodeGridTier(const FQuest& QuestIn, const FQuestTaskID NodeIn, const FVector2D& NodeWidgetSizeIn, const FVector2D& GridSizeIn)
{
	//invalid node?
	if (!UQuestsFunctionLibrary::IsTaskIDValidInQuest(QuestIn, NodeIn)) return FVector2D::ZeroVector;

	//Get node cenbter position
	FVector2D Position = GetNodeCenterPosition(QuestIn, NodeIn, NodeWidgetSizeIn);

	//Calculate The amount of grid movements on each axis
	Position.X = Position.X / GridSizeIn.X;
	Position.Y = Position.Y / GridSizeIn.Y;

	//Truncate values
	Position.X = FMath::RoundHalfToEven(Position.X);
	Position.Y = FMath::RoundHalfToEven(Position.Y);

	return Position;
}

TArray<FQuestTaskID> UQuestEditorDevFunctionLibrary::GetQuestNonConditionalStartNodes(const FQuest& QuestIn)
{
	TArray <FQuestTaskID> SlotsInitial = TArray <FQuestTaskID>();
	FQuestTaskID LocalID = FQuestTaskID();
	FNPCQuestDef NPCDefLocal = FNPCQuestDef();

	//--------------------------Tasks
	LocalID.TaskType = EQuestTaskType::Task;
	for (int i = 0; i < QuestIn.Tasks.Num(); i++)
	{
		//Is start node?
		if (QuestIn.Tasks[i].SuperiorNodes.Num() == 0)
		{
			//Is NOT activate cond only?
			if (!UQuestsFunctionLibrary::TaskIsActivateConditionOnly(QuestIn, i))
			{
				LocalID.TaskID = i;
				SlotsInitial.AddUnique(LocalID);
			}
		}
	}
	
	//--------------------------Talk Tasks
	LocalID.TaskType = EQuestTaskType::TalkTask;
	for (int i = 0; i < QuestIn.TalkTasks.Num(); i++)
	{
		//Is start node?
		if (QuestIn.TalkTasks[i].SuperiorNodes.Num() == 0)
		{
			LocalID.TaskID = i;
			SlotsInitial.AddUnique(LocalID);			
		}
	}
	
	return SlotsInitial;
}

TArray<FQuestTaskID> UQuestEditorDevFunctionLibrary::GetQuestPseudoStartNodes(const FQuest& QuestIn)
{
	//Propagates activation of condition only tasks to get the state of the quest once all are autocompleted and gets the talktasks that this nodes activate.

	//------------------------------------------Obtain  all tasks that are starting and activate condition only
	TArray <int> StartACOTasksCompleted = GetInitialPseudoTasksOfQuest(QuestIn);

	if (StartACOTasksCompleted.Num() == 0) return TArray <FQuestTaskID>();

	//---------------------------------------get all initial inferior nodes
	TArray <FQuestTaskID> InferiorNodesA;
	TArray <FQuestTaskID> InferiorNodesB;
	TArray <FQuestTaskID> AnalizedNodes;
	TArray <FQuestTaskID> PseudoCompletedNodes;

	//Talk task pseudo initial (they are initial after ACO nodes completed)
	TArray <FQuestTaskID> PseudoInitialNodesOut;

	FQuestTaskID TaskIDLocal;
	TaskIDLocal.TaskType = EQuestTaskType::Task;

	//Add to array all inferior nodes of ACO that are "completed" on start
	for (int i = 0; i < StartACOTasksCompleted.Num(); i++)
	{
		TaskIDLocal.TaskID = StartACOTasksCompleted[i];
		//Temporary save all initial ACO nodes on ID form
		InferiorNodesB.AddUnique(TaskIDLocal);
		//Add all to initial pool
		AnalizedNodes.Append(UQuestsFunctionLibrary::ConvertLinkArrayToNodeArray(QuestIn.Tasks[StartACOTasksCompleted[i]].InferiorNodes));
	}

	//filter to get all unique inferior nodes no duplicates
	for (int i = 0; i < AnalizedNodes.Num(); i++)
	{
		InferiorNodesA.AddUnique(AnalizedNodes[i]);
	}

	//Save all ACO nodes used previously. 
	//Dont care about repeated nodes here
	AnalizedNodes = InferiorNodesB;

	PseudoCompletedNodes = InferiorNodesB;	//This are condition tasks only, that i will consider completed for a pseudo activate
	AnalizedNodes = InferiorNodesB; //This are all analized nodes

	//Clear Bnodes
	InferiorNodesB.Empty();

	FNPCQuestDef NPCDefLocal = FNPCQuestDef();

	bool WhileEnable = true;

	int32 LoopCounter = 0;
	while (WhileEnable && LoopCounter < 500)  //Use a big number for while cancel
	{
		LoopCounter++;
		if (LoopCounter == 500) UE_LOG(QuestEditorDevLog, Error, TEXT("UQuestEditorDevFunctionLibrary::GetQuestPseudoStartNodes - While Max Loop reached"));
			
		//Check all inferior A nodes
		//talk node
		for (int i = 0; i < InferiorNodesA.Num(); i++)
		{
			//Node is already analized?
			if (AnalizedNodes.Contains(InferiorNodesA[i])) continue;  //continue

			//Talk task
			if (InferiorNodesA[i].TaskType == EQuestTaskType::TalkTask)
			{
				//if it can potentially be a pseudo start (make sure all superior REQUIRED nodes are conditions)
				if (UQuestsFunctionLibrary::AreAllRequiredSuperiorNodesOfNodeConditions(QuestIn, InferiorNodesA[i]))  
				{
					//Add to pseudo start
					PseudoInitialNodesOut.Add(InferiorNodesA[i]);
				}
			}
			//Tasks
			else
			{
				//ActivateConditionOnly tasks (no spawn task, only check)
				if (UQuestsFunctionLibrary::TaskIsActivateConditionOnly(QuestIn, InferiorNodesA[i].TaskID))
				{
					//Asumes it can always activate
					//add to array of already analized 
					AnalizedNodes.Add(InferiorNodesA[i]);

					//add  INFERIOR nodes to arrayB to check in next while loop (propagation)				
					InferiorNodesB.Append(UQuestsFunctionLibrary::ConvertLinkArrayToNodeArray(QuestIn.Tasks[InferiorNodesA[i].TaskID].InferiorNodes));
				}
				//If task is not activate condition only is a pseudo start task
				else
				{
					//if it can potentially be a pseudo start (make sure all superior nodes are conditions)
					//if (UQuestsFunctionLibrary::AreAllSuperiorNodesOfNodeConditions(QuestIn, InferiorNodesA[i]))
					{
						//Add to pseudo start
						PseudoInitialNodesOut.Add(InferiorNodesA[i]);
					}					
				}
			}			

			//add to array of already analized, prevent nodes to be analized more than once
			AnalizedNodes.Add(InferiorNodesA[i]);
		}

		//If B is empty there is nothing more to check
		if (InferiorNodesB.Num() == 0) WhileEnable = false;
		//Need to clear A, load with B and clear B
		else
		{
			InferiorNodesA = InferiorNodesB;
			InferiorNodesB.Empty();
		}
	}

	//Filter unique only
	TArray<FQuestTaskID> FilteredNodes;

	for (int i = 0; i < PseudoInitialNodesOut.Num(); i++)
	{
		FilteredNodes.AddUnique(PseudoInitialNodesOut[i]);
	}

	return FilteredNodes;	
}

TArray<FNPCQuestDefRep> UQuestEditorDevFunctionLibrary::GetQuestStartNPCs(const FQuest& QuestIn)
{	
	TArray<FQuestTaskID> StartNodes = TArray<FQuestTaskID>();
	//Get all starts and pseudo start nodes
	StartNodes = GetQuestNonConditionalStartNodes(QuestIn);
	StartNodes.Append(GetQuestPseudoStartNodes(QuestIn));
	
	TArray<FNPCQuestDefRep> NPCs = TArray<FNPCQuestDefRep>();
	for (int i = 0; i < StartNodes.Num(); i++)
	{
		//Is talk task?
		if (StartNodes[i].TaskType == EQuestTaskType::TalkTask)
		{
			//Get the npc of the talk tasks
			if (QuestIn.TalkTasks[StartNodes[i].TaskID].TalkNPC.NPCDefDTName != "None")
			{
				//Is valid add
				NPCs.AddUnique(QuestIn.TalkTasks[StartNodes[i].TaskID].TalkNPC);
			}			
		}
	}
		
	return NPCs;
}

TArray<int> UQuestEditorDevFunctionLibrary::GetInitialPseudoTasksOfQuest(const FQuest& QuestIn)
{
	TArray <int> SlotsInitial = TArray <int>();
	FNPCQuestDef NPCDefLocal = FNPCQuestDef();

	for (int i = 0; i < QuestIn.Tasks.Num(); i++)
	{
		//Is start node?
		if (QuestIn.Tasks[i].SuperiorNodes.Num() == 0)
		{
			//Is activate cond only?
			if (UQuestsFunctionLibrary::TaskIsActivateConditionOnly(QuestIn, i))
			{
				SlotsInitial.AddUnique(i);
			}
		}		
	}
	return SlotsInitial;
}

TArray<int> UQuestEditorDevFunctionLibrary::GetStartingTalkTaskForNPC(const FQuest& QuestIn, const FString& NPCNameIn)
{
	TArray <int> SlotsInitial = TArray <int>();
	FNPCQuestDef NPCDefLocal = FNPCQuestDef();

	//Search all tasks that are start nodes and cond only
	for (int i = 0; i < QuestIn.TalkTasks.Num(); i++)
	{
		//Get npc name
		NPCDefLocal = UQuestsFunctionLibrary::NPCQuestDefRepToNPCQuestDef(QuestIn.TalkTasks[i].TalkNPC);

		//Is starting node and for this specific NPC
		if (QuestIn.TalkTasks[i].SuperiorNodes.Num() == 0 && (NPCDefLocal.NPCName == NPCNameIn))
		{
			SlotsInitial.Add(i);
		}
	}
	return SlotsInitial;
}

TArray<int> UQuestEditorDevFunctionLibrary::GetStartingPseudoTalkTaskForNPC(const FQuest& QuestIn, const FString& NPCNameIn)
{
	//Propagates activation of condition only tasks to get the state of the quest once all are autocompleted and gets the talktasks that this nodes activate.

	//------------------------------------------Obtain  all tasks that are starting and activate condition only
	TArray <int> StartACOTasksCompleted = GetInitialPseudoTasksOfQuest(QuestIn);

	if (StartACOTasksCompleted.Num() == 0) return TArray <int>();

	//---------------------------------------get all initial inferior nodes
	TArray <FQuestTaskID> InferiorNodesA;
	TArray <FQuestTaskID> InferiorNodesB;
	TArray <FQuestTaskID> AnalizedNodes;

	//Talk task pseudo initial (they are initial after ACO nodes completed)
	TArray <int> TalkPseudoInitialNodesOut;

	FQuestTaskID TaskIDLocal;
	TaskIDLocal.TaskType = EQuestTaskType::Task;

	//Add to array all inferior nodes of ACO that are "completed" on start
	for (int i = 0; i < StartACOTasksCompleted.Num(); i++)
	{
		TaskIDLocal.TaskID = StartACOTasksCompleted[i];
		//Temporary save all initial ACO nodes on ID form
		InferiorNodesB.AddUnique(TaskIDLocal);
		//Add all to initial pool
		AnalizedNodes.Append(UQuestsFunctionLibrary::ConvertLinkArrayToNodeArray(QuestIn.Tasks[StartACOTasksCompleted[i]].InferiorNodes));
	}

	//filter to get all unique inferior nodes no duplicates
	for (int i = 0; i < AnalizedNodes.Num(); i++)
	{
		InferiorNodesA.AddUnique(AnalizedNodes[i]);
	}
	//Save all ACO nodes used previously. 
	//Dont care about repeated nodes here
	AnalizedNodes = InferiorNodesB;

	//Clear Bnodes
	InferiorNodesB.Empty();
	FNPCQuestDef NPCDefLocal = FNPCQuestDef();

	bool WhileEnable = true;
	while (WhileEnable)
	{
		//Check all inferior A nodes
		//talk node
		for (int i = 0; i < InferiorNodesA.Num(); i++)
		{
			//Talk task
			if (InferiorNodesA[i].TaskType == EQuestTaskType::TalkTask)
			{
				//Get npcdef
				NPCDefLocal = UQuestsFunctionLibrary::NPCQuestDefRepToNPCQuestDef(QuestIn.TalkTasks[InferiorNodesA[i].TaskID].TalkNPC);				
				
				//Is this npc
				if (NPCDefLocal.NPCName == NPCNameIn)
				{					
					//if it can potentially be a pseudo start (make sure all superior nodes are conditions)
					if (UQuestsFunctionLibrary::AreAllSuperiorNodesOfNodeConditions(QuestIn, InferiorNodesA[i]))
					{
						//Add to array out of initial talk task	
						TalkPseudoInitialNodesOut.AddUnique(InferiorNodesA[i].TaskID);
					}					
				}					
			}

			//ActivateConditionOnly tasks (no spawn task, only check)
			else if (UQuestsFunctionLibrary::TaskIsActivateConditionOnly(QuestIn, InferiorNodesA[i].TaskID))
			{
				//Asumes it can always activate
				//add to array of already analized 
				AnalizedNodes.Add(InferiorNodesA[i]);
				//add  INFERIOR nodes to arrayB to check in next while loop (propagation)				
				InferiorNodesB.Append(UQuestsFunctionLibrary::ConvertLinkArrayToNodeArray(QuestIn.Tasks[InferiorNodesA[i].TaskID].InferiorNodes));
			}
		}

		//If B is empty there is nothing more to check
		if (InferiorNodesB.Num() == 0) WhileEnable = false;
		//Need to clear A, load with B and clear B
		else
		{
			InferiorNodesA = InferiorNodesB;
			InferiorNodesB.Empty();
		}
	}

	//Filter unique only
	TArray<int> FilteredNodes;

	for (int i = 0; i < TalkPseudoInitialNodesOut.Num(); i++)
	{
		FilteredNodes.AddUnique(TalkPseudoInitialNodesOut[i]);
	}

	return FilteredNodes;
}

bool UQuestEditorDevFunctionLibrary::NPCCanGiveQuest(const FQuest& QuestIn, const FString& NPCNameIn)
{
	//If has a direct talk for this NPC can give this quest
	TArray<int> StartTalks = GetStartingTalkTaskForNPC(QuestIn, NPCNameIn);
	if (StartTalks.Num() > 0) return true;

	//If has a pseudo talk for this NPC can give this quest
	StartTalks = GetStartingPseudoTalkTaskForNPC(QuestIn, NPCNameIn);
	if (StartTalks.Num() > 0) return true;
	else
	{
		UE_LOG(QuestEditorDevLog, Log, TEXT("UQuestEditorDevFunctionLibrary::NPCCanGiveQuest - Could not find a Pseudo start Talk Task node for npc %s."), *NPCNameIn);
		return false;
	}		

	return false;
}

TArray<FQuestRep> UQuestEditorDevFunctionLibrary::GetAllQuestsInDTForNPC(TSoftObjectPtr<UDataTable> Datatable, const FString& NPCNameIn)
{
	UDataTable* DTLocal = UQuestsFunctionLibrary::GetDTFromPath(Datatable);
	
	if (!DTLocal) return TArray<FQuestRep>();
	
	TArray<FQuestRep> LocalQuests = TArray<FQuestRep>(); 
		
	FQuest* QuestPointer = nullptr;
	FQuest QuestLocal = FQuest();
	FQuestRep QuestRepOut = FQuestRep();

	//Get row names
	TArray <FName> RowNames = DTLocal->GetRowNames();

	//Iterate through DT names
	for (int j = 0; j < RowNames.Num(); j++)
	{
		QuestPointer = DTLocal->FindRow<FQuest>(RowNames[j], FString(), false);
		if (QuestPointer != nullptr)
		{
			QuestLocal = *QuestPointer;

			//This NPC can give this quest?
			if (NPCCanGiveQuest(QuestLocal, NPCNameIn))
			{
				//Create the rep variable
				QuestRepOut.QuestDTName = RowNames[j];
				QuestRepOut.Datatable = DTLocal->GetPathName();

				//Save
				LocalQuests.Add(QuestRepOut);
			}
		}
	}
	
	return LocalQuests;
}

UWorld* UQuestEditorDevFunctionLibrary::GetQuestsGameWorld()
{	
	if (GEditor)
	{
		if (FWorldContext* WorldContext = GEditor->GetPIEWorldContext())
		{
			return WorldContext->World();
		}

		return nullptr;
	}

	return GWorld;
}

TArray<FQuestRep> UQuestEditorDevFunctionLibrary::GetAllSuperiorQuestsOfQuest(const FQuest& QuestIn)
{
	TArray<FQuestRep> QuestsOut = TArray<FQuestRep>();
	for (int i = 0; i < QuestIn.Tasks.Num(); i++)
	{
		//Is starting node
		if (QuestIn.Tasks[i].SuperiorNodes.Num() == 0)
		{
			//Is a Has Quest Completed Task?
			if (QuestIn.Tasks[i].TaskClass.Get()->IsChildOf(UTask_HasQuestCompleted::StaticClass()))
			{
				//ConvertToQuestRep and add to out array
				QuestsOut.AddUnique(UQuestsFunctionLibrary::StringToQuestRep(QuestIn.Tasks[i].Payload.StringA));
			}
			else if (QuestIn.Tasks[i].TaskClass.Get()->IsChildOf(UTask_HasQuestCompletedWithEnding::StaticClass()))
			{
				FQuestNode CurrentQuestNode = UQuestsFunctionLibrary::StringToQuestNode(QuestIn.Tasks[i].Payload.StringA);
				//ConvertToQuestRep and add to out array
				QuestsOut.AddUnique(CurrentQuestNode.Quest);
			}
		}
	}

	return QuestsOut;
}

TArray<FQuestRep> UQuestEditorDevFunctionLibrary::GetAllInferiorQuestsOfQuestRep(const FQuestRep& QuestRepIn, TArray <UDataTable*> DatatablesIn)
{

	TArray<FQuestRep> QuestsOut = TArray<FQuestRep>();
	TArray<FQuestRep> SuperiorQuests = TArray<FQuestRep>();

	//Convert quest to quest rep
	FQuest QuestIn = UQuestsFunctionLibrary::QuestRepToQuest(QuestRepIn);

	FQuest* QuestPointer = nullptr;
	FQuest QuestLocal = FQuest();
	FQuestRep QuestRepLocal = FQuestRep();

	//Need to search all dts (sadly)	
	TArray<FName> RowNamesCurrent = TArray<FName>();
	for (int i = 0; i < DatatablesIn.Num(); i++)
	{
		//Get DT rows
		RowNamesCurrent = DatatablesIn[i]->GetRowNames();

		//Iterate through DT names
		for (int j = 0; j < RowNamesCurrent.Num(); j++)
		{
			QuestPointer = DatatablesIn[i]->FindRow<FQuest>(RowNamesCurrent[j], FString(), false);
			
			if (QuestPointer != nullptr)
			{
				QuestLocal = *QuestPointer;
				//If is different quest name
				if (QuestLocal.QuestName.ToString() != QuestIn.QuestName.ToString())
				{
					//All quests that have the QuestIn as superior, are inferior quests of QuestIn
					SuperiorQuests = GetAllSuperiorQuestsOfQuest(QuestLocal);
					
					//If questin is contained, this quest is connected to it.
					if (SuperiorQuests.Contains(QuestRepIn))
					{
						QuestRepLocal.QuestDTName = RowNamesCurrent[j];
						QuestRepLocal.Datatable = DatatablesIn[i];
						QuestsOut.AddUnique(QuestRepLocal);
					}					
				}
			}
		}	
	}	
	
	return QuestsOut;
}

ENodeLinkType UQuestEditorDevFunctionLibrary::GetLinkTypeBetweenConnectedQuests(const FQuestRep& SuperiorQuestIn, const FQuestRep& InferiorQuestIn)
{
	ENodeLinkType CurrentLinkType = ENodeLinkType::Required;
	bool FoundALinkToQuest = false;
	FQuest InferiorQuest = UQuestsFunctionLibrary::QuestRepToQuest(InferiorQuestIn);
	TArray<FQuestTaskID> LocalNodes = TArray<FQuestTaskID>();

	//If there is a START (no pseudo) node, then the link is optional because there is a way to skip the condition
	//I am not condisering this way for now, start nodes are just attached. 
	LocalNodes = GetQuestNonConditionalStartNodes(InferiorQuest);
	if (LocalNodes.Num() > 0) return ENodeLinkType::Optional;

	//If all Pseudo start nodes of inferior quest require the superior quest to start and the link between condition and pseudo start is required, then the link is required, otherwise is optional.
	LocalNodes = UQuestEditorDevFunctionLibrary::GetQuestPseudoStartNodes(InferiorQuest);
	TArray<FQuestTaskLink> SuperiorCurrentLinks = TArray<FQuestTaskLink>();

	//Assume it has a required link to quest
	bool CurrentHasRequiredLink = true;

	FQuestRep CurrentTaskQuest = FQuestRep();

	//for each Pseudo Start Node
	for (int i = 0; i < LocalNodes.Num(); i++)
	{
		//If all pseudo start nodes have a REQUIRED connection to a "HasQuestCompleted" node with the Quest involved beign the Superior Quest the Connection between quests is REQUIREd. OTERWHISE IS OPTIONAL

		//Get The superior Links
		SuperiorCurrentLinks = UQuestsFunctionLibrary::GetNodeSuperiorLinksInQuest(InferiorQuest, LocalNodes[i]);

		//Search all links. Needs to find a HasQuestCompleted with required link
		if (AreGroupOfNodesConectedToQuest(SuperiorQuestIn, InferiorQuestIn, SuperiorCurrentLinks, CurrentLinkType))
		{
			//If the link is optional needs to break
			if (CurrentLinkType == ENodeLinkType::Optional)
			{
				//Update link type							
				CurrentLinkType = ENodeLinkType::Optional;
				//Stop looping
				break;
			}
			FoundALinkToQuest = true;
		}
		else
		{
			//if finds a unconnected node to a quest then the link is optional, you can access the quest via this group of nodes without the quest
			CurrentLinkType = ENodeLinkType::Optional;
			break;
		}

		//If it was found a optional link, the operation is cancelled.
		if (CurrentLinkType == ENodeLinkType::Optional)
		{
			break;
		}
	}

	return CurrentLinkType;
}

bool UQuestEditorDevFunctionLibrary::AreGroupOfNodesConectedToQuest(const FQuestRep& SuperiorQuestIn, const FQuestRep& InferiorQuestIn, const TArray<FQuestTaskLink>& NodeLinksIn, ENodeLinkType& LinkTypeOut)
{
	//Assume required
	ENodeLinkType CurrentLinkType = ENodeLinkType::Required;
	bool FoundALinkToQuest = false;

	
	FQuestRep CurrentTaskQuest = FQuestRep();
	
	//Search all links
	for (int j = 0; j < NodeLinksIn.Num(); j++)
	{
		//If the link is connected connected to quest
		if (ConditionalTaskIsLinkedToQuest(SuperiorQuestIn, InferiorQuestIn, NodeLinksIn[j].Node, CurrentTaskQuest))
		{
			//If link is optional needs to break and cancel operation no need to continue
			if (NodeLinksIn[j].LinkType == ENodeLinkType::Optional)
			{
				//Update link type							
				CurrentLinkType = ENodeLinkType::Optional;
				FoundALinkToQuest = true;
				//Stop looping
				break;
			}
			//If link is required then this loops needs to continue with next node.
			else
			{				
				FoundALinkToQuest = true;
				continue;
			}
		}
	}

	LinkTypeOut = CurrentLinkType;

	return FoundALinkToQuest;
}

bool UQuestEditorDevFunctionLibrary::ConditionalTaskIsLinkedToQuest(const FQuestRep& SuperiorQuestIn, const FQuestRep& InferiorQuestIn, const FQuestTaskID& NodeIn, FQuestRep& QuestOut)
{
	FQuest InferiorQuestLocal = UQuestsFunctionLibrary::QuestRepToQuest(InferiorQuestIn);
	FQuestRep CurrentTaskQuest = FQuestRep();
	bool IsLinked = false;

	//If node is a talk task then the node is not pseudo and that should never happen.
	if (NodeIn.TaskType == EQuestTaskType::TalkTask)
	{
		//Log
		UE_LOG(QuestEditorDevLog, Error, TEXT("UQuestEditorDevFunctionLibrary::AreGroupOfNodesConectedToQuest - Pseudo start node has superior talk task! Quest: %s."), *InferiorQuestLocal.QuestName.ToString());

		//Stop looping
		return false;
	}
	//If is task (Check is extra but is fine)
	else if (NodeIn.TaskType == EQuestTaskType::Task)
	{
		//If node is not a conditional only, then this is a error and should never happen. This node is not Pseudo.
		if (!UQuestsFunctionLibrary::TaskIsActivateConditionOnly(InferiorQuestLocal, NodeIn.TaskID))
		{
			//Log
			UE_LOG(QuestEditorDevLog, Error, TEXT("UQuestEditorDevFunctionLibrary::AreGroupOfNodesConectedToQuest - Pseudo start node has superior non conditional nodes! Quest: %s."), *InferiorQuestLocal.QuestName.ToString());
			//Stop looping
			return false;
		}
		//For activate condition only.
		else
		{
			//task is "HasQuestCompleted"
			if (InferiorQuestLocal.Tasks[NodeIn.TaskID].TaskClass.Get()->IsChildOf(UTask_HasQuestCompleted::StaticClass()))
			{
				//Check if the quest in the payload is the superior one. StringA stores de quest in this type of task
				CurrentTaskQuest = UQuestsFunctionLibrary::StringToQuestRep(InferiorQuestLocal.Tasks[NodeIn.TaskID].Payload.StringA);
				//Is this the superior Quest?
				if (CurrentTaskQuest == SuperiorQuestIn)
				{
					//Same quest is linked
					IsLinked = true;
				}
			}
			//Task is "HasQuestCompletedWithEnding"
			else if (InferiorQuestLocal.Tasks[NodeIn.TaskID].TaskClass.Get()->IsChildOf(UTask_HasQuestCompletedWithEnding::StaticClass()))
			{
				//Check if the quest in the payload is the superior one. StringA stores de quest in this type of task as a node
				FQuestNode CurrentQuestNode = UQuestsFunctionLibrary::StringToQuestNode(InferiorQuestLocal.Tasks[NodeIn.TaskID].Payload.StringA);
				CurrentTaskQuest = CurrentQuestNode.Quest;
				//Is this the superior Quest?
				if (CurrentTaskQuest == SuperiorQuestIn)
				{
					//Same quest is linked
					IsLinked = true;
				}
			}
			//If none of this previous tasks then returns false
		}
	}

	//Out vars
	QuestOut = CurrentTaskQuest;

	return IsLinked;
}

FQuestTask UQuestEditorDevFunctionLibrary::ConvertQuestRepToHierarchyTask(const FQuestRep& QuestIn)
{
	//This task will represent the quest
	FQuestTask TaskOut = FQuestTask();
	
	//StringA of payload holds the quest as string
	TaskOut.Payload.StringA = UQuestsFunctionLibrary::QuestRepToString(QuestIn);
	
	return TaskOut;
}

FQuestTask UQuestEditorDevFunctionLibrary::CreateDirectStartHierarchyTask()
{
	//This task will represent the quest
	FQuestTask TaskOut = FQuestTask();

	//StringB of payload holds a string reference. A is not used to avoid problems.
	TaskOut.Payload.StringB = "DirectStart";

	return TaskOut;
}

TArray <FQuestTaskID> UQuestEditorDevFunctionLibrary::AddHierarchyOfNodeToQuest(UPARAM(ref)FQuest& HierarchyQuestIn, const FQuestTaskID& QuestRepNodeIn, int SuperiorNodeTier, bool PropagateUp, TArray <UDataTable*> DatatablesIn)
{
	TArray<FQuestTask> HierarchyTasks = TArray<FQuestTask>();

	//Task Node is valid in quest in?
	if (!UQuestsFunctionLibrary::IsTaskIDValidInQuest(HierarchyQuestIn, QuestRepNodeIn)) return TArray <FQuestTaskID>();

	//----------------------------------------------------------------------------------------------
	//-----------------------------------------------Get Quest In From Task Node
	//----------------------------------------------------------------------------------------------	

	//The Quest Node that indicates a quest. Needs to find the quest based on the task.
	FQuestRep QuestRepIn = FQuestRep();		

	//StringA stores the quest in hierarchy nodes.
	QuestRepIn = UQuestsFunctionLibrary::StringToQuestRep(HierarchyQuestIn.Tasks[QuestRepNodeIn.TaskID].Payload.StringA);
	FQuest QuestIn = UQuestsFunctionLibrary::QuestRepToQuest(QuestRepIn);

	//----------------------------------------------------------------------------------------------
	//-----------------------------------------------Get Quest Superior Quest Nodes
	//----------------------------------------------------------------------------------------------
	TArray<FQuestRep> SuperiorQuests = TArray<FQuestRep>();
	//Keep a track of links between quests
	TArray<ENodeLinkType> SuperiorLinkTypes = TArray<ENodeLinkType>();

	//---------------------------------------Normal superior nodes
	if(PropagateUp) SuperiorQuests = GetAllSuperiorQuestsOfQuest(QuestIn);
	else SuperiorQuests = GetAllInferiorQuestsOfQuestRep(QuestRepIn, DatatablesIn);

	//There is a StartNode? Insert a empty quest in position 0 to maintain indexes
	bool QuestHasDirectStartNode = QuestRequiresDirectStartNode(QuestIn);
	if (PropagateUp && QuestHasDirectStartNode) SuperiorQuests.Insert(FQuestRep(), 0);
			   
	//GridValues, set to variables to easy customize
	int XOffset = 300;
	int YOffset = 300;
	//Editor mid value (take node size into account)
	int XMid = 24655;
	int YMid = 25000;
	   	 
	//----------------------------------------------------------------------------------------------
	//-----------------------------------------------CONVERT TO TASKS
	//----------------------------------------------------------------------------------------------
	//Convert to task each of the quests and adjust position

	//I use this value a lot, calculate once
	int SuperiorTasksNum = SuperiorQuests.Num();

	int AmountOfNodesInTierSoFar = GetAmountOfNodesInHierarchyTier(HierarchyQuestIn, SuperiorNodeTier);

	//-------------------------Superior Tasks
	for (int i = 0; i < SuperiorTasksNum; i++)
	{
		//Propagates and this is the first node and there is a start node
		if (PropagateUp && i == 0 && QuestHasDirectStartNode)
		{
			//Add quest node for direct start
			HierarchyTasks.Add(CreateDirectStartHierarchyTask());
		}
		else
		{
			//Convert Quest to task
			HierarchyTasks.Add(ConvertQuestRepToHierarchyTask(SuperiorQuests[i]));
		}

		//Set node tier in X and Y
		//X needs to take into account nodes that are already placed from previous operations
		HierarchyTasks[i].Payload.VectorInvolved.X = AmountOfNodesInTierSoFar + i;
		//The Y tier is the one given as input
		HierarchyTasks[i].Payload.VectorInvolved.Y = SuperiorNodeTier;

		//Propagates and this is the first node and there is a start node
		if (PropagateUp && i == 0 && QuestHasDirectStartNode)
		{
			//Add task to hierarchy tasks for grid align
			SuperiorLinkTypes.Add(ENodeLinkType::Optional);
		}
		else
		{
			//Track Links
			SuperiorLinkTypes.Add(GetLinkTypeBetweenConnectedQuests(SuperiorQuests[i], QuestRepIn));
		}		
	}

	//----------------------------------------------------------------------------------------------
	//-----------------------------------------------ADD SUPERIOR TASKS TO QUEST
	//----------------------------------------------------------------------------------------------
	//Array to hold all nodes added to quest
	TArray <FQuestTaskID> AddedTasksIDS = TArray <FQuestTaskID>();

	//Local Current ID
	FQuestTaskID TaskIDOfCurrentTask = FQuestTaskID();
	TaskIDOfCurrentTask.TaskType = EQuestTaskType::Task;

	for (int i = 0; i < HierarchyTasks.Num(); i++)
	{
		//Add Node To Quest
		HierarchyQuestIn.Tasks.Add(HierarchyTasks[i]);

		//Save ID of node for further linking. Since is added, the ID is the num -1
		TaskIDOfCurrentTask.TaskID = HierarchyQuestIn.Tasks.Num()-1;

		AddedTasksIDS.Add(TaskIDOfCurrentTask);
	}

	//----------------------------------------------------------------------------------------------
	//-----------------------------------------------LINK TASKS
	//----------------------------------------------------------------------------------------------

	//----------------------Link all new nodes with the quest node.
	for (int i = 0; i < AddedTasksIDS.Num(); i++)
	{
		//Create link
		if (PropagateUp) CreateNewLinkBetweenNodes(HierarchyQuestIn, QuestRepNodeIn, AddedTasksIDS[i], SuperiorLinkTypes[i]);
		else  CreateNewLinkBetweenNodes(HierarchyQuestIn, AddedTasksIDS[i], QuestRepNodeIn, SuperiorLinkTypes[i]);
	}

	//Remove direct access node if needed to avoid this node propagation
	if (PropagateUp && QuestHasDirectStartNode) AddedTasksIDS.RemoveAt(0);

	return AddedTasksIDS;
}

void UQuestEditorDevFunctionLibrary::AddInitialHierarchyOfQuest(UPARAM(ref)FQuest& HierarchyQuestIn, const FQuestRep& QuestRepIn, TArray <UDataTable*> DatatablesIn)
{
	FQuestTask HierarchyTask =FQuestTask();

	//----------------------------------------------------------------------------------------------
	//-----------------------------------------------CONVERT TO TASKS
	//----------------------------------------------------------------------------------------------
	//Convert to task each of the quests and adjust position

	//------------------------Quest Itself

	//Convert Quest to task
	HierarchyTask = ConvertQuestRepToHierarchyTask(QuestRepIn);

	//Set tiers to 0 0 for initial quest
	HierarchyTask.Payload.VectorInvolved.X = 0;
	HierarchyTask.Payload.VectorInvolved.Y = 0;

	//Add task to hierarchy quest
	HierarchyQuestIn.Tasks.Add(HierarchyTask);

	return;
}

void UQuestEditorDevFunctionLibrary::PropagateQuestsInHierarchy(UPARAM(ref)FQuest& HierarchyQuestIn, const FQuestRep& QuestRepIn, int MaxSuperiorTier, bool PropagateUp, TArray <UDataTable*> DatatablesIn)
{
	//Propagates up the quests hierarchy

	//---------------------------------------get all initial inferior nodes
	//Prepare variables
	TArray <FQuestTaskID> SuperiorNodesA = TArray <FQuestTaskID>();
	TArray <FQuestTaskID> SuperiorNodesB = TArray <FQuestTaskID>();
	TArray <FQuestTaskID> AnalizedNodes = TArray <FQuestTaskID>();
	
	//Update analized nodes
	FQuestTaskID TaskIDCurrent = FQuestTaskID();
	TaskIDCurrent.TaskType = EQuestTaskType::Task;

	//The quest is always the task 0 in hierarchy.
	TaskIDCurrent.TaskID = 0;
		
	//Prepare for first loop
	SuperiorNodesA.Add(TaskIDCurrent);
	TArray <FQuestTaskID> CurrentSuperiorNodes = TArray <FQuestTaskID>();


	int CurrentNodesTier = 1;

	bool WhileEnable = true;
	while (WhileEnable)
	{
		//Check all Superior A nodes

		for (int i = 0; i < SuperiorNodesA.Num(); i++)
		{
			//if quests has not been analized
			if (!AnalizedNodes.Contains(SuperiorNodesA[i]))
			{
				//--------------------------Add all superior nodes as tasks to quest 
				if (PropagateUp) CurrentSuperiorNodes = AddHierarchyOfNodeToQuest(HierarchyQuestIn, SuperiorNodesA[i], CurrentNodesTier, PropagateUp, DatatablesIn);
				//Use negative tiers when propagating down
				else CurrentSuperiorNodes = AddHierarchyOfNodeToQuest(HierarchyQuestIn, SuperiorNodesA[i], -CurrentNodesTier, PropagateUp, DatatablesIn);

				//-------------------------Add current quest to analized
				AnalizedNodes.Add(SuperiorNodesA[i]);

				//-------------------------Add superior nodes of each to the arrayB for further looping
				SuperiorNodesB.Append(CurrentSuperiorNodes);
			}			
		}

		//Increment tier
		CurrentNodesTier++;
		
		//If there is a limit of tiers, check that there is still tiers availables
		if (CurrentNodesTier >= MaxSuperiorTier && MaxSuperiorTier>0) WhileEnable = false;
		else
		{
			//If B is empty there is nothing more to check
			if (SuperiorNodesB.Num() == 0) WhileEnable = false;
			//Need to clear A, load with B and clear B
			else
			{
				SuperiorNodesA = SuperiorNodesB;
				SuperiorNodesB.Empty();
			}
		}
	}

	return;
}

FQuest UQuestEditorDevFunctionLibrary::GetQuestHierarchy(const FQuestRep& QuestRepIn, TArray <UDataTable*> DatatablesIn, int MaxSuperiorTier, int MaxInferiorTier)
{
	//Local hierarchy quest
	FQuest QuestOut = FQuest();
	QuestOut.QuestName = UQuestsFunctionLibrary::QuestRepToQuest(QuestRepIn).QuestName;

	//Add quest node
	AddInitialHierarchyOfQuest(QuestOut, QuestRepIn, DatatablesIn);
	
	//Propagate inferior nodes and add them
	PropagateQuestsInHierarchy(QuestOut, QuestRepIn, MaxInferiorTier, false, DatatablesIn);

	//Propagate superior nodes and add them
	PropagateQuestsInHierarchy(QuestOut, QuestRepIn, MaxSuperiorTier, true, DatatablesIn);
		
	//Filter repetitions
	FilterRepeatedNodesInHierarchy(QuestOut);

	//Collpase tiers
	CollapseXTiers(QuestOut);

	//Adjust nodes positions (X only matters for tier with most nodes, Y matters for all nodes)
	SetAllNodePositionsInHierarchy(QuestOut);

	//Adjust positions in X to avoid overlap in same tier
	AdjustAllTierPositionsToAvoidOverlap(QuestOut);

	return QuestOut;
}

int UQuestEditorDevFunctionLibrary::GetAmountOfNodesInHierarchyTier(const FQuest& QuestIn, int YTierIn)
{
	int AmountNodes = 0;

	for (int i = 0; i < QuestIn.Tasks.Num(); i++)
	{
		//Node is of this tier?
		if (QuestIn.Tasks[i].Payload.VectorInvolved.Y == YTierIn)
		{
			AmountNodes++;
		}
	}
	return AmountNodes;
}

int UQuestEditorDevFunctionLibrary::GetMaxAmountOfNodesInHierarchyTiers(const FQuest& QuestIn, int& TierOut)
{
	//get extremes
	int MaxTier = GetMaxHierarchyYTier(QuestIn);
	int MinTier = GetMinHierarchyYTier(QuestIn);
	int MaxNodes = 0;
	int CurrentMax = 0;
	int TierCurrent = 0;
	for (int i = MinTier; i < MaxTier +1; i++)
	{
		CurrentMax = GetAmountOfNodesInHierarchyTier(QuestIn, i);
		if (CurrentMax > MaxNodes)
		{
			MaxNodes = CurrentMax;
			TierCurrent = i;
		}
	}
	TierOut = TierCurrent;
	return MaxNodes;
}

int UQuestEditorDevFunctionLibrary::GetMaxHierarchyYTier(const FQuest& QuestIn)
{
	int TierExtreme = 0;

	for (int i = 0; i < QuestIn.Tasks.Num(); i++)
	{
		if (QuestIn.Tasks[i].Payload.VectorInvolved.Y > TierExtreme)
		{
			TierExtreme = QuestIn.Tasks[i].Payload.VectorInvolved.Y;
		}
	}
	return TierExtreme;
}

int UQuestEditorDevFunctionLibrary::GetMinHierarchyYTier(const FQuest& QuestIn)
{
	int TierExtreme = 0;

	for (int i = 0; i < QuestIn.Tasks.Num(); i++)
	{
		if (QuestIn.Tasks[i].Payload.VectorInvolved.Y < TierExtreme)
		{
			TierExtreme = QuestIn.Tasks[i].Payload.VectorInvolved.Y;
		}
	}
	return TierExtreme;
}

void UQuestEditorDevFunctionLibrary::SetNodePositionByTierInHierarchy(UPARAM(ref)FQuest& HierarchyQuestIn, FQuestTaskID NodeIn, int MaxTaskInAnyTier)
{
	//Talktasks should not be allowed
	if (NodeIn.TaskType == EQuestTaskType::TalkTask) return;


	//Editor mid value (take node size into account)
	int XMid = 24655;
	int YMid = 25000;

	int TierX = HierarchyQuestIn.Tasks[NodeIn.TaskID].Payload.VectorInvolved.X;
	int TierY = HierarchyQuestIn.Tasks[NodeIn.TaskID].Payload.VectorInvolved.Y;

	int CurrentNodesInTier = GetAmountOfNodesInHierarchyTier(HierarchyQuestIn, TierY);

	//GridValues, set to variables to easy customize
	int XOffset = GetHierarchyAdjustOffset();
	int YOffset = 300;
	
	//Adjust Position of task based on a fixed value
	if (CurrentNodesInTier == 0) return;

	HierarchyQuestIn.Tasks[NodeIn.TaskID].QuestEditorPosition.X = XMid - ((MaxTaskInAnyTier)*XOffset / 2) + XOffset * (TierX);
	HierarchyQuestIn.Tasks[NodeIn.TaskID].QuestEditorPosition.Y = YMid - YOffset * TierY;
	return;
}

void UQuestEditorDevFunctionLibrary::AdjustAllNodePositionsInTierByConnectedNodesInHierarchy(UPARAM(ref)FQuest& HierarchyQuestIn, int TierIn, bool UseInferiorNodes)
{
	TArray <int> CurrentConnectedNodes = TArray <int>();
	float AveragePosition = 0;
	
	//For each Task
	for (int j = 0; j < HierarchyQuestIn.Tasks.Num(); j++)
	{
		//Is node from this Y tier?
		if (HierarchyQuestIn.Tasks[j].Payload.VectorInvolved.Y == TierIn)
		{
			//Get array of nodes IDS
			CurrentConnectedNodes.Empty();
			if (UseInferiorNodes)
			{
				//Inferior nodes of node
				CurrentConnectedNodes = GetInferiorNodesOfNodeWithImmediateHierarchyTier(HierarchyQuestIn, j);
			}
			else
			{
				//Superior nodes of node
				CurrentConnectedNodes = GetSuperiorNodesOfNodeWithImmediateHierarchyTier(HierarchyQuestIn, j);
			}
							
			//Has inferior nodes?
			if (CurrentConnectedNodes.Num() > 0)
			{
				//Calculate average position of all inferior nodes
				AveragePosition = GetNodesAverageXPositionInHierarchy(HierarchyQuestIn, CurrentConnectedNodes);
				//Set node position to average of inferior nodes.
				HierarchyQuestIn.Tasks[j].QuestEditorPosition.X = AveragePosition;
			}
		}
	}
	
	return;
}

TArray<int> UQuestEditorDevFunctionLibrary::GetInferiorNodesOfNodeWithImmediateHierarchyTier(const FQuest& HierarchyQuestIn, int NodeID)
{
	int NodeYHierarchy = HierarchyQuestIn.Tasks[NodeID].Payload.VectorInvolved.Y;
	int CurrentID = 0;
	TArray<int> NodesOut = TArray<int>();

	for (int i = 0; i < HierarchyQuestIn.Tasks[NodeID].InferiorNodes.Num(); i++)
	{
		//Immediate inferior hierarchy
		CurrentID = HierarchyQuestIn.Tasks[NodeID].InferiorNodes[i].Node.TaskID;
		//Inferior tier?
		if (HierarchyQuestIn.Tasks[CurrentID].Payload.VectorInvolved.Y == NodeYHierarchy-1)
		{
			NodesOut.Add(CurrentID);
		}
	}

	return NodesOut;
}

TArray<int> UQuestEditorDevFunctionLibrary::GetSuperiorNodesOfNodeWithImmediateHierarchyTier(const FQuest& HierarchyQuestIn, int NodeID)
{
	int NodeYHierarchy = HierarchyQuestIn.Tasks[NodeID].Payload.VectorInvolved.Y;
	int CurrentID = 0;
	TArray<int> NodesOut = TArray<int>();

	for (int i = 0; i < HierarchyQuestIn.Tasks[NodeID].SuperiorNodes.Num(); i++)
	{
		//Immediate Superior hierarchy
		CurrentID = HierarchyQuestIn.Tasks[NodeID].SuperiorNodes[i].Node.TaskID;
		//Superior tier?
		if (HierarchyQuestIn.Tasks[CurrentID].Payload.VectorInvolved.Y == NodeYHierarchy + 1)
		{
			NodesOut.Add(CurrentID);
		}
	}

	return NodesOut;
}

TArray<int> UQuestEditorDevFunctionLibrary::GetNodesIDsByTierInHierarchy(const FQuest& HierarchyQuestIn, int TierIn)
{
	TArray<int> NodeIDS = TArray <int>();

	for (int i = 0; i < HierarchyQuestIn.Tasks.Num(); i++)
	{
		//This node has same tier?
		if (HierarchyQuestIn.Tasks[i].Payload.VectorInvolved.Y == TierIn)
		{
			NodeIDS.Add(i);
		}
	}

	return NodeIDS;
}

float UQuestEditorDevFunctionLibrary::GetNodesAverageXPositionInHierarchy(const FQuest& HierarchyQuestIn, TArray <int> NodesIn)
{
	//no nodes returns 0
	if (NodesIn.Num() <= 0)
	{
		return 0.0f;
	}		

	float TotalX = 0.0f;

	for (int i = 0; i < NodesIn.Num(); i++)
	{
		//add total X
		TotalX = TotalX + HierarchyQuestIn.Tasks[NodesIn[i]].QuestEditorPosition.X;
	}

	//Calculate average
	TotalX = TotalX / (NodesIn.Num());
	return TotalX;
}

TArray <int> UQuestEditorDevFunctionLibrary::GetNodesInTierOverlappingPosition(const FQuest& HierarchyQuestIn, int TierIn)
{
	//Get Nodes
	TArray<int> NodesInTier = GetNodesIDsByTierInHierarchy(HierarchyQuestIn, TierIn);

	//If there is only one node in tier there is no overlapping
	if (NodesInTier.Num() < 1) return TArray <int>();

	TArray<int> OverlappingNodes = TArray<int>();

	float NodeAPosition = 0;
	float NodeBPosition = 0;

	bool FoundOverlap = false;

	//Compare positions in tier for all to all
	for (int i = 0; i < NodesInTier.Num(); i++)
	{
		for (int j = 0; j < NodesInTier.Num(); j++)
		{
			//Different node
			if (i != j)
			{
				//Positions
				NodeAPosition = HierarchyQuestIn.Tasks[NodesInTier[i]].QuestEditorPosition.X;
				NodeBPosition = HierarchyQuestIn.Tasks[NodesInTier[j]].QuestEditorPosition.X;		
							
				//There is overlap? (use a bit less than the offset for round errors in floats)
				if (FMath::Abs(NodeAPosition - NodeBPosition) < (GetHierarchyAdjustOffset()-5))
				{
					//Save the overlapping nodes
					OverlappingNodes.Add(NodesInTier[i]);
					OverlappingNodes.Add(NodesInTier[j]);
					FoundOverlap = true;
					//Break if found overlap
					break;
				}				
			}
		}
		//Break if found overlap
		if (FoundOverlap) break;
	}

	return OverlappingNodes;
}

void UQuestEditorDevFunctionLibrary::AdjustNodePairPositionsToAvoidOverlap(UPARAM(ref)FQuest& HierarchyQuestIn, TArray <int> NodePairIn)
{
	//If this is not a pair leave
	if (NodePairIn.Num() != 2)
	{
		return;
	}		

	TArray <int> NodePairLocal = TArray <int>();

	//Rearrange by X tier, lower tier first
	//If tier of node B is < than tier of A need to swap in array
	//Tier A > Tier B
	if (HierarchyQuestIn.Tasks[NodePairIn[0]].Payload.VectorInvolved.X > HierarchyQuestIn.Tasks[NodePairIn[1]].Payload.VectorInvolved.X)
	{
		//Rearrange local Ids
		NodePairLocal.Add(NodePairIn[1]);
		NodePairLocal.Add(NodePairIn[0]);
	}
	else NodePairLocal = NodePairIn;

	//Adjustment
	float PositionA = HierarchyQuestIn.Tasks[NodePairLocal[0]].QuestEditorPosition.X;
	float PositionB = HierarchyQuestIn.Tasks[NodePairLocal[1]].QuestEditorPosition.X;

	float PositionOffset = GetHierarchyAdjustOffset();
	float PositionDifference = FMath::Abs(PositionB- PositionA);

	TArray <int> SameTierNodesLocal = TArray <int>();

	//If tier is lesser or equal than the half of the max tier nodes, then move left node to the left.
	int TierY = HierarchyQuestIn.Tasks[NodePairLocal[0]].Payload.VectorInvolved.Y;
	int ATierX = HierarchyQuestIn.Tasks[NodePairLocal[0]].Payload.VectorInvolved.X;

	//Get Total max tasks in this tiers
	int MaxTaskInTier = GetAmountOfNodesInHierarchyTier(HierarchyQuestIn, TierY);
	
	//Get Y Tier Nodes
	SameTierNodesLocal = GetNodesIDsByTierInHierarchy(HierarchyQuestIn, TierY);
	   
	bool MoveLeft = true;
	
	//--------------------Can move by position unnocupied
	//Can move left?
	if (!IsNodeNearPositionInHierarchy(HierarchyQuestIn, PositionA - PositionOffset + PositionDifference, TierY))
	{
		MoveLeft = true;
	}
	//Can move right?
	else if (!IsNodeNearPositionInHierarchy(HierarchyQuestIn, PositionB + PositionOffset - PositionDifference, TierY))
	{
		MoveLeft = false;
	}
	//----------------------Move by tier 
	//Is a lower tier of the mid one
	else if (ATierX < MaxTaskInTier / 2)
	{
		MoveLeft = true;	
	}
	//is a high tier compared to mid one
	else
	{
		MoveLeft = false;
	}
	//-------------------------Move nodes
	if (MoveLeft)
	{
		HierarchyQuestIn.Tasks[NodePairLocal[0]].QuestEditorPosition.X = PositionA - PositionOffset + PositionDifference;
	}
	else
	{
		HierarchyQuestIn.Tasks[NodePairLocal[1]].QuestEditorPosition.X = PositionB + PositionOffset - PositionDifference;
	}

	return;
}

void UQuestEditorDevFunctionLibrary::AdjustNodePositionsToAvoidOverlapInTier(UPARAM(ref)FQuest& HierarchyQuestIn, int TierIn)
{
	TArray <int> NodesOverlapping = GetNodesInTierOverlappingPosition(HierarchyQuestIn, TierIn);
	
	int MaxCount = 0;

	//If nodes overlap keep looping
	while (NodesOverlapping.Num() >1 && MaxCount<100)
	{
		//Adjust nodes positions
		AdjustNodePairPositionsToAvoidOverlap(HierarchyQuestIn, NodesOverlapping);

		//Get Overlapping nodes again
		NodesOverlapping = GetNodesInTierOverlappingPosition(HierarchyQuestIn, TierIn);

		MaxCount++;
	}

	if (MaxCount >98) UE_LOG(QuestEditorDevLog, Error, TEXT("UQuestEditorDevFunctionLibrary::AdjustNodePositionsToAvoidOverlapInTier - Max loop count reached on tier %i."), TierIn);
}

void UQuestEditorDevFunctionLibrary::AdjustAllTierPositionsToAvoidOverlap(UPARAM(ref)FQuest& HierarchyQuestIn)
{
	int MinTier = GetMinHierarchyYTier(HierarchyQuestIn);
	int MaxTier = GetMaxHierarchyYTier(HierarchyQuestIn);
	int TierWithMaxNodes = 0;
	int MaxNodes = GetMaxAmountOfNodesInHierarchyTiers(HierarchyQuestIn, TierWithMaxNodes);

	//Adjust nodes for each tier
	//The first tier will not be adjusted. That tier is the base one and is used to calculate the others.

	//---------------------------------------Superior Nodes
	//Nodes that are superior of the tier with max nodes
	
	//Avoid doing anything when the max nodes tier is the max tier
	if (TierWithMaxNodes < MaxTier)
	{
		for (int i = TierWithMaxNodes + 1; i < MaxTier + 1; i++)
		{
			//Adjust positions of nodes to the average of the inferior nodes of the previous tier
			AdjustAllNodePositionsInTierByConnectedNodesInHierarchy(HierarchyQuestIn, i, true);

			//Avoid overlap in the tier
			AdjustNodePositionsToAvoidOverlapInTier(HierarchyQuestIn, i);
		}
	}

	//---------------------------------------Inferior Nodes
	//Go with inferior nodes of tier with max nodes in decreasing tier fashion

	//Avoid doing anything when the max nodes tier is the minimun tier
	if (TierWithMaxNodes > MinTier)
	{
		for (int i = TierWithMaxNodes - 1; i >= MinTier; i--)
		{
			//Adjust positions of nodes to the average of the inferior nodes of the previous tier
			AdjustAllNodePositionsInTierByConnectedNodesInHierarchy(HierarchyQuestIn, i, false);

			//Avoid overlap in the tier
			AdjustNodePositionsToAvoidOverlapInTier(HierarchyQuestIn, i);
		}
	}
}

int UQuestEditorDevFunctionLibrary::GetHierarchyAdjustOffset()
{
	return 350;
}

bool UQuestEditorDevFunctionLibrary::IsNodeNearPositionInHierarchy(const FQuest& HierarchyQuestIn, int PositionXIn, int TierIn)
{
	//Get Nodes
	TArray<int> NodesInTier = GetNodesIDsByTierInHierarchy(HierarchyQuestIn, TierIn);

	bool result = false;

	//Compare positions in tier for all to all
	for (int i = 0; i < NodesInTier.Num(); i++)
	{
		//There is overlap? (use a bit less than the offset for round errors in floats)
		if (FMath::Abs(HierarchyQuestIn.Tasks[NodesInTier[i]].QuestEditorPosition.X - PositionXIn) < (GetHierarchyAdjustOffset() - 5))
		{
			//Found a node
			result = true;
			//Break if found node
			break;
		}
	}

	return result;
}

void UQuestEditorDevFunctionLibrary::SetAllNodePositionsInHierarchy(UPARAM(ref)FQuest& HierarchyQuestIn)
{
	FQuestTaskID LocalNode = FQuestTaskID();
	LocalNode.TaskType = EQuestTaskType::Task;

	//Get Total max tasks in all tiers
	int MaxTier = 0;
	int MaxTaskInTier = GetMaxAmountOfNodesInHierarchyTiers(HierarchyQuestIn, MaxTier);
		
	for (int i = 0; i < HierarchyQuestIn.Tasks.Num(); i++)
	{
		LocalNode.TaskID = i;
		SetNodePositionByTierInHierarchy(HierarchyQuestIn, LocalNode, MaxTaskInTier);
	}
	
	return;
}

void UQuestEditorDevFunctionLibrary::FilterRepeatedNodesInHierarchy(UPARAM(ref)FQuest& HierarchyQuestIn)
{
	TArray <FQuestTaskID> MergedNodes = TArray <FQuestTaskID>();
	FQuestTaskID CurrentNodeB = FQuestTaskID();
	CurrentNodeB.TaskType = EQuestTaskType::Task;
	FQuestTaskID CurrentNodeA = FQuestTaskID();
	CurrentNodeA.TaskType = EQuestTaskType::Task;
	FQuestRep QuestRepNodeA = FQuestRep();
	FQuestRep QuestRepNodeB = FQuestRep();

	//Search all tasks
	for (int i = 0; i < HierarchyQuestIn.Tasks.Num(); i++)
	{
		//If node A is not Direct
		if (HierarchyQuestIn.Tasks[i].Payload.StringB != "DirectStart")
		{
			//A node has not been merged yet?
			CurrentNodeA.TaskID = i;
			if (!MergedNodes.Contains(CurrentNodeA))
			{
				//Compare to all nodes
				for (int j = 0; j < HierarchyQuestIn.Tasks.Num(); j++)
				{
					//Is not the same node?
					if (i != j)
					{
						//if B Is not direct node?(i dont merge direct node)
						if (HierarchyQuestIn.Tasks[j].Payload.StringB != "DirectStart")
						{
							//B node has not been merged yet?
							CurrentNodeB.TaskID = j;
							if (!MergedNodes.Contains(CurrentNodeB))
							{
								//Node is of same quest?
								QuestRepNodeA = UQuestsFunctionLibrary::StringToQuestRep(HierarchyQuestIn.Tasks[i].Payload.StringA);
								QuestRepNodeB = UQuestsFunctionLibrary::StringToQuestRep(HierarchyQuestIn.Tasks[j].Payload.StringA);
								if (QuestRepNodeA == QuestRepNodeB)
								{
									//Swap the connections of the lower tier one to the higher tier on the inferior nodes
									//If i is higher tier than j
									if (HierarchyQuestIn.Tasks[i].Payload.VectorInvolved.Y > HierarchyQuestIn.Tasks[j].Payload.VectorInvolved.Y)
									{
										//---------Swap connections										
										SwapNodeConnections(HierarchyQuestIn, CurrentNodeB, CurrentNodeA, ENodeConnectorType::Inferior);

										//---------Mark as merged
										MergedNodes.AddUnique(CurrentNodeB);

										//---------Maintain the lower X tier in the non merged node
										//If i is higher tier than j
										if (HierarchyQuestIn.Tasks[i].Payload.VectorInvolved.X > HierarchyQuestIn.Tasks[j].Payload.VectorInvolved.X)
										{
											HierarchyQuestIn.Tasks[i].Payload.VectorInvolved.X = HierarchyQuestIn.Tasks[j].Payload.VectorInvolved.X;
										}
									}
									//If j is higher tier than i
									else
									{
										//---------Append connections
										SwapNodeConnections(HierarchyQuestIn, CurrentNodeA, CurrentNodeB, ENodeConnectorType::Inferior);

										//---------Mark as merged
										MergedNodes.AddUnique(CurrentNodeA);

										//---------Maintain the lower X tier in the non merged node
										//If i is higher tier than j
										if (HierarchyQuestIn.Tasks[i].Payload.VectorInvolved.X < HierarchyQuestIn.Tasks[j].Payload.VectorInvolved.X)
										{
											HierarchyQuestIn.Tasks[j].Payload.VectorInvolved.X = HierarchyQuestIn.Tasks[i].Payload.VectorInvolved.X;
										}
									}	
								}
							}
						}
					}
				}
			}			
		}
	}

	//Remove the merged tasks
	RemoveMultipleTasksByID(HierarchyQuestIn, MergedNodes);

	return;
}

void UQuestEditorDevFunctionLibrary::CollapseXTiers(UPARAM(ref)FQuest& HierarchyQuestIn)
{
	int MinTier = GetMinHierarchyYTier(HierarchyQuestIn);
	int MaxTier = GetMaxHierarchyYTier(HierarchyQuestIn);

	int TierCount = 0;
	for (int i = MinTier; i < MaxTier+1; i++)
	{
		//Reset tier counter
		TierCount = 0;

		for (int j = 0; j < HierarchyQuestIn.Tasks.Num(); j++)
		{
			//This node has same tier?
			if (HierarchyQuestIn.Tasks[j].Payload.VectorInvolved.Y == i)
			{
				HierarchyQuestIn.Tasks[j].Payload.VectorInvolved.X = TierCount;
				TierCount++;
			}
		}
	}
	return;
}

bool UQuestEditorDevFunctionLibrary::QuestRequiresDirectStartNode(const FQuest& QuestIn)
{
	TArray <FQuestTaskID> SlotsInitial = TArray <FQuestTaskID>();
	FQuestTaskID LocalID = FQuestTaskID();
	FNPCQuestDef NPCDefLocal = FNPCQuestDef();

	bool HasConditional = false;
	bool HasDirectStart = false;
	//--------------------------Tasks
	LocalID.TaskType = EQuestTaskType::Task;
	for (int i = 0; i < QuestIn.Tasks.Num(); i++)
	{
		//Is start node?
		if (QuestIn.Tasks[i].SuperiorNodes.Num() == 0)
		{
			//Is activate cond only?
			if (UQuestsFunctionLibrary::TaskIsActivateConditionOnly(QuestIn, i))
			{
				HasConditional = true;
				break;
			}
			else HasDirectStart = true;
		}		
	}

	if (HasDirectStart == false)
	{
		//--------------------------Talk Tasks
		LocalID.TaskType = EQuestTaskType::TalkTask;
		for (int i = 0; i < QuestIn.TalkTasks.Num(); i++)
		{
			//Is start node?
			if (QuestIn.TalkTasks[i].SuperiorNodes.Num() == 0)
			{
				HasDirectStart = true;
				break;
			}
		}
	}

	return (HasConditional && HasDirectStart);
}
