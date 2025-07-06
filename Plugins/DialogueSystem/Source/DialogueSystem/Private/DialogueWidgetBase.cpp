#include "DialogueWidgetBase.h"
#include "TimerManager.h"
#include "Events/TextDialogueEvent.h"

void UDialogueWidgetBase::AppendVisibleText(const FText& NewText)
{
	if (NewText.IsEmpty())
		return;

	// Set StartIndex to current length
	CurrentCharIndex = FullText.Len();
	FullText += NewText.ToString();

	// Start timer if not already running
	if (!GetWorld()->GetTimerManager().IsTimerActive(TextDisplayTimer))
	{
		GetWorld()->GetTimerManager().SetTimer(
			TextDisplayTimer,
			this,
			&UDialogueWidgetBase::AppendNextCharacter,
			CharacterDisplayInterval,
			true
		);
	}
}

void UDialogueWidgetBase::AppendNextCharacter()
{
	if (CurrentCharIndex >= FullText.Len())
	{
		GetWorld()->GetTimerManager().ClearTimer(TextDisplayTimer);

		if (OwningTextEvent)
		{
			OwningTextEvent->NotifyTextFinished();
		}
		return;
	}

	CurrentText.AppendChar(FullText[CurrentCharIndex]);
	++CurrentCharIndex;

	UpdateVisibleText(FText::FromString(CurrentText));
}

void UDialogueWidgetBase::NativeDestruct()
{
	GetWorld()->GetTimerManager().ClearTimer(TextDisplayTimer);
	Super::NativeDestruct();
}
void UDialogueWidgetBase::InstantComplete()
{
	// Timer abbrechen
	GetWorld()->GetTimerManager().ClearTimer(TextDisplayTimer);

	// Direkt den gesamten Text anzeigen
	UpdateVisibleText(FText::FromString(FullText));
	CurrentText = FullText;
	CurrentCharIndex = FullText.Len();
}
bool UDialogueWidgetBase::IsTextFullyDisplayed() const
{
	return CurrentCharIndex >= FullText.Len();
}