//Copyright 2020 Marchetti S. Alfredo I. All Rights Reserved.


#include "TalkTaskCustomization/BaseTalkTaskSelectorWidget.h"


#include "QuestEditor/BaseQuestEditor.h"
#include "QuestEditor/QuestEditorDevFunctionLibrary.h"

UBaseTalkTaskSelectorWidget::UBaseTalkTaskSelectorWidget() : Super()
{
}

void UBaseTalkTaskSelectorWidget::NativeConstruct()
{
	//Get quest editor reference
	QuestEditorReference = UQuestEditorDevFunctionLibrary::FindQuestEditorReference(this);
	//Update on start with task exposed on spawn
	UpdateFromTask(QuestTalkTask, IsDialog);

	Super::NativeConstruct();
}

void UBaseTalkTaskSelectorWidget::SelectorCommitPlayerSay(FText PlayerSayIn)
{
	SelectedPlayerSay.Broadcast(PlayerSayIn);
}

void UBaseTalkTaskSelectorWidget::SelectorCommitNPCResponse(FText NPCResponseIn)
{
	SelectedNPCResponse.Broadcast(NPCResponseIn);
}

void UBaseTalkTaskSelectorWidget::SelectorCommitShortDescription(FText ShortDescriptionIn)
{
	SelectedShortDescription.Broadcast(ShortDescriptionIn);
}

void UBaseTalkTaskSelectorWidget::SelectorCommitCustomPayloadData(FString NewData)
{
	SelectedCustomPayloadData.Broadcast(NewData);
}

void UBaseTalkTaskSelectorWidget::SelectorCommitPlayerSound(TSoftObjectPtr<USoundCue> NewSoundIn)
{
	SelectedPlayerSound.Broadcast(NewSoundIn);
}

void UBaseTalkTaskSelectorWidget::SelectorCommitNPCSound(TSoftObjectPtr<USoundCue> NewSoundIn)
{
	SelectedNPCSound.Broadcast(NewSoundIn);
}
