//Copyright 2020 Marchetti S. Alfredo I. All Rights Reserved.

#include "QuestEditor/BaseQuestEditor.h"
#include "Task/BaseQuestTask.h"
#include "Quest/QuestsFunctionLibrary.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "QuestEditor/QuestEditorDevFunctionLibrary.h"
#include "QuestEditor/QuestEditorFunctionLibrary.h"
#include "Engine/ObjectLibrary.h"
#include "QuestEditorSettings.h"//Settings
#include "QuestEditor/QuestEditorSaveGame.h"//Save
#include "Kismet/GameplayStatics.h"
#include "Misc/Paths.h"//Paths
#include "Misc/FileHelper.h"//Localization Files search
#include "Misc/LocalTimestampDirectoryVisitor.h"
#include "QuestEditorDev.h"//Log
#include "Subsystem/QuestEditorSubsystem.h"

UBaseQuestEditor::UBaseQuestEditor() : Super()
{
}

void UBaseQuestEditor::NativeConstruct()
{	
	UQuestEditorSubsystem* QESubsystem = GEditor->GetEditorSubsystem<UQuestEditorSubsystem>();
	QESubsystem->SetQuestEditorReference(this);

	//Get datatables
	SetAllDatatablesOfQuest();

	//Fix data on quest editor start
	bool ShowChanges = false;
	FString MessageLocal = FString();
	bool FixedData = FixPreviousVersionData(MessageLocal, ShowChanges);

	//convert dialogs to quest if needed
	bool ConvertedData = ChangeDTsType();

	//Get datatables again, since dialogs changed
	if (ConvertedData) SetAllDatatablesOfQuest();

	//Create editor
	Super::NativeConstruct();

	//---------------------Messages
	if (FixedData)
	{
		//Send a message to the editor
		if (MessageLocal != FString())
		{
			ShowWarnningMessage("Datatables updated. Please save changes.");
		}

		//Show changelog
		if (ShowChanges)
		{
			ShowChangelog.Broadcast();
		}
	}

	//Update Dialogs DT and Localization IDs
	if (ConvertedData)
	{
		//Send a message to the editor
		ShowWarnningMessage("Dialogs datatables converted to Quest. Please save changes.");
	}
}

void UBaseQuestEditor::SetAllDatatablesOfQuest()
{
	//Clear DTS
	QuestDatatables.Empty();
	DialogDatatables.Empty();
	NPCDatatables.Empty();
	TaskInfoDatatables.Empty();
	TalkTaskInfoDatatables.Empty();
	
	//Appends all Dts For all folders
	TArray<FString> CurrentDTs = TArray<FString>();
	
	//Editor content. I always add it. I dont think is good to let them customize this stuff. The only case it could be valid, is if the user copies everything in premade tasks on their own project and customize them.
	CurrentDTs.Add("/QuestEditor/PremadeTasks");

	//Must show example content?
	if (UQuestEditorDevFunctionLibrary::GetQuestEditorSettings()->ShowExampleContent)
	{
		CurrentDTs.Add("/QuestEditor/Examples/Example_Datatables");
	}

	//Append all user defined
	TArray<FString> DefinedDTs = UQuestEditorDevFunctionLibrary::GetQuestEditorSettings()->DatatablesPaths;

	if (DefinedDTs.Num() == 0)
	{
		//If the user has not defined DTs. I will use /Game.
	//	DefinedDTs.Add("/Game"); //we used to search game, but now we just dont search cuz it can freeze ur pc if project is too big.
		UE_LOG(QuestEditorDevLog, Warning, TEXT("UBaseQuestEditor::SetAllDatatablesOfQuest - There is no user defined paths."));
		UE_LOG(QuestEditorDevLog, Warning, TEXT("UBaseQuestEditor::SetAllDatatablesOfQuest - Add the datatable folders path in the Edit->ProjectSettings->Plugins->QuestEditor category."));
	}
		
	CurrentDTs.Append(DefinedDTs);

	//Handle all DTs
	for (int i = 0; i < CurrentDTs.Num(); i++)
	{
		AppendDatatablesOfQuestForOneFolderPath(CurrentDTs[i]);
	}
}

void UBaseQuestEditor::AppendDatatablesOfQuestForOneFolderPath(const FString& FolderPath)
{
	UE_LOG(QuestEditorDevLog, Log, TEXT("UBaseQuestEditor::AppendDatatablesOfQuestForOneFolderPath - Added DTs from Path %s."), *FolderPath);

	// Load the asset registry module
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked< FAssetRegistryModule >(FName("AssetRegistry"));
	IAssetRegistry& AssetRegistry = AssetRegistryModule.Get();

	//--------------------------------Path
	FString FolderPathLocal = FolderPath;

	TArray <FString> ContentPaths;
	ContentPaths.Add(FolderPathLocal);
	
	if (!AssetRegistry.PathExists(FolderPathLocal))
	{
		UE_LOG(QuestEditorDevLog, Error, TEXT("UBaseQuestEditor::AppendDatatablesOfQuestForOneFolderPath - Invalid path %s. Folder skipped. Check that the sintaxis is correct."), *FolderPathLocal);
		return;
	}

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
	if (!FolderPathLocal.IsEmpty())
	{
		Filter.PackagePaths.Add(*FolderPathLocal);
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
			//Quest?
			if (UQuestEditorDevFunctionLibrary::IsDTofStructType(DTLocal, FQuest::StaticStruct()))
			{
				//Add to array
				QuestDatatables.Add(UQuestsFunctionLibrary::GetDTFromStringPath(StringLocal));
			}
			//Dialog
			else if (UQuestEditorDevFunctionLibrary::IsDTofStructType(DTLocal, FDialog::StaticStruct()))
			{
				//Add to array
				DialogDatatables.Add(UQuestsFunctionLibrary::GetDTFromStringPath(StringLocal));
			}
			//NPC
			else if (UQuestEditorDevFunctionLibrary::IsDTofStructType(DTLocal, FNPCQuestDef::StaticStruct()))
			{
				//Add to array
				NPCDatatables.Add(UQuestsFunctionLibrary::GetDTFromStringPath(StringLocal));
			}
			//TaskInfo
			else if (UQuestEditorDevFunctionLibrary::IsDTofStructType(DTLocal, FQuestTaskEditorInfo::StaticStruct()))
			{
				//Add to array
				TaskInfoDatatables.Add(UQuestsFunctionLibrary::GetDTFromStringPath(StringLocal));
			}
			//TalkTaskInfo
			else if (UQuestEditorDevFunctionLibrary::IsDTofStructType(DTLocal, FQuestTalkTaskEditorInfo::StaticStruct()))
			{
				//Add to array
				TalkTaskInfoDatatables.Add(UQuestsFunctionLibrary::GetDTFromStringPath(StringLocal));
			}
		}
	}
}

bool UBaseQuestEditor::FixPreviousVersionData(FString& MessageOut, bool& ShowChangelogOut)
{
	//--------------------------------------------------------------------------
	//-------Check if need update
	//--------------------------------------------------------------------------

	//Do i Need to force update in the quest editor?
	int PreviousVersion = UQuestEditorDevFunctionLibrary::GetSettings_PreviousVersion();

	//Here update the version when wanting to update changes on editor version start.
	//Also update to show changelog window
	int ToCheckVersion = 11;

	//Check if neeed update
	if (PreviousVersion >= ToCheckVersion) return false;

	//--------------------------------------------------------------------------
	//--------Update version in config. 
	//--------------------------------------------------------------------------
	UQuestEditorSettings* EditorSettings = UQuestEditorDevFunctionLibrary::GetQuestEditorSettings();
	
	if (EditorSettings)
	{
		EditorSettings->SetQEVersion(ToCheckVersion);
	}

	//--------------------------------------------------------------------------
	//--------Fixes
	//--------------------------------------------------------------------------
	FString LocalMessage = FString();

	
	if (PreviousVersion < 3)    //Only fix datatables for lower than 3 versions.
	{
		//---------------------------Variables
		TArray <FName, TInlineAllocator<10>> RowNames;
		FQuest* QuestPointer = nullptr;
		FQuest QuestLocal = FQuest();

		TArray <FQuest> AllQuestsInDT = TArray <FQuest>();

		//--------------------------------------------------------------------------
		//-------------------------------------Fix quest datatables
		//--------------------------------------------------------------------------
		FName DTName = FName();

		//Iterate through DT
		for (int i = 0; i < QuestDatatables.Num(); i++)
		{
			//If datatble is from system, do not touch
			DTName = QuestDatatables[i]->GetFName();

			//Skipp example DTs
			if (DTName == "QuestsExamples" || DTName == "DialogsExamples") continue;

			//Get row names
			RowNames = QuestDatatables[i]->GetRowNames();
			AllQuestsInDT.Empty();

			//Make array of correct quests
			for (int j = 0; j < RowNames.Num(); j++)
			{
				QuestPointer = QuestDatatables[i]->FindRow<FQuest>(RowNames[j], FString(), false);
				if (QuestPointer != nullptr)
				{
					//Update local quest
					QuestLocal = *QuestPointer;

					//-------Correct data in local quest.
					//Localization Ids are always corrected in save, so i dont correct them here.
					//Correct conversation IDs
					UQuestEditorDevFunctionLibrary::UpdateAllConversationsIDs(QuestLocal);

					//-------Add to corrected array
					AllQuestsInDT.Add(QuestLocal);
				}
			}

			//Save all rows to the datatable again.
			for (int k = 0; k < AllQuestsInDT.Num(); k++)
			{
				UQuestEditorDevFunctionLibrary::SaveQuestRowToDT(AllQuestsInDT[k], QuestDatatables[i]);
			}
		}

		//--------------------------------------------------------------------------
		//-------------------------------------Fix Dialogs datatables
		//--------------------------------------------------------------------------

		FDialog* DialogPointer = nullptr;

		//Iterate through DT
		for (int i = 0; i < DialogDatatables.Num(); i++)
		{
			//If datatble is from system, do not touch
			DTName = DialogDatatables[i]->GetFName();

			//Skipp example DTs
			if (DTName == "DialogsExamples") continue;

			//Get row names
			RowNames = DialogDatatables[i]->GetRowNames();
			AllQuestsInDT.Empty();

			//Make array of correct quests
			for (int j = 0; j < RowNames.Num(); j++)
			{

				DialogPointer = DialogDatatables[i]->FindRow<FDialog>(RowNames[j], FString(), false);

				if (DialogPointer != nullptr)
				{
					//Update local quest
					QuestLocal = UQuestEditorFunctionLibrary::ConvertDialogToQuest(*DialogPointer);

					//Correct data in local quest. Localization Ids are always corrected in save, so i dont correct them here.
					//For dialogs there is nothing to correct for now.... 
					//I need to resave DTs to correct localization IDs.

					//Add to corrected array
					AllQuestsInDT.Add(QuestLocal);
				}
			}

			//Save all rows to the datatable again.
			for (int k = 0; k < AllQuestsInDT.Num(); k++)
			{
				UQuestEditorDevFunctionLibrary::SaveDialogRowToDT(AllQuestsInDT[k], DialogDatatables[i]);
			}
		}

		UE_LOG(QuestEditorDevLog, Log, TEXT("UBaseQuestEditor::FixPreviousVersionData - All datatables have been updated from previous versions."));

		//Print a message for DT fixed
		LocalMessage = "Datatables updated. Please save changes.";
	}
		
	//Clear editor save game
	DeleteQuestEditorSaveGame();

	//Show changelog the first time and message out
	ShowChangelogOut = true;
	MessageOut = LocalMessage;

	//Returns true to write a message for fixed datatables.
	return true;
}

bool UBaseQuestEditor::ChangeDTsType()
{
	if (DialogDatatables.Num() == 0) return false;

	//---------------------------Variables
	TArray <FName, TInlineAllocator<10>> RowNames;
	FQuest QuestLocal = FQuest();
	TArray <FQuest> AllQuestsInDT = TArray <FQuest>();
	
	//--------------------------------------------------------------------------
	//-------------------------------------Fix Dialogs datatables
	//--------------------------------------------------------------------------

	FDialog* DialogPointer = nullptr;
	FName DTName = FName();

	//Iterate through DT
	for (int i = 0; i < DialogDatatables.Num(); i++)
	{
		//If datatble is from system, do not touch
		DTName = DialogDatatables[i]->GetFName();

		//Skipp example DTs
		if (DTName == "DialogsExamples") continue;

		//Get row names
		RowNames = DialogDatatables[i]->GetRowNames();
		AllQuestsInDT.Empty();

		//Make array of correct quests
		for (int j = 0; j < RowNames.Num(); j++)
		{

			DialogPointer = DialogDatatables[i]->FindRow<FDialog>(RowNames[j], FString(), false);

			if (DialogPointer != nullptr)
			{
				//Update local quest
				QuestLocal = UQuestEditorFunctionLibrary::ConvertDialogToQuest(*DialogPointer);

				//Add to corrected array
				AllQuestsInDT.Add(QuestLocal);
			}
		}

		//Change Datatable Type
		DialogDatatables[i]->EmptyTable();
		DialogDatatables[i]->CleanBeforeStructChange();		
		DialogDatatables[i]->RowStruct = FQuest::StaticStruct();

		//Save all rows to the datatable again as quests.
		for (int k = 0; k < AllQuestsInDT.Num(); k++)
		{
			UQuestEditorDevFunctionLibrary::SaveQuestRowToDT(AllQuestsInDT[k], DialogDatatables[i]);
		}
	}
	
	//Fix localization Ids in localization Game Files
	//--------------------------------Path for Localization is fixed
	FString FolderPathLocal = FPaths::ProjectDir() + "Content/Localization/Game/";

	//Uses localization?
	if (!FPaths::DirectoryExists(FolderPathLocal)) return true;

	//-------------------------------Get all files in directory
	TArray<FString> directoriesToSkip;
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	FLocalTimestampDirectoryVisitor Visitor(PlatformFile, directoriesToSkip, directoriesToSkip, false);
	PlatformFile.IterateDirectory(*FolderPathLocal, Visitor);
	TArray<FString> files = TArray<FString>();
	FString filePath = FString();
	FString fileName = FString();
	FString fileExtension = FString();

	for (TMap<FString, FDateTime>::TIterator TimestampIt(Visitor.FileTimes); TimestampIt; ++TimestampIt)
	{
		filePath = TimestampIt.Key();
		fileName = FPaths::GetBaseFilename(filePath);
		fileExtension = FPaths::GetExtension(filePath);

		//If name is Game.archive
		if (fileName == "Game" && fileExtension == "archive")
		{
			files.Add(filePath);
		}
	}

	for (int i = 0; i < files.Num(); i++)
	{
		//Try load, if cannot load stop
		FString StringBuffer = FString();
		if (FFileHelper::LoadFileToString(StringBuffer, *files[i]))
		{
			//Replace string  Player Dialog  DPDD  -> TPDQ
			FString From = "DPDD";
			FString To = "TPDQ";
			const TCHAR* FromChar = *From;
			const TCHAR* ToChar = *To;
			StringBuffer = StringBuffer.Replace(FromChar, ToChar, ESearchCase::CaseSensitive);

			//Replace string  NPC Response  DNRD  ->  TNRQ
			From = "DNRD";
			To = "TNRQ";
			FromChar = *From;
			ToChar = *To;
			StringBuffer = StringBuffer.Replace(FromChar, ToChar, ESearchCase::CaseSensitive);

			//Replace string  TaskShortDescription  DSDD  ->  TSDQ
			From = "DSDD";
			To = "TSDQ";
			FromChar = *From;
			ToChar = *To;
			StringBuffer = StringBuffer.Replace(FromChar, ToChar, ESearchCase::CaseSensitive);

			//Save File
			FFileHelper::SaveStringToFile(StringBuffer, *files[i]);
		}
	}
	
	//Returns true to write a message for fixed datatables.
	return true;
}

bool UBaseQuestEditor::DeleteQuestEditorSaveGame()
{	
	//----Save Name
	FString LocalSaveName = "QuestEditorData";

	UE_LOG(QuestEditorDevLog, Log, TEXT("UBaseQuestEditor::DeleteQuestEditorSaveGame - Quest Editor Save Game cleared."));
	return UGameplayStatics::DeleteGameInSlot(LocalSaveName, 0);		
}

UClass* UBaseQuestEditor::GetSelectorFromClass(FSoftClassPath ClassSelected, FSoftClassPath DefaultSelector)
{
	FSoftClassPath PathLocal;

	//If asset is valid...
	if (ClassSelected.IsValid())
	{
		PathLocal = ClassSelected;
	}
	else PathLocal = DefaultSelector;

	//Load the class, returns null if not found.
	UClass* ClassLoaded = PathLocal.TryLoadClass<UObject>();

	//If class provided is null go back to default
	if (!ClassLoaded)
	{
		//load default
		PathLocal = DefaultSelector;
		ClassLoaded = PathLocal.TryLoadClass<UObject>();
	}

	return ClassLoaded;
}

UClass* UBaseQuestEditor::GetQETaskSelector()
{
	FString ConfigSelector = UQuestEditorDevFunctionLibrary::GetQuestEditorSettings()->EditorTaskSelector;

	return GetSelectorFromClass(ConfigSelector, FSoftClassPath(TEXT("/QuestEditor/QuestEditor/Selectors/TaskSelector/EWBP_TaskSelector.EWBP_TaskSelector_C")));
}

UClass* UBaseQuestEditor::GetQEQuestSelector()
{
	FString ConfigSelector = UQuestEditorDevFunctionLibrary::GetQuestEditorSettings()->EditorQuestSelector;

	return GetSelectorFromClass(ConfigSelector, FSoftClassPath(TEXT("/QuestEditor/QuestEditor/Selectors/StringSelectors/EWBP_QuestSelector.EWBP_QuestSelector_C")));
}

UClass* UBaseQuestEditor::GetQENPCSelector()
{
	FString ConfigSelector = UQuestEditorDevFunctionLibrary::GetQuestEditorSettings()->EditorNPCSelector;

	return GetSelectorFromClass(ConfigSelector, FSoftClassPath(TEXT("/QuestEditor/QuestEditor/Selectors/StringSelectors/EWBP_NPCSelector.EWBP_NPCSelector_C")));
}

FVector2D UBaseQuestEditor::GetEditorGridSize()
{
	return  UQuestEditorDevFunctionLibrary::GetQuestEditorSettings()->EditorGridDimensions;
}

float UBaseQuestEditor::GetEditorAutosaveFreq()
{
	float Freq = UQuestEditorDevFunctionLibrary::GetQuestEditorSettings()->AutosaveFrequency;
	//Negative is disable.
	if (Freq <= 0) return 0;
	//If is not 0 wich is disable, clamp it on lower value only. I dont want to save too often.
	if (Freq < 30 && Freq >0) Freq = 30;
	return Freq;
}

FQuestTalkTaskEditorInfo UBaseQuestEditor::GetDefaultTalkModeInfo(bool& FoundModeOut)
{	
	FString TalkModeNameDefault = UQuestEditorDevFunctionLibrary::GetQuestEditorSettings()->DefaultTalkMode;
	
	TArray <FName> DTNames = TArray <FName>();
	bool FoundMode = false;
	FQuestTalkTaskEditorInfo InfoOUT = FQuestTalkTaskEditorInfo();

	FTalkModeDef TalkMode = FTalkModeDef();

	for (int i = 0; i < TalkTaskInfoDatatables.Num(); i++)
	{
		//-------------------------------------------------Analise DT
		DTNames = TalkTaskInfoDatatables[i]->GetRowNames();

		for (int j = 0; j < DTNames.Num(); j++)
		{

			FQuestTalkTaskEditorInfo* InfoPointer = TalkTaskInfoDatatables[i]->FindRow<FQuestTalkTaskEditorInfo>(DTNames[j], FString(), false);

			if (InfoPointer)
			{
				InfoOUT = *InfoPointer;
				//same class. check by asset name
				if (InfoOUT.TalkModeName == TalkModeNameDefault)
				{
					FoundMode = true;
					break;
				}
			}
		}
		//if found a  talk mode already break first loop
		if (FoundMode == true) break;		
	}

	//notify if found the mode.
	FoundModeOut = FoundMode;

	return InfoOUT;
}

void UBaseQuestEditor::ShowNormalMessage(const FString& MessageIn)
{
	ShowMessage.Broadcast(MessageIn, EditorNormalMessageColor);
}

void UBaseQuestEditor::ShowWarnningMessage(const FString& MessageIn)
{
	ShowMessage.Broadcast(MessageIn, EditorWarnningMessageColor);
}

FQuestTalkTaskEditorInfo UBaseQuestEditor::GetCurrentTalkModeInfo(const FQuest& QuestIn)
{
	FQuestTalkTaskEditorInfo CurrentMode = FQuestTalkTaskEditorInfo();
	UDataTable* DT;
	FQuestTalkTaskEditorInfo* CurrentModePointer = nullptr;

	bool FoundMode = false;

	//DT is valid?
	if (QuestIn.TalkMode.Datatable.IsValid() && QuestIn.TalkMode.RowName != "None")
	{
		//Finds DT
		DT = UQuestsFunctionLibrary::GetDTFromPath(QuestIn.TalkMode.Datatable);

		//Is DT valid?
		if (DT) 
		{
			CurrentModePointer = DT->FindRow<FQuestTalkTaskEditorInfo>(QuestIn.TalkMode.RowName, FString(), false);

			if (CurrentModePointer != nullptr)
			{
				CurrentMode = *CurrentModePointer;
				FoundMode = true;
			}
		}
	
	}

	//If has not found the mode, try default
	if (!FoundMode)
	{		
		//If invalid use default mode.
		//Is default mode valid?
		CurrentMode = GetDefaultTalkModeInfo(FoundMode);		
	}

	//If still cannot find mode, use conversation mode directly.
	if (!FoundMode)
	{
		//Finds DT fixed for conversation
		DT = UQuestsFunctionLibrary::GetDTFromStringPath("/QuestEditor/PremadeTasks/TalkTask_Modes.TalkTask_Modes");
		//Is DT valid?
		if (DT)
		{
			//find conversation mode.
			CurrentModePointer = DT->FindRow<FQuestTalkTaskEditorInfo>("Conversation", FString(), false);

			if (CurrentModePointer != nullptr)
			{
				CurrentMode = *CurrentModePointer;
				FoundMode = true;
			}
		}
	}	
	
	return CurrentMode;
}

UQuestEditorSaveGame* UBaseQuestEditor::QEditorSave_GetSavegame(bool& FoundSaveOut)
{
	//Save name
	FString SaveName = "QuestEditorData";

	//Createnew or load existing one
	UQuestEditorSaveGame* SaveGameInstance = nullptr;
	if (UGameplayStatics::DoesSaveGameExist(SaveName, 0))
	{
		//Load existing savegame object
		SaveGameInstance = Cast<UQuestEditorSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveName, 0));
		FoundSaveOut = true;
	}
	else
	{
		FoundSaveOut = false;
	}	
	
	return SaveGameInstance;
}

void UBaseQuestEditor::QEditorSave_SaveVariables(bool MantainPrevDialogsIn, bool MantainPrevQuestsIn, EQuestEditor_EditMode CurrentEditorModeIn, int UndoQuestIndexSavedIn, int UndoDialogIndexSavedIn, float ViewportScaleSavedIn, const TArray<FQuest>& PreviousQuestsIn, const TArray<FQuest>& UndoHistoryQuestsIn, const TArray<FQuest>& PreviousDialogsIn, const TArray<FQuest>& UndoHistoryDialogsIn)
{
	//Save name
	FString SaveName = "QuestEditorData";

	//Createnew or load existing one
	UQuestEditorSaveGame* SaveGameInstance = nullptr;
	if (UGameplayStatics::DoesSaveGameExist(SaveName, 0))
	{
		//Load existing savegame object
		SaveGameInstance = Cast<UQuestEditorSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveName, 0));
	}
	else
	{
		//Create new save object
		SaveGameInstance = Cast<UQuestEditorSaveGame>(UGameplayStatics::CreateSaveGameObject(UQuestEditorSaveGame::StaticClass()));		
	}

	//Save data
	if (SaveGameInstance)
	{
		// Set data on the savegame object.
		SaveGameInstance->CurrentEditorMode = CurrentEditorModeIn;

		SaveGameInstance->UndoQuestIndexSaved = UndoQuestIndexSavedIn;
		SaveGameInstance->UndoDialogIndexSaved = UndoDialogIndexSavedIn;

		SaveGameInstance->ViewportScaleSaved = ViewportScaleSavedIn;
		//If maint previous, ignore the input and keep the one that is already saved
		if (!MantainPrevQuestsIn)SaveGameInstance->PreviousQuests = PreviousQuestsIn;
		SaveGameInstance->UndoHistoryQuests = UndoHistoryQuestsIn;
		//If maint previous, ignore the input and keep the one that is already saved
		if (!MantainPrevDialogsIn)SaveGameInstance->PreviousDialogs = PreviousDialogsIn;
		SaveGameInstance->UndoHistoryDialogs = UndoHistoryDialogsIn;

		// Start sync save process.
		UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveName, 0);
	}

	return;
}

void UBaseQuestEditor::QEditorSave_GetVariables(EQuestEditor_EditMode& CurrentEditorModeOut, int& UndoQuestIndexSavedOut, int& UndoDialogIndexSavedOut, float& ViewportScaleSavedOut, TArray<FQuest>& PreviousQuestsOut, TArray<FQuest>& UndoHistoryQuestsOut, TArray<FQuest>& PreviousDialogsOut, TArray<FQuest>& UndoHistoryDialogsOut)
{
	//Save name
	FString SaveName = "QuestEditorData";

	bool FoundSave = false;
	UQuestEditorSaveGame* SaveGameInstance = QEditorSave_GetSavegame(FoundSave);

	if (FoundSave)
	{
		//Set data from the savegame object.
		CurrentEditorModeOut = SaveGameInstance->CurrentEditorMode;

		UndoQuestIndexSavedOut = SaveGameInstance->UndoQuestIndexSaved;
		UndoDialogIndexSavedOut = SaveGameInstance->UndoDialogIndexSaved;

		ViewportScaleSavedOut = SaveGameInstance->ViewportScaleSaved;

		PreviousQuestsOut = SaveGameInstance->PreviousQuests;
		UndoHistoryQuestsOut = SaveGameInstance->UndoHistoryQuests;
		PreviousDialogsOut = SaveGameInstance->PreviousDialogs;
		UndoHistoryDialogsOut = SaveGameInstance->UndoHistoryDialogs;
	}

	return;
}

bool UBaseQuestEditor::QEditorSave_GetVariables_Primary(TArray<FQuest>& PreviousQuestsOut, TArray<FQuest>& PreviousDialogsOut)
{
	bool FoundSave = false;
	UQuestEditorSaveGame* SaveGameInstance = QEditorSave_GetSavegame(FoundSave);

	if (FoundSave)
	{
		PreviousQuestsOut = SaveGameInstance->PreviousQuests;
		PreviousDialogsOut = SaveGameInstance->PreviousDialogs;
	}
	return FoundSave;
}

bool UBaseQuestEditor::QEditorSave_GetVariables_Undo(int& UndoQuestIndexSavedOut, int& UndoDialogIndexSavedOut, TArray<FQuest>& UndoHistoryQuestsOut, TArray<FQuest>& UndoHistoryDialogsOut)
{
	bool FoundSave = false;
	UQuestEditorSaveGame* SaveGameInstance = QEditorSave_GetSavegame(FoundSave);

	if (FoundSave)
	{
		UndoQuestIndexSavedOut = SaveGameInstance->UndoQuestIndexSaved;
		UndoDialogIndexSavedOut = SaveGameInstance->UndoDialogIndexSaved;
		UndoHistoryQuestsOut = SaveGameInstance->UndoHistoryQuests;
		UndoHistoryDialogsOut = SaveGameInstance->UndoHistoryDialogs;
	}
	return FoundSave;
}

bool UBaseQuestEditor::QEditorSave_GetVariables_Misc(EQuestEditor_EditMode& CurrentEditorModeOut, float& ViewportScaleSavedOut)
{
	bool FoundSave = false;
	UQuestEditorSaveGame* SaveGameInstance = QEditorSave_GetSavegame(FoundSave);

	if (FoundSave)
	{
		CurrentEditorModeOut = SaveGameInstance->CurrentEditorMode;
		ViewportScaleSavedOut = SaveGameInstance->ViewportScaleSaved;
	}
	return FoundSave;
}

void UBaseQuestEditor::UpdateTalkTaskNPC(UPARAM(ref)FQuest& QuestIn, const TArray <FQuestTaskID>& TalkTasksIn, const FNPCQuestDefRep& NPCDefIn, TArray <FQuestTaskID>& TalkTaskIDsOut, TArray <FQuestTalkTask>& TalkTasksOut)
{
	TArray <FQuestTaskID, TInlineAllocator<3>> TalkTaskIDsLocal;
	TArray <FQuestTalkTask, TInlineAllocator<3>> TalkTasksLocal;

	for (int i = 0; i < TalkTasksIn.Num(); i++)
	{
		//Task is Talk type
		if (TalkTasksIn[i].TaskType == EQuestTaskType::TalkTask)
		{
			if (UQuestsFunctionLibrary::IsTaskIDValidInQuest(QuestIn, TalkTasksIn[i]))
			{
				//Update the task
				QuestIn.TalkTasks[TalkTasksIn[i].TaskID].TalkNPC = NPCDefIn;
				//Updated tasks track
				TalkTaskIDsLocal.Add(TalkTasksIn[i]);
				TalkTasksLocal.Add(QuestIn.TalkTasks[TalkTasksIn[i].TaskID]);
			}
			else
			{
				UE_LOG(QuestEditorDevLog, Warning, TEXT("UBaseQuestEditor::UpdateTalkTaskNPC - Talk Task %i is invalid."), TalkTasksIn[i].TaskID);
			}
		}		
		else
		{
			UE_LOG(QuestEditorDevLog, Warning, TEXT("UBaseQuestEditor::UpdateTalkTaskNPC - Task provided instead of TalkTask."));
		}	
	}

	//Must update conversations ids
	//The node is not connected to other nodes and is only one node editing? common case to rule out
	if (!(QuestIn.TalkTasks[TalkTasksIn[0].TaskID].SuperiorNodes.Num() == 0 && QuestIn.TalkTasks[TalkTasksIn[0].TaskID].InferiorNodes.Num() == 0 && TalkTasksIn.Num() == 1))
	{
		UQuestEditorDevFunctionLibrary::UpdateAllConversationsIDs(QuestIn);
	}		

	//Out variables update
	TalkTaskIDsOut = TalkTaskIDsLocal;
	TalkTasksOut = TalkTasksLocal;
}

void UBaseQuestEditor::UpdateTalkTaskShortDesc(UPARAM(ref)FQuest& QuestIn, const TArray <FQuestTaskID>& TalkTasksIn, FText ShortDescIn, TArray <FQuestTaskID>& TalkTaskIDsOut, TArray <FQuestTalkTask>& TalkTasksOut)
{	
	TArray <FQuestTaskID, TInlineAllocator<3>> TalkTaskIDsLocal;
	TArray <FQuestTalkTask, TInlineAllocator<3>> TalkTasksLocal;

	for (int i = 0; i < TalkTasksIn.Num(); i++)
	{
		//Task is Talk type
		if (TalkTasksIn[i].TaskType == EQuestTaskType::TalkTask)
		{
			if (UQuestsFunctionLibrary::IsTaskIDValidInQuest(QuestIn, TalkTasksIn[i]))
			{
				//Update the task
				QuestIn.TalkTasks[TalkTasksIn[i].TaskID].TaskShortDesc = ShortDescIn;
				//Updated tasks track
				TalkTaskIDsLocal.Add(TalkTasksIn[i]);
				TalkTasksLocal.Add(QuestIn.TalkTasks[TalkTasksIn[i].TaskID]);
			}
			else
			{
				UE_LOG(QuestEditorDevLog, Warning, TEXT("UBaseQuestEditor::UpdateTalkTaskShortDesc - Talk Task %i is invalid."), TalkTasksIn[i].TaskID);
			}
		}
		else
		{
			UE_LOG(QuestEditorDevLog, Warning, TEXT("UBaseQuestEditor::UpdateTalkTaskShortDesc - Task provided instead of TalkTask."));
		}
	}

	//Out variables update
	TalkTaskIDsOut = TalkTaskIDsLocal;
	TalkTasksOut = TalkTasksLocal;	
}

void UBaseQuestEditor::UpdateTalkTaskCustomPayloadData(UPARAM(ref)FQuest& QuestIn, const TArray<FQuestTaskID>& TalkTasksIn, FString CustomPaylodDataIn, TArray<FQuestTaskID>& TalkTaskIDsOut, TArray<FQuestTalkTask>& TalkTasksOut)
{
	TArray <FQuestTaskID, TInlineAllocator<3>> TalkTaskIDsLocal;
	TArray <FQuestTalkTask, TInlineAllocator<3>> TalkTasksLocal;

	for (int i = 0; i < TalkTasksIn.Num(); i++)
	{
		//Task is Talk type
		if (TalkTasksIn[i].TaskType == EQuestTaskType::TalkTask)
		{
			if (UQuestsFunctionLibrary::IsTaskIDValidInQuest(QuestIn, TalkTasksIn[i]))
			{
				//Update the task
				QuestIn.TalkTasks[TalkTasksIn[i].TaskID].CustomPayloadData = CustomPaylodDataIn;
				//Updated tasks track
				TalkTaskIDsLocal.Add(TalkTasksIn[i]);
				TalkTasksLocal.Add(QuestIn.TalkTasks[TalkTasksIn[i].TaskID]);
			}
			else
			{
				UE_LOG(QuestEditorDevLog, Warning, TEXT("UBaseQuestEditor::UpdateTalkTaskCustomPayloadData - Talk Task %i is invalid."), TalkTasksIn[i].TaskID);
			}
		}
		else
		{
			UE_LOG(QuestEditorDevLog, Warning, TEXT("UBaseQuestEditor::UpdateTalkTaskCustomPayloadData - Task provided instead of TalkTask."));
		}
	}

	//Out variables update
	TalkTaskIDsOut = TalkTaskIDsLocal;
	TalkTasksOut = TalkTasksLocal;
}

void UBaseQuestEditor::UpdateTalkTaskYouSay(UPARAM(ref)FQuest& QuestIn, const TArray <FQuestTaskID>& TalkTasksIn, const FText& YouSay, TArray <FQuestTaskID>& TalkTaskIDsOut, TArray <FQuestTalkTask>& TalkTasksOut)
{
	TArray <FQuestTaskID, TInlineAllocator<3>> TalkTaskIDsLocal;
	TArray <FQuestTalkTask, TInlineAllocator<3>> TalkTasksLocal;

	for (int i = 0; i < TalkTasksIn.Num(); i++)
	{
		//Task is Talk type
		if (TalkTasksIn[i].TaskType == EQuestTaskType::TalkTask)
		{
			if (UQuestsFunctionLibrary::IsTaskIDValidInQuest(QuestIn, TalkTasksIn[i]))
			{
				//Update the task
				QuestIn.TalkTasks[TalkTasksIn[i].TaskID].PlayerDialog = YouSay;
						
				//Updated tasks track
				TalkTaskIDsLocal.Add(TalkTasksIn[i]);
				TalkTasksLocal.Add(QuestIn.TalkTasks[TalkTasksIn[i].TaskID]);
			}
			else
			{
				UE_LOG(QuestEditorDevLog, Warning, TEXT("UBaseQuestEditor::UpdateTalkTaskYouSay - Talk Task %i is invalid."), TalkTasksIn[i].TaskID);
			}
		}
		else
		{
			UE_LOG(QuestEditorDevLog, Warning, TEXT("UBaseQuestEditor::UpdateTalkTaskYouSay - Task provided instead of TalkTask."));
		}
	}

	//Out variables update
	TalkTaskIDsOut = TalkTaskIDsLocal;
	TalkTasksOut = TalkTasksLocal;
}

void UBaseQuestEditor::UpdateTalkTaskNPCResponse(UPARAM(ref)FQuest& QuestIn, const TArray <FQuestTaskID>& TalkTasksIn, FText NpcResponse, TArray <FQuestTaskID>& TalkTaskIDsOut, TArray <FQuestTalkTask>& TalkTasksOut)
{
	TArray <FQuestTaskID, TInlineAllocator<3>> TalkTaskIDsLocal;
	TArray <FQuestTalkTask, TInlineAllocator<3>> TalkTasksLocal;

	for (int i = 0; i < TalkTasksIn.Num(); i++)
	{
		//Task is Talk type
		if (TalkTasksIn[i].TaskType == EQuestTaskType::TalkTask)
		{
			if (UQuestsFunctionLibrary::IsTaskIDValidInQuest(QuestIn, TalkTasksIn[i]))
			{
				//Update the task
				QuestIn.TalkTasks[TalkTasksIn[i].TaskID].NPCResponse = NpcResponse;
				//Updated tasks track
				TalkTaskIDsLocal.Add(TalkTasksIn[i]);
				TalkTasksLocal.Add(QuestIn.TalkTasks[TalkTasksIn[i].TaskID]);
			}
			else
			{
				UE_LOG(QuestEditorDevLog, Warning, TEXT("UBaseQuestEditor::UpdateTalkTaskNPCResponse - Talk Task %i is invalid."), TalkTasksIn[i].TaskID);
			}
		}
		else
		{
			UE_LOG(QuestEditorDevLog, Warning, TEXT("UBaseQuestEditor::UpdateTalkTaskNPCResponse - Task provided instead of TalkTask."));
		}
	}

	//Out variables update
	TalkTaskIDsOut = TalkTaskIDsLocal;
	TalkTasksOut = TalkTasksLocal;			
}

void UBaseQuestEditor::UpdateTalkTaskPlayerCue(UPARAM(ref)FQuest& QuestIn, const TArray <FQuestTaskID>& TalkTasksIn, TSoftObjectPtr<USoundCue> PlayerCue, TArray <FQuestTaskID>& TalkTaskIDsOut, TArray <FQuestTalkTask>& TalkTasksOut)
{
	TArray <FQuestTaskID, TInlineAllocator<3>> TalkTaskIDsLocal;
	TArray <FQuestTalkTask, TInlineAllocator<3>> TalkTasksLocal;

	for (int i = 0; i < TalkTasksIn.Num(); i++)
	{
		//Task is Talk type
		if (TalkTasksIn[i].TaskType == EQuestTaskType::TalkTask)
		{
			if (UQuestsFunctionLibrary::IsTaskIDValidInQuest(QuestIn, TalkTasksIn[i]))
			{
				//Update the task
				QuestIn.TalkTasks[TalkTasksIn[i].TaskID].PlayerSoundCue = PlayerCue;
				//Updated tasks track
				TalkTaskIDsLocal.Add(TalkTasksIn[i]);
				TalkTasksLocal.Add(QuestIn.TalkTasks[TalkTasksIn[i].TaskID]);
			}
			else
			{
				UE_LOG(QuestEditorDevLog, Warning, TEXT("UBaseQuestEditor::UpdateTalkTaskPlayerCue - Talk Task %i is invalid."), TalkTasksIn[i].TaskID);
			}
		}
		else
		{
			UE_LOG(QuestEditorDevLog, Warning, TEXT("UBaseQuestEditor::UpdateTalkTaskPlayerCue - Task provided instead of TalkTask."));
		}
	}

	//Out variables update
	TalkTaskIDsOut = TalkTaskIDsLocal;
	TalkTasksOut = TalkTasksLocal;	
}

void UBaseQuestEditor::UpdateTalkTaskNPCCue(UPARAM(ref)FQuest& QuestIn, const TArray <FQuestTaskID>& TalkTasksIn, TSoftObjectPtr<USoundCue> NpcCue, TArray <FQuestTaskID>& TalkTaskIDsOut, TArray <FQuestTalkTask>& TalkTasksOut)
{
	TArray <FQuestTaskID, TInlineAllocator<3>> TalkTaskIDsLocal;
	TArray <FQuestTalkTask, TInlineAllocator<3>> TalkTasksLocal;

	for (int i = 0; i < TalkTasksIn.Num(); i++)
	{
		//Task is Talk type
		if (TalkTasksIn[i].TaskType == EQuestTaskType::TalkTask)
		{
			if (UQuestsFunctionLibrary::IsTaskIDValidInQuest(QuestIn, TalkTasksIn[i]))
			{
				//Update the task
				QuestIn.TalkTasks[TalkTasksIn[i].TaskID].NPCSoundCue = NpcCue;
				//Updated tasks track
				TalkTaskIDsLocal.Add(TalkTasksIn[i]);
				TalkTasksLocal.Add(QuestIn.TalkTasks[TalkTasksIn[i].TaskID]);
			}
			else
			{
				UE_LOG(QuestEditorDevLog, Warning, TEXT("UBaseQuestEditor::UpdateTalkTaskNPCCue - Talk Task %i is invalid."), TalkTasksIn[i].TaskID);
			}
		}
		else
		{
			UE_LOG(QuestEditorDevLog, Warning, TEXT("UBaseQuestEditor::UpdateTalkTaskNPCCue - Task provided instead of TalkTask."));
		}
	}

	//Out variables update
	TalkTaskIDsOut = TalkTaskIDsLocal;
	TalkTasksOut = TalkTasksLocal;	
}

void UBaseQuestEditor::UpdateTaskNameInv(UPARAM(ref)FQuest& QuestIn, const TArray <FQuestTaskID>& TasksIn, FString NameIn, TArray <FQuestTaskID>& TaskIDsOut, TArray <FQuestTask>& TasksOut)
{
	TArray <FQuestTaskID, TInlineAllocator<3>> TaskIDsLocal;
	TArray <FQuestTask, TInlineAllocator<3>> TasksLocal;

	for (int i = 0; i < TasksIn.Num(); i++)
	{
		//Task is Task type
		if (TasksIn[i].TaskType == EQuestTaskType::Task)
		{
			if (UQuestsFunctionLibrary::IsTaskIDValidInQuest(QuestIn, TasksIn[i]))
			{
				//Update the task
				QuestIn.Tasks[TasksIn[i].TaskID].Payload.StringA = NameIn;
				//Updated tasks track
				TaskIDsLocal.Add(TasksIn[i]);
				TasksLocal.Add(QuestIn.Tasks[TasksIn[i].TaskID]);
			}
			else
			{
				UE_LOG(QuestEditorDevLog, Warning, TEXT("UBaseQuestEditor::UpdateTaskNameInv - Task %i is invalid."), TasksIn[i].TaskID);
			}
		}
		else
		{
			UE_LOG(QuestEditorDevLog, Warning, TEXT("UBaseQuestEditor::UpdateTaskNameInv - Talk Task provided instead of Task."));
		}
	}

	//Out variables update
	TaskIDsOut = TaskIDsLocal;
	TasksOut = TasksLocal;	
}

void UBaseQuestEditor::UpdateTaskName2Inv(UPARAM(ref)FQuest& QuestIn, const TArray <FQuestTaskID>& TasksIn, FString NameIn, TArray <FQuestTaskID>& TaskIDsOut, TArray <FQuestTask>& TasksOut)
{
	TArray <FQuestTaskID, TInlineAllocator<3>> TaskIDsLocal;
	TArray <FQuestTask, TInlineAllocator<3>> TasksLocal;

	for (int i = 0; i < TasksIn.Num(); i++)
	{
		//Task is Task type
		if (TasksIn[i].TaskType == EQuestTaskType::Task)
		{
			if (UQuestsFunctionLibrary::IsTaskIDValidInQuest(QuestIn, TasksIn[i]))
			{
				//Update the task
				QuestIn.Tasks[TasksIn[i].TaskID].Payload.StringB = NameIn;
				//Updated tasks track
				TaskIDsLocal.Add(TasksIn[i]);
				TasksLocal.Add(QuestIn.Tasks[TasksIn[i].TaskID]);
			}
			else
			{
				UE_LOG(QuestEditorDevLog, Warning, TEXT("UBaseQuestEditor::UpdateTaskName2Inv - Task %i is invalid."), TasksIn[i].TaskID);
			}
		}
		else
		{
			UE_LOG(QuestEditorDevLog, Warning, TEXT("UBaseQuestEditor::UpdateTaskName2Inv - Talk Task provided instead of Task."));
		}
	}

	//Out variables update
	TaskIDsOut = TaskIDsLocal;
	TasksOut = TasksLocal;
}

void UBaseQuestEditor::UpdateTaskTimesToCheck(UPARAM(ref)FQuest& QuestIn, const TArray <FQuestTaskID>& TasksIn, int TimesIn, TArray <FQuestTaskID>& TaskIDsOut, TArray <FQuestTask>& TasksOut)
{
	TArray <FQuestTaskID, TInlineAllocator<3>> TaskIDsLocal;
	TArray <FQuestTask, TInlineAllocator<3>> TasksLocal;

	for (int i = 0; i < TasksIn.Num(); i++)
	{
		//Task is Task type
		if (TasksIn[i].TaskType == EQuestTaskType::Task)
		{
			if (UQuestsFunctionLibrary::IsTaskIDValidInQuest(QuestIn, TasksIn[i]))
			{
				//Update the task
				QuestIn.Tasks[TasksIn[i].TaskID].Payload.TimesToCheckTask = TimesIn;
				//Updated tasks track
				TaskIDsLocal.Add(TasksIn[i]);
				TasksLocal.Add(QuestIn.Tasks[TasksIn[i].TaskID]);
			}
			else
			{
				UE_LOG(QuestEditorDevLog, Warning, TEXT("UBaseQuestEditor::UpdateTaskTimesToCheck - Task %i is invalid."), TasksIn[i].TaskID);
			}
		}
		else
		{
			UE_LOG(QuestEditorDevLog, Warning, TEXT("UBaseQuestEditor::UpdateTaskTimesToCheck - Talk Task provided instead of Task."));
		}
	}

	//Out variables update
	TaskIDsOut = TaskIDsLocal;
	TasksOut = TasksLocal;
}

void UBaseQuestEditor::UpdateTaskAmountInv(UPARAM(ref)FQuest& QuestIn, const TArray <FQuestTaskID>& TasksIn, float AmountIn, TArray <FQuestTaskID>& TaskIDsOut, TArray <FQuestTask>& TasksOut)
{
	TArray <FQuestTaskID, TInlineAllocator<3>> TaskIDsLocal;
	TArray <FQuestTask, TInlineAllocator<3>> TasksLocal;

	for (int i = 0; i < TasksIn.Num(); i++)
	{
		//Task is Task type
		if (TasksIn[i].TaskType == EQuestTaskType::Task)
		{
			if (UQuestsFunctionLibrary::IsTaskIDValidInQuest(QuestIn, TasksIn[i]))
			{
				//Update the task
				QuestIn.Tasks[TasksIn[i].TaskID].Payload.AmountInvolved = AmountIn;
				//Updated tasks track
				TaskIDsLocal.Add(TasksIn[i]);
				TasksLocal.Add(QuestIn.Tasks[TasksIn[i].TaskID]);
			}
			else
			{
				UE_LOG(QuestEditorDevLog, Warning, TEXT("UBaseQuestEditor::UpdateTaskAmountInv - Task %i is invalid."), TasksIn[i].TaskID);
			}
		}
		else
		{
			UE_LOG(QuestEditorDevLog, Warning, TEXT("UBaseQuestEditor::UpdateTaskAmountInv - Talk Task provided instead of Task."));
		}
	}

	//Out variables update
	TaskIDsOut = TaskIDsLocal;
	TasksOut = TasksLocal;
}

void UBaseQuestEditor::UpdateTaskTagsRequired(UPARAM(ref)FQuest& QuestIn, const TArray <FQuestTaskID>& TasksIn, FGameplayTagContainer TagsIn, TArray <FQuestTaskID>& TaskIDsOut, TArray <FQuestTask>& TasksOut)
{
	TArray <FQuestTaskID, TInlineAllocator<3>> TaskIDsLocal;
	TArray <FQuestTask, TInlineAllocator<3>> TasksLocal;

	for (int i = 0; i < TasksIn.Num(); i++)
	{
		//Task is Task type
		if (TasksIn[i].TaskType == EQuestTaskType::Task)
		{
			if (UQuestsFunctionLibrary::IsTaskIDValidInQuest(QuestIn, TasksIn[i]))
			{
				//Update the task
				QuestIn.Tasks[TasksIn[i].TaskID].Payload.RequiredTags = TagsIn;
				//Updated tasks track
				TaskIDsLocal.Add(TasksIn[i]);
				TasksLocal.Add(QuestIn.Tasks[TasksIn[i].TaskID]);
			}
			else
			{
				UE_LOG(QuestEditorDevLog, Warning, TEXT("UBaseQuestEditor::UpdateTaskTagsRequired - Task %i is invalid."), TasksIn[i].TaskID);
			}
		}
		else
		{
			UE_LOG(QuestEditorDevLog, Warning, TEXT("UBaseQuestEditor::UpdateTaskTagsRequired - Talk Task provided instead of Task."));
		}
	}

	//Out variables update
	TaskIDsOut = TaskIDsLocal;
	TasksOut = TasksLocal;	
}

void UBaseQuestEditor::UpdateTaskTagsIgnored(UPARAM(ref)FQuest& QuestIn, const TArray <FQuestTaskID>& TasksIn, FGameplayTagContainer TagsIn, TArray <FQuestTaskID>& TaskIDsOut, TArray <FQuestTask>& TasksOut)
{
	TArray <FQuestTaskID, TInlineAllocator<3>> TaskIDsLocal;
	TArray <FQuestTask, TInlineAllocator<3>> TasksLocal;

	for (int i = 0; i < TasksIn.Num(); i++)
	{
		//Task is Task type
		if (TasksIn[i].TaskType == EQuestTaskType::Task)
		{
			if (UQuestsFunctionLibrary::IsTaskIDValidInQuest(QuestIn, TasksIn[i]))
			{
				//Update the task
				QuestIn.Tasks[TasksIn[i].TaskID].Payload.IgnoredTags = TagsIn;
				//Updated tasks track
				TaskIDsLocal.Add(TasksIn[i]);
				TasksLocal.Add(QuestIn.Tasks[TasksIn[i].TaskID]);
			}
			else
			{
				UE_LOG(QuestEditorDevLog, Warning, TEXT("UBaseQuestEditor::UpdateTaskTagsIgnored - Task %i is invalid."), TasksIn[i].TaskID);
			}
		}
		else
		{
			UE_LOG(QuestEditorDevLog, Warning, TEXT("UBaseQuestEditor::UpdateTaskTagsIgnored - Talk Task provided instead of Task."));
		}
	}

	//Out variables update
	TaskIDsOut = TaskIDsLocal;
	TasksOut = TasksLocal;	
}

void UBaseQuestEditor::UpdateTaskShortDesc(UPARAM(ref)FQuest& QuestIn, const TArray <FQuestTaskID>& TasksIn, FText ShortDescIn, TArray <FQuestTaskID>& TaskIDsOut, TArray <FQuestTask>& TasksOut)
{
	TArray <FQuestTaskID, TInlineAllocator<3>> TaskIDsLocal;
	TArray <FQuestTask, TInlineAllocator<3>> TasksLocal;

	for (int i = 0; i < TasksIn.Num(); i++)
	{
		//Task is Task type
		if (TasksIn[i].TaskType == EQuestTaskType::Task)
		{
			if (UQuestsFunctionLibrary::IsTaskIDValidInQuest(QuestIn, TasksIn[i]))
			{
				//Update the task
				QuestIn.Tasks[TasksIn[i].TaskID].TaskShortDesc = ShortDescIn;
				//Updated tasks track
				TaskIDsLocal.Add(TasksIn[i]);
				TasksLocal.Add(QuestIn.Tasks[TasksIn[i].TaskID]);
			}
			else
			{
				UE_LOG(QuestEditorDevLog, Warning, TEXT("UBaseQuestEditor::UpdateTaskShortDesc - Task %i is invalid."), TasksIn[i].TaskID);
			}
		}
		else
		{
			UE_LOG(QuestEditorDevLog, Warning, TEXT("UBaseQuestEditor::UpdateTaskShortDesc - Talk Task provided instead of Task."));
		}
	}

	//Out variables update
	TaskIDsOut = TaskIDsLocal;
	TasksOut = TasksLocal;	
}

void UBaseQuestEditor::UpdateInverseActivateCond(UPARAM(ref)FQuest& QuestIn, const TArray <FQuestTaskID>& TasksIn, bool InverseCondIn, TArray <FQuestTaskID>& TaskIDsOut, TArray <FQuestTask>& TasksOut)
{
	TArray <FQuestTaskID, TInlineAllocator<3>> TaskIDsLocal;
	TArray <FQuestTask, TInlineAllocator<3>> TasksLocal;

	for (int i = 0; i < TasksIn.Num(); i++)
	{
		//Task is Task type
		if (TasksIn[i].TaskType == EQuestTaskType::Task)
		{
			if (UQuestsFunctionLibrary::IsTaskIDValidInQuest(QuestIn, TasksIn[i]))
			{
				//Update the task
				QuestIn.Tasks[TasksIn[i].TaskID].InverseActivateCondition = InverseCondIn;
				//Updated tasks track
				TaskIDsLocal.Add(TasksIn[i]);
				TasksLocal.Add(QuestIn.Tasks[TasksIn[i].TaskID]);
			}
			else
			{
				UE_LOG(QuestEditorDevLog, Warning, TEXT("UBaseQuestEditor::UpdateInverseActivateCond - Task %i is invalid."), TasksIn[i].TaskID);
			}
		}
		else
		{
			UE_LOG(QuestEditorDevLog, Warning, TEXT("UBaseQuestEditor::UpdateInverseActivateCond - Talk Task provided instead of Task."));
		}
	}

	//Out variables update
	TaskIDsOut = TaskIDsLocal;
	TasksOut = TasksLocal;
}

void UBaseQuestEditor::UpdateTaskClass(UPARAM(ref)FQuest& QuestIn, const TArray <FQuestTaskID>& TasksIn, TSoftClassPtr<UObject> ClassIn, TArray <FQuestTaskID>& TaskIDsOut, TArray <FQuestTask>& TasksOut)
{
	TArray <FQuestTaskID, TInlineAllocator<3>> TaskIDsLocal;
	TArray <FQuestTask, TInlineAllocator<3>> TasksLocal;

	for (int i = 0; i < TasksIn.Num(); i++)
	{
		//Task is Task type
		if (TasksIn[i].TaskType == EQuestTaskType::Task)
		{
			if (UQuestsFunctionLibrary::IsTaskIDValidInQuest(QuestIn, TasksIn[i]))
			{
				//Update the task
				QuestIn.Tasks[TasksIn[i].TaskID].Payload.ClassInvolved = ClassIn;
				//Updated tasks track
				TaskIDsLocal.Add(TasksIn[i]);
				TasksLocal.Add(QuestIn.Tasks[TasksIn[i].TaskID]);
			}
			else
			{
				UE_LOG(QuestEditorDevLog, Warning, TEXT("UBaseQuestEditor::UpdateTaskClass - Task %i is invalid."), TasksIn[i].TaskID);
			}
		}
		else
		{
			UE_LOG(QuestEditorDevLog, Warning, TEXT("UBaseQuestEditor::UpdateTaskClass - Talk Task provided instead of Task."));
		}
	}

	//Out variables update
	TaskIDsOut = TaskIDsLocal;
	TasksOut = TasksLocal;
}

void UBaseQuestEditor::UpdateTaskVector(UPARAM(ref)FQuest& QuestIn, const TArray <FQuestTaskID>& TasksIn, FVector VectorIn, TArray <FQuestTaskID>& TaskIDsOut, TArray <FQuestTask>& TasksOut)
{
	TArray <FQuestTaskID, TInlineAllocator<3>> TaskIDsLocal;
	TArray <FQuestTask, TInlineAllocator<3>> TasksLocal;

	for (int i = 0; i < TasksIn.Num(); i++)
	{
		//Task is Task type
		if (TasksIn[i].TaskType == EQuestTaskType::Task)
		{
			if (UQuestsFunctionLibrary::IsTaskIDValidInQuest(QuestIn, TasksIn[i]))
			{
				//Update the task
				QuestIn.Tasks[TasksIn[i].TaskID].Payload.VectorInvolved = VectorIn;
				//Updated tasks track
				TaskIDsLocal.Add(TasksIn[i]);
				TasksLocal.Add(QuestIn.Tasks[TasksIn[i].TaskID]);
			}
			else
			{
				UE_LOG(QuestEditorDevLog, Warning, TEXT("UBaseQuestEditor::UpdateTaskVector - Task %i is invalid."), TasksIn[i].TaskID);
			}
		}
		else
		{
			UE_LOG(QuestEditorDevLog, Warning, TEXT("UBaseQuestEditor::UpdateTaskVector - Talk Task provided instead of Task."));
		}
	}

	//Out variables update
	TaskIDsOut = TaskIDsLocal;
	TasksOut = TasksLocal;	
}

void UBaseQuestEditor::UpdateTaskPayload(UPARAM(ref)FQuest& QuestIn, const TArray <FQuestTaskID>& TasksIn, FQuestTaskPayload PayloadIn, TArray <FQuestTaskID>& TaskIDsOut, TArray <FQuestTask>& TasksOut)
{
	TArray <FQuestTaskID, TInlineAllocator<3>> TaskIDsLocal;
	TArray <FQuestTask, TInlineAllocator<3>> TasksLocal;

	for (int i = 0; i < TasksIn.Num(); i++)
	{
		//Task is Task type
		if (TasksIn[i].TaskType == EQuestTaskType::Task)
		{
			if (UQuestsFunctionLibrary::IsTaskIDValidInQuest(QuestIn, TasksIn[i]))
			{
				//Update the task
				QuestIn.Tasks[TasksIn[i].TaskID].Payload = PayloadIn;
				//Updated tasks track
				TaskIDsLocal.Add(TasksIn[i]);
				TasksLocal.Add(QuestIn.Tasks[TasksIn[i].TaskID]);
			}
			else
			{
				UE_LOG(QuestEditorDevLog, Warning, TEXT("UBaseQuestEditor::UpdateTaskPayload - Task %i is invalid."), TasksIn[i].TaskID);
			}
		}
		else
		{
			UE_LOG(QuestEditorDevLog, Warning, TEXT("UBaseQuestEditor::UpdateTaskPayload - Talk Task provided instead of Task."));
		}
	}

	//Out variables update
	TaskIDsOut = TaskIDsLocal;
	TasksOut = TasksLocal;
}

void UBaseQuestEditor::UpdateTaskSoftClass(UPARAM(ref)FQuest& QuestIn, const TArray <FQuestTaskID>& TasksIn, TSoftClassPtr<UBaseQuestTask> TaskClassIn, TArray <FQuestTaskID>& TaskIDsOut, TArray <FQuestTask>& TasksOut)
{
	TArray <FQuestTaskID, TInlineAllocator<3>> TaskIDsLocal;
	TArray <FQuestTask, TInlineAllocator<3>> TasksLocal;

	for (int i = 0; i < TasksIn.Num(); i++)
	{
		//Task is Task type
		if (TasksIn[i].TaskType == EQuestTaskType::Task)
		{
			if (UQuestsFunctionLibrary::IsTaskIDValidInQuest(QuestIn, TasksIn[i]))
			{
				//Update the task
				QuestIn.Tasks[TasksIn[i].TaskID].TaskClass = TaskClassIn;
				//Clear payloads to ensure no extra info is saved.
				//This operation is not "necessary" and forces to set all data again if you change from a task to another of same payload style, for example, if both use a string, the string might be fine for both task, and will be deleted.
				QuestIn.Tasks[TasksIn[i].TaskID].Payload = FQuestTaskPayload();
				QuestIn.Tasks[TasksIn[i].TaskID].CustomPayloadData.Empty();
				//Updated tasks track
				TaskIDsLocal.Add(TasksIn[i]);
				TasksLocal.Add(QuestIn.Tasks[TasksIn[i].TaskID]);
			}
			else
			{
				UE_LOG(QuestEditorDevLog, Warning, TEXT("UBaseQuestEditor::UpdateTaskSoftClass - Task %i is invalid."), TasksIn[i].TaskID);
			}
		}
		else
		{
			UE_LOG(QuestEditorDevLog, Warning, TEXT("UBaseQuestEditor::UpdateTaskSoftClass - Talk Task provided instead of Task."));
		}
	}
		
	//The node is connected to other nodes and is only editing one then i dont have to update. This is a common case
	if (!(QuestIn.Tasks[TasksIn[0].TaskID].SuperiorNodes.Num() == 0 && QuestIn.Tasks[TasksIn[0].TaskID].InferiorNodes.Num() == 0 && TasksIn.Num() == 1))
	{
		//Must update conversations ids in case the task changed from condition to normal or vice
		UQuestEditorDevFunctionLibrary::UpdateAllConversationsIDs(QuestIn);
	}

	//Out variables update
	TaskIDsOut = TaskIDsLocal;
	TasksOut = TasksLocal;	
}

void UBaseQuestEditor::UpdateTaskCustomPayload(UPARAM(ref)FQuest& QuestIn, const TArray <FQuestTaskID>& TasksIn, int PayloadSlotIn, FString PayloadDataIn, TArray <FQuestTaskID>& TaskIDsOut, TArray <FQuestTask>& TasksOut)
{
	TArray <FQuestTaskID, TInlineAllocator<3>> TaskIDsLocal;
	TArray <FQuestTask, TInlineAllocator<3>> TasksLocal;

	TArray <FString, TInlineAllocator<3>> PayloadData;

	for (int i = 0; i < TasksIn.Num(); i++)
	{
		//Task is Task type
		if (TasksIn[i].TaskType == EQuestTaskType::Task)
		{
			if (UQuestsFunctionLibrary::IsTaskIDValidInQuest(QuestIn, TasksIn[i]))
			{
				//get the previous payload
				PayloadData = QuestIn.Tasks[TasksIn[i].TaskID].CustomPayloadData;

				//If valid payload index just replace
				if (PayloadData.IsValidIndex(PayloadSlotIn)) PayloadData[PayloadSlotIn] = PayloadDataIn;
				else
				{
					//resize
					PayloadData.SetNumZeroed(PayloadSlotIn + 1);
					//Set element
					PayloadData[PayloadSlotIn] = PayloadDataIn;
				}

				//Update payload in task
				QuestIn.Tasks[TasksIn[i].TaskID].CustomPayloadData = PayloadData;
				
				//Updated tasks track
				TaskIDsLocal.Add(TasksIn[i]);
				TasksLocal.Add(QuestIn.Tasks[TasksIn[i].TaskID]);
			}
			else
			{
				UE_LOG(QuestEditorDevLog, Warning, TEXT("UBaseQuestEditor::UpdateTaskCustomPayload - Task %i is invalid."), TasksIn[i].TaskID);
			}
		}
		else
		{
			UE_LOG(QuestEditorDevLog, Warning, TEXT("UBaseQuestEditor::UpdateTaskCustomPayload - Talk Task provided instead of Task."));
		}
	}

	//Out variables update
	TaskIDsOut = TaskIDsLocal;
	TasksOut = TasksLocal;
}

bool UBaseQuestEditor::UpdateNodeQEPosition(UPARAM(ref)FQuest& QuestIn, FQuestTaskID TaskID, FVector2D NewPositionIn)
{
	if (!UQuestsFunctionLibrary::IsTaskIDValidInQuest(QuestIn, TaskID))
	{
		UE_LOG(QuestEditorDevLog, Warning, TEXT("UBaseQuestEditor::UpdateNodeQEPosition - Task or Talk task provided of ID %i is invalid."), TaskID.TaskID);
		return false;
	}

	switch (TaskID.TaskType)
	{
	case EQuestTaskType::Task: QuestIn.Tasks[TaskID.TaskID].QuestEditorPosition = NewPositionIn;
		break;
	case EQuestTaskType::TalkTask: QuestIn.TalkTasks[TaskID.TaskID].QuestEditorPosition = NewPositionIn;
		break;
	default:
		break;
	}

	return true;
}

TArray <FVector2D> UBaseQuestEditor::AlignNodesToSides(UPARAM(ref)FQuest& QuestIn, const TArray <FQuestTaskID>& SelectedNodesIn, const TArray <FVector2D>& NodesWidgetSizeIn, bool UseY, bool CheckLower)
{
	//Calculate Position
	FQuestTaskID ExtremeNode = FQuestTaskID();
	int Position = UQuestEditorDevFunctionLibrary::GetPositionValue(QuestIn, SelectedNodesIn, NodesWidgetSizeIn, UseY, CheckLower, ExtremeNode);

	//Half extreme node size
	FVector2D ExtremeNodeSize = NodesWidgetSizeIn[SelectedNodesIn.Find(ExtremeNode)] /2 ;

	TArray <FVector2D> PositionsArray;

	//vector to make things easier
	FVector2D LocalVector = FVector2D();

	for (int i = 0; i < SelectedNodesIn.Num(); i++)
	{
		if (UQuestsFunctionLibrary::IsTaskIDValidInQuest(QuestIn, SelectedNodesIn[i]))
		{
			//Get value of current vector
			switch (SelectedNodesIn[i].TaskType)
			{
			case EQuestTaskType::Task:

				if (UseY)
				{
					//Apply movement in central coordinates
					LocalVector.Y = Position;
					//Conver to QEPosition, only care about one position
					LocalVector = UQuestEditorDevFunctionLibrary::ConvertCenterPositionToQEPosition(NodesWidgetSizeIn[i], LocalVector);
					//Update QE position
					QuestIn.Tasks[SelectedNodesIn[i].TaskID].QuestEditorPosition.Y = LocalVector.Y;
					//Update out var
					PositionsArray.Add(QuestIn.Tasks[SelectedNodesIn[i].TaskID].QuestEditorPosition);
				}
				else
				{
					//Apply movement
					LocalVector.X = Position;
					//Conver to QEPosition, only care about one position
					LocalVector = UQuestEditorDevFunctionLibrary::ConvertCenterPositionToQEPosition(NodesWidgetSizeIn[i], LocalVector);
					//Update QE position
					QuestIn.Tasks[SelectedNodesIn[i].TaskID].QuestEditorPosition.X = LocalVector.X;
					//Update out var
					PositionsArray.Add(QuestIn.Tasks[SelectedNodesIn[i].TaskID].QuestEditorPosition);
				}
				break;
			case EQuestTaskType::TalkTask:
				if (UseY)
				{
					//Apply movement
					LocalVector.Y = Position;
					//Conver to QEPosition, only care about one position
					LocalVector = UQuestEditorDevFunctionLibrary::ConvertCenterPositionToQEPosition(NodesWidgetSizeIn[i], LocalVector);
					//Update QE position
					QuestIn.TalkTasks[SelectedNodesIn[i].TaskID].QuestEditorPosition.Y = LocalVector.Y;
					//Update out var
					PositionsArray.Add(QuestIn.TalkTasks[SelectedNodesIn[i].TaskID].QuestEditorPosition);
				}
				else
				{
					//Apply movement
					LocalVector.X = Position;
					//Conver to QEPosition, only care about one position
					LocalVector = UQuestEditorDevFunctionLibrary::ConvertCenterPositionToQEPosition(NodesWidgetSizeIn[i], LocalVector);
					//Update QE position
					QuestIn.TalkTasks[SelectedNodesIn[i].TaskID].QuestEditorPosition.X = LocalVector.X;
					//Update out var
					PositionsArray.Add(QuestIn.TalkTasks[SelectedNodesIn[i].TaskID].QuestEditorPosition);
				}
			default:
				break;
			}
		}
	}

	return PositionsArray;
}

TArray<FVector2D> UBaseQuestEditor::AlignNodesToGrid(UPARAM(ref)FQuest& QuestIn, const TArray<FQuestTaskID>& SelectedNodesIn, const TArray <FVector2D>& NodesWidgetSizeIn, const FVector2D& GridSizeIn)
{
	//For Calculate Position
	FVector2D Position = FVector2D();

	//Node is used as metric and it wont move. To be consistent it will be added to array out.
	TArray <FVector2D> PositionsArray;
	
	for (int i = 0; i < SelectedNodesIn.Num(); i++)
	{
		//Get node Tier
		Position = UQuestEditorDevFunctionLibrary::GetNodeGridTier(QuestIn, SelectedNodesIn[i], NodesWidgetSizeIn[i], GridSizeIn);
		
		//Multiply to snap to grid. Needs to add first node offset
		Position = (Position * GridSizeIn);

		//I consider this the central position, i need to convert to node position
		Position = UQuestEditorDevFunctionLibrary::ConvertCenterPositionToQEPosition(NodesWidgetSizeIn[i], Position);

		//Update position in quest
		UpdateNodeQEPosition(QuestIn, SelectedNodesIn[i], Position);

		//Add to positions array
		PositionsArray.Add(Position);
	}
	
	return PositionsArray;
}

TArray<FVector2D> UBaseQuestEditor::AlignNodesToEvenDistribution(UPARAM(ref)FQuest& QuestIn, const TArray<FQuestTaskID>& SelectedNodesIn, bool UseY)
{
	//Must have at least 3 nodes to so something
	if (SelectedNodesIn.Num() < 2) return TArray <FVector2D>();

	//Order nodes from lower to higher
	TArray<FQuestTaskID> OrderedNodes = UQuestEditorDevFunctionLibrary::SortNodesByEditorPositiong(QuestIn, SelectedNodesIn, UseY, false);

	int PositionLower = 0;
	int PositionHigher = 0;

	//Determine positions
	if (UseY)
	{
		PositionLower = UQuestEditorDevFunctionLibrary::GetNodePosition(QuestIn, OrderedNodes[0]).Y;
		PositionHigher = UQuestEditorDevFunctionLibrary::GetNodePosition(QuestIn, OrderedNodes.Last()).Y;
	}
	else
	{
		PositionLower = UQuestEditorDevFunctionLibrary::GetNodePosition(QuestIn, OrderedNodes[0]).X;
		PositionHigher = UQuestEditorDevFunctionLibrary::GetNodePosition(QuestIn, OrderedNodes.Last()).X;
	}

	//Set positions for all nodes inbetween.
	FVector2D VectorLocal = FVector2D();
	FVector2D PreviousNodePosition = FVector2D();

	//Determin node separation.
	int Separation = (PositionHigher - PositionLower) / (OrderedNodes.Num()-1);

	for (int i = 0; i < OrderedNodes.Num(); i++)
	{
		//Is valid
		if (UQuestsFunctionLibrary::IsTaskIDValidInQuest(QuestIn, OrderedNodes[i]))
		{
			//Get Node position previous changes
			PreviousNodePosition = UQuestEditorDevFunctionLibrary::GetNodePosition(QuestIn, OrderedNodes[i]);
			//Make vector
			if (UseY)
			{
				VectorLocal.X = PreviousNodePosition.X;
				VectorLocal.Y = PositionLower + (i* Separation);
			}
			else
			{
				//Use X
				VectorLocal.X = PositionLower + (i * Separation);
				VectorLocal.Y = PreviousNodePosition.Y;
			}
			//Update position of node
			UpdateNodeQEPosition(QuestIn, OrderedNodes[i], VectorLocal);
		}
	}

	//Build output array to rearrange nodes in UI
	TArray <FVector2D> PositionsArray;

	//same order as input nodes
	for (int i = 0; i < SelectedNodesIn.Num(); i++)
	{
		//Is valid
		if (UQuestsFunctionLibrary::IsTaskIDValidInQuest(QuestIn, SelectedNodesIn[i]))
		{
			//Add position to output
			PositionsArray.Add(UQuestEditorDevFunctionLibrary::GetNodePosition(QuestIn, SelectedNodesIn[i]));
		}		
	}

	return PositionsArray;
}

TArray<FVector2D> UBaseQuestEditor::DisplaceNodesPosition(UPARAM(ref)FQuest& QuestIn, const TArray<FQuestTaskID>& SelectedNodesIn, float Displacement, bool UseY, bool UseInverseOperation)
{
	//For Calculate Position
	FVector2D Position = FVector2D();

	float Amount = Displacement;

	//If need to use inverse displacemente, invert the sign
	if (UseInverseOperation)
	{
		Amount = -Amount;
	}

	//Node is used as metric and it wont move. To be consistent it will be added to array out.
	TArray <FVector2D> PositionsArray;

	for (int i = 0; i < SelectedNodesIn.Num(); i++)
	{
		//Get node position in editor
		Position = UQuestEditorDevFunctionLibrary::GetNodePosition(QuestIn, SelectedNodesIn[i]);
		
		//Change node position
		if (UseY)
		{
			Position.Y = Position.Y + Amount;
		}
		else
		{
			Position.X = Position.X + Amount;
		}
		
		//Update position in quest
		UpdateNodeQEPosition(QuestIn, SelectedNodesIn[i], Position);

		//Add to positions array
		PositionsArray.Add(Position);
	}

	return PositionsArray;
}

void UBaseQuestEditor::ConnectNodes(UPARAM(ref)FQuest& QuestIn, const TArray<FQuestTaskID>& SelectedNodesIn)
{
	//at least 2 nodes
	if (SelectedNodesIn.Num() < 2) return;

	//sort nodes
	TArray<FQuestTaskID> OrderedNodesLocal = UQuestEditorDevFunctionLibrary::SortNodesByEditorPositiong(QuestIn, SelectedNodesIn, true, true);

	FQuestTaskID PreviousNode = FQuestTaskID();

	//Loop starts in second element, manually save first previous
	PreviousNode = OrderedNodesLocal[0];

	for (int i = 1 ; i < OrderedNodesLocal.Num(); i++)
	{
		//connect
		UQuestEditorDevFunctionLibrary::CreateNewLinkBetweenNodes(QuestIn, PreviousNode, OrderedNodesLocal[i]);
		//Update previous node for next operation
		PreviousNode = OrderedNodesLocal[i];
	}

	//Update all conversations
	UQuestEditorDevFunctionLibrary::UpdateAllConversationsIDs(QuestIn);
	return;
}

void UBaseQuestEditor::ConnectNodesGridLayout(UPARAM(ref)FQuest& QuestIn, const TArray<FQuestTaskID>& SelectedNodesIn, const TArray<FVector2D>& NodesWidgetSizeIn, const FVector2D& GridSizeIn)
{
	//Need at least 2 nodes
	if (SelectedNodesIn.Num() < 2) return;

	FVector2D Position = FVector2D::ZeroVector;

	//All diferent tiers, to know the hierarchy. Use map to sort easy.
	TArray <int32> YTiers;

	//TMap Of node and tier
	TMap <FQuestTaskID, int32> TaskIDsTierMap;
	
	bool Added = false;

	//Determine tier of all nodes
	for (int i = 0; i < SelectedNodesIn.Num(); i++)
	{
		//Calculate grid tier
		Position = UQuestEditorDevFunctionLibrary::GetNodeGridTier(QuestIn, SelectedNodesIn[i], NodesWidgetSizeIn[i], GridSizeIn);

		//Add unique to tier list
		if (!YTiers.Contains(Position.Y))
		{
			//Add if first
			if (YTiers.Num() == 0)
			{
				
				YTiers.AddUnique(Position.Y);
			}
			//If not first in array, need to search the position to insert in order.
			else
			{
				//reset variables
				Added = false;

				//Search where to fit the new tier in order
				for (int j = 0; j < YTiers.Num(); j++)
				{
					//If the current one is bigger or eqaul, then insert in front of it
					if (YTiers[j] >= Position.Y)
					{
						YTiers.Insert(Position.Y, j);
						Added = true;
						break;
					}						
				}

				//If did not add yet, add last
				if (!Added)
				{
					//Add last
					YTiers.AddUnique(Position.Y);
				}
			}
			
		}

		//Node tiers maps populate
		TaskIDsTierMap.Add(SelectedNodesIn[i], Position.Y);
	}

	//Arrays to hold reelevant nodes
	TArray <FQuestTaskID> CurrentTierNodes = TArray <FQuestTaskID>();
	TArray <FQuestTaskID> NextTierNodes = TArray <FQuestTaskID>();

	//Convert map into 2 arrays.
	TArray <int32> ValueArray = TArray <int32>();
	TArray <FQuestTaskID> KeyArray = TArray <FQuestTaskID>();
	TaskIDsTierMap.GenerateValueArray(ValueArray);
	TaskIDsTierMap.GenerateKeyArray(KeyArray); 

	//Loop through tier hierarchy

	//Need at least 2 different tiers
	if (YTiers.Num() < 2) return;

	//Loop tiers. One less loop since the last one wont have next tier
	for (int i = 0; i < YTiers.Num()-1; i++)
	{
		//Get all nodes of current tier
		CurrentTierNodes.Empty();
		
		for (int j = 0; j < ValueArray.Num(); j++)
		{
			//This node has same tier of the current one?
			if (ValueArray[j] == YTiers[i])
			{
				//Add node from key array paralel
				CurrentTierNodes.Add(KeyArray[j]);
			}
		}		

		//Get all nodes of next tier if possible
		NextTierNodes.Empty();

		for (int j = 0; j < ValueArray.Num(); j++)
		{
			//This node has same tier of the current one?
			if (ValueArray[j] == YTiers[i+1])
			{
				//Add node from key array paralel
				NextTierNodes.Add(KeyArray[j]);
			}
		}
		
		//Connect nodes, Current nodes are superior of all NextNodes.
		for (int j = 0; j < CurrentTierNodes.Num(); j++)
		{
			for (int k = 0; k < NextTierNodes.Num(); k++)
			{
				UQuestEditorDevFunctionLibrary::CreateNewLinkBetweenNodes(QuestIn, NextTierNodes[k], CurrentTierNodes[j]);
			}
		}		
	}
		
	//Update all conversations
	UQuestEditorDevFunctionLibrary::UpdateAllConversationsIDs(QuestIn);

	return;
}

void UBaseQuestEditor::ConnectNodesDisabling(UPARAM(ref)FQuest& QuestIn, const TArray<FQuestTaskID>& SelectedNodesIn)
{
	//at least 2 nodes
	if (SelectedNodesIn.Num() < 2) return;

	for (int i = 0; i < SelectedNodesIn.Num(); i++)
	{
		for (int j = 0; j < SelectedNodesIn.Num(); j++)
		{
			//Do not disable self
			if (i != j)
			{
				UQuestEditorDevFunctionLibrary::CreateDisableLinkBetweenNodes(QuestIn, SelectedNodesIn[i], SelectedNodesIn[j]);
			}			
		}
	}
}

void UBaseQuestEditor::RemoveSelectedNodesConnections(UPARAM(ref)FQuest& QuestIn, const TArray<FQuestTaskID>& SelectedNodesIn)
{
	for (int i = 0; i < SelectedNodesIn.Num(); i++)
	{
		UQuestEditorDevFunctionLibrary::BreakAllConnectionOfNode(QuestIn, SelectedNodesIn[i]);
	}

	//Update conversations
	UQuestEditorDevFunctionLibrary::UpdateAllConversationsIDs(QuestIn);

	return;
}

void UBaseQuestEditor::RemoveSelectedNodesDisableConnections(UPARAM(ref)FQuest& QuestIn, const TArray<FQuestTaskID>& SelectedNodesIn)
{
	for (int i = 0; i < SelectedNodesIn.Num(); i++)
	{
		//Superior node goes as superior node of inferior nod
		switch (SelectedNodesIn[i].TaskType)
		{
		case EQuestTaskType::Task:
		{
			QuestIn.Tasks[SelectedNodesIn[i].TaskID].DisabledNodesOnComplete.Empty();
		}

		break;
		case EQuestTaskType::TalkTask:
		{
			QuestIn.TalkTasks[SelectedNodesIn[i].TaskID].DisabledNodesOnComplete.Empty();
		}
		break;
		default:
			break;
		}
	}
}

void UBaseQuestEditor::ToggleSelectedNodesConnectionsType(UPARAM(ref)FQuest& QuestIn, const TArray<FQuestTaskID>& SelectedNodesIn)
{
	//at least 2 nodes
	if (SelectedNodesIn.Num() < 1) return;

	TArray<FQuestTaskID> InferiorNodes;
	InferiorNodes.Empty();
	TArray<FQuestTaskID> SuperiorNodes;
	SuperiorNodes.Empty();
	TArray<ENodeLinkType> LinkTypesLocal = TArray<ENodeLinkType>();

	//Find all connections between nodes
	UQuestEditorDevFunctionLibrary::GetAllSelectedNodesConnections(QuestIn, SelectedNodesIn, SuperiorNodes, InferiorNodes, LinkTypesLocal);

	for (int i = 0; i < InferiorNodes.Num(); i++)
	{
		UQuestEditorDevFunctionLibrary::ChangeLinkTypeBetweenNodes(QuestIn, SuperiorNodes[i], InferiorNodes[i]);
	}

}

void UBaseQuestEditor::SwapNodeConnectionsAndPrintErrors(UPARAM(ref)FQuest& QuestIn, const FQuestTaskID& NodeA, const FQuestTaskID& NodeB, ENodeConnectorType ConnectorIn)
{
	TArray <FString> Messages = UQuestEditorDevFunctionLibrary::SwapNodeConnections(QuestIn, NodeA, NodeB, ConnectorIn);
	
	for (int i = 0; i < Messages.Num(); i++)
	{
		ShowWarnningMessage(Messages[i]);
	}
}

void UBaseQuestEditor::HandleCopyPasteNodes(UPARAM(ref)FQuest& QuestIn, const FQuest& QuestCopiedIn, const TArray<FQuestTaskID>& CopiedNodesIn, const TArray <FVector2D>& NodesWidgetSizeIn, FVector2D MousePastePositionIn, TArray<FQuestTaskID>& PastedNodesOut)
{
	FQuestTaskID LocalTaskID = FQuestTaskID();
	int LocalID = 0;
	TArray<FQuestTaskID> LocalPastedNodesOut = TArray<FQuestTaskID>();

	//Keys are copied nodes, Values are new nodes representing them
	TMap <FQuestTaskID, FQuestTaskID> TaskIDsMap;

	//----------------------------------------------------------------------------------------------------------------
	//----------------------------ADD NODES
	//----------------------------------------------------------------------------------------------------------------
	int PreviousLocalID = -1;
	//Add nodes to "QuestIn" and set values of nodes using the "QuestCopiedIn" information
	for (int i = 0; i < CopiedNodesIn.Num(); i++)
	{
		//Add task of same type
		LocalID = UQuestEditorDevFunctionLibrary::AddTaskOfType(QuestIn, CopiedNodesIn[i].TaskType);
		
		//Add both, the selected node and the last task added to a Map of nodes to node pasted
		LocalTaskID.TaskID = LocalID;
		LocalTaskID.TaskType = CopiedNodesIn[i].TaskType;

		TaskIDsMap.Add(CopiedNodesIn[i], LocalTaskID);
		
		//save nodes added for a out array, to facilitate the widget creation of only new nodes.
		LocalPastedNodesOut.Add(LocalTaskID);

		//Set the new node with ALL the information from the old node using the "QuestCopiedIn" information
		//This information is temporarly inacurated and need correction.
		switch (LocalTaskID.TaskType)
		{
		case EQuestTaskType::Task: 			
			//Do not modify the UniqueID, since it is generated on add.
			PreviousLocalID = QuestIn.Tasks[LocalTaskID.TaskID].UniqueID;
			//Set New Node Information with copied one from CopiedQuest   -----    (No check of valids. Should not be invalid because of how i am saving the quest copied)
			QuestIn.Tasks[LocalTaskID.TaskID] = QuestCopiedIn.Tasks[CopiedNodesIn[i].TaskID];	
			//RestoreID
			QuestIn.Tasks[LocalTaskID.TaskID].UniqueID = PreviousLocalID;
			break;
		case EQuestTaskType::TalkTask:
			//Do not modify the UniqueID, since it is generated on add.
			PreviousLocalID = QuestIn.TalkTasks[LocalTaskID.TaskID].UniqueID;
			//Set New Node Information with copied one from CopiedQuest   -----    (No check of valids. Should not be invalid because of how i am saving the quest copied)
			QuestIn.TalkTasks[LocalTaskID.TaskID] = QuestCopiedIn.TalkTasks[CopiedNodesIn[i].TaskID];
			//RestoreID
			QuestIn.TalkTasks[LocalTaskID.TaskID].UniqueID = PreviousLocalID;
			break;
		default:
			break;
		}
	}
	//----------------------------------------------------------------------------------------------------------------
	//----------------------------Remove all external connections From NEW nodes added
	//----------------------------------------------------------------------------------------------------------------
	//Remove all links that contain a node (superior or inferior or disabled) that is not in the CopiedNodes	
	//Iterate through all new nodes added
	TArray<FQuestTaskLink> LocalLinks = TArray<FQuestTaskLink>();
	TArray<FQuestTaskLink> LocalLinksFiltered = TArray<FQuestTaskLink>();
	FQuestTaskLink LocalLinkInvolved = FQuestTaskLink();
		
	//-----------------------Superior nodes correction
	for (int i = 0; i < LocalPastedNodesOut.Num(); i++)
	{
		//Clear links array for this node
		LocalLinksFiltered.Empty();

		//Get Superior nodes
		LocalLinks = UQuestsFunctionLibrary::GetNodeSuperiorLinksInQuest(QuestIn, LocalPastedNodesOut[i]);

		for (int j = 0; j < LocalLinks.Num(); j++)
		{
			//If node is from selection, needs correction
			//If node is not from selection, needs to be deleted and will not be added to array
			
			//If found means the link is part of the copy 
			if (CopiedNodesIn.Contains(LocalLinks[j].Node))
			{
				//Maped ID
				LocalLinkInvolved.Node = TaskIDsMap[LocalLinks[j].Node];
				LocalLinkInvolved.LinkType = LocalLinks[j].LinkType;
				//Add to new nodes
				LocalLinksFiltered.Add(LocalLinkInvolved);
			}
		}

		//Set superior nodes of this node to corrected version

		switch (LocalPastedNodesOut[i].TaskType)
		{
		case EQuestTaskType::Task:
		{
			QuestIn.Tasks[LocalPastedNodesOut[i].TaskID].SuperiorNodes = LocalLinksFiltered;
		}
		break;
		case EQuestTaskType::TalkTask:
		{
			QuestIn.TalkTasks[LocalPastedNodesOut[i].TaskID].SuperiorNodes = LocalLinksFiltered;
		}
		break;
		default:
			break;
		}
	}

	//-----------------------Inferior nodes correction
	for (int i = 0; i < LocalPastedNodesOut.Num(); i++)
	{
		//Clear links array for this node
		LocalLinksFiltered.Empty();

		//Get inferior nodes
		LocalLinks = UQuestsFunctionLibrary::GetNodeInferiorLinksInQuest(QuestIn, LocalPastedNodesOut[i]);

		for (int j = 0; j < LocalLinks.Num(); j++)
		{
			//If node is from selection, needs correction
			//If node is not from selection, needs to be deleted and will not be added to array

			//If found means the link is part of the copy
			if (CopiedNodesIn.Contains(LocalLinks[j].Node))
			{
				//Maped ID
				LocalLinkInvolved.Node = TaskIDsMap[LocalLinks[j].Node];
				LocalLinkInvolved.LinkType = LocalLinks[j].LinkType;
				//Add to new nodes
				LocalLinksFiltered.Add(LocalLinkInvolved);
			}
		}

		//Set inferior nodes of this node to corrected version

		switch (LocalPastedNodesOut[i].TaskType)
		{
		case EQuestTaskType::Task:
		{
			QuestIn.Tasks[LocalPastedNodesOut[i].TaskID].InferiorNodes = LocalLinksFiltered;
		}
		break;
		case EQuestTaskType::TalkTask:
		{
			QuestIn.TalkTasks[LocalPastedNodesOut[i].TaskID].InferiorNodes = LocalLinksFiltered;
		}
		break;
		default:
			break;
		}
	}

	//Disabled Nodes Correction
	//This are not links, are TasksIDs only and therefore are a bit different

	TArray<FQuestTaskID> LocalIDs = TArray<FQuestTaskID>();
	TArray<FQuestTaskID> LocalIDFiltered = TArray<FQuestTaskID>();
	FQuestTaskID LocalIDInvolved = FQuestTaskID();

	//-----------------------Disabled nodes correction
	for (int i = 0; i < LocalPastedNodesOut.Num(); i++)
	{
		//Clear links array for this node
		LocalIDFiltered.Empty();

		//Get disabled nodes
		LocalIDs = UQuestEditorDevFunctionLibrary::GetDisabledNodesOfNode(QuestIn, LocalPastedNodesOut[i]);

		for (int j = 0; j < LocalIDs.Num(); j++)
		{
			//If node is from selection, needs correction
			//If node is not from selection, needs to be deleted and will not be added to array

			//If found means the link is part of the copy
			if (CopiedNodesIn.Contains(LocalIDs[j]))
			{
				//Maped ID
				LocalIDInvolved = TaskIDsMap[LocalIDs[j]];
				//Add to new nodes
				LocalIDFiltered.Add(LocalIDInvolved);
			}
		}

		//Set superior nodes of this node to corrected version

		switch (LocalPastedNodesOut[i].TaskType)
		{
		case EQuestTaskType::Task:
		{
			QuestIn.Tasks[LocalPastedNodesOut[i].TaskID].DisabledNodesOnComplete = LocalIDFiltered;
		}
		break;
		case EQuestTaskType::TalkTask:
		{
			QuestIn.TalkTasks[LocalPastedNodesOut[i].TaskID].DisabledNodesOnComplete = LocalIDFiltered;
		}
		break;
		default:
			break;
		}
	}

	//----------------------------------------------------------------------------------------------------------------
	//----------------------------NODES POSITIONING
	//----------------------------------------------------------------------------------------------------------------
	
	//-----------------------------------------Adjust New nodes positions
	//Using mouse position, need to adjust the new nodes position.

	int XMin, XMax, YMin, YMax;
	
	//Get all extreme values to draw a box.
	//LocalIDInvolved is used to store nodes, i dont care about the nodes

	//X values
	XMin = UQuestEditorDevFunctionLibrary::GetPositionValue(QuestCopiedIn, CopiedNodesIn, NodesWidgetSizeIn, false, true, LocalIDInvolved);
	XMax = UQuestEditorDevFunctionLibrary::GetPositionValue(QuestCopiedIn, CopiedNodesIn, NodesWidgetSizeIn, false, false, LocalIDInvolved);
	//Y values
	YMin = UQuestEditorDevFunctionLibrary::GetPositionValue(QuestCopiedIn, CopiedNodesIn, NodesWidgetSizeIn, true, true, LocalIDInvolved);
	YMax = UQuestEditorDevFunctionLibrary::GetPositionValue(QuestCopiedIn, CopiedNodesIn, NodesWidgetSizeIn, true, false, LocalIDInvolved);


	//I draw a box between al nodes, and calculate the box coordinates for using with mouse.
	//Calculate box center as the "Previous mouse position"
	FVector2D BoxCenter = FVector2D::ZeroVector;
	BoxCenter.X = ((XMax - XMin) / 2) + XMin;
	BoxCenter.Y = ((YMax - YMin) / 2) + YMin;


	//The nodes "moved" from "previous mouse position" to "new mouse position"
	FVector2D NewPositionOffset = MousePastePositionIn - BoxCenter;

	FVector2D NodePreviousPositionLocal = FVector2D::ZeroVector;	
	//I need to take the mouse position difference and offset the node in that amount

	for (int i = 0; i < LocalPastedNodesOut.Num(); i++)
	{
		//Get the node position before moving.
		NodePreviousPositionLocal = UQuestEditorDevFunctionLibrary::GetNodePosition(QuestIn, LocalPastedNodesOut[i]);
				
		//Set the node position with the offset from the mouse paste position
		NodePreviousPositionLocal = NodePreviousPositionLocal + NewPositionOffset;
		UpdateNodeQEPosition(QuestIn, LocalPastedNodesOut[i], NodePreviousPositionLocal);

	}
	
	//Update all conversation IDs
	UQuestEditorDevFunctionLibrary::UpdateAllConversationsIDs(QuestIn);

	//Out Variables
	PastedNodesOut = LocalPastedNodesOut;
	
	return;
}

bool UBaseQuestEditor::ClearNPCForAllTalkTasks(UPARAM(ref)FQuest& QuestIn)
{

	for (int i = 0; i < QuestIn.TalkTasks.Num(); i++)
	{
		QuestIn.TalkTasks[i].TalkNPC = FNPCQuestDefRep();
	}
	
	return true;
}
