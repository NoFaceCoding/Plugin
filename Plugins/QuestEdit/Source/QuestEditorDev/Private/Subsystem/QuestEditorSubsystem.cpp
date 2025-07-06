//Copyright 2020 Marchetti S. Alfredo I. All Rights Reserved.

#include "Subsystem/QuestEditorSubsystem.h"
#include "QuestEditor/BaseQuestEditor.h"

void UQuestEditorSubsystem::SetQuestEditorReference(UBaseQuestEditor* InQuestEditorReference)
{
	QuestEditorReference = InQuestEditorReference;
}

UBaseQuestEditor* UQuestEditorSubsystem::GetQuestEditorReference() const
{
	return QuestEditorReference;
}
