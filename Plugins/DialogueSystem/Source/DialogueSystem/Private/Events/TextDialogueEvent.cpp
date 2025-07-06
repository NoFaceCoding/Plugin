#include "Events/TextDialogueEvent.h"
#include "DialogueWidgetBase.h"
#include "DialogueComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DialogueDataAsset.h"

void UTextDialogueEvent::Execute_Implementation(UDialogueComponent* DialogueComponent, bool bIsEmbedded)
{
	if (!DialogueComponent || !DialogueWidgetClass)
	{
		DialogueComponent->ContinueDialogue();
		return;
	}

	bIsActive = true;
	bIsSkipping = false;
	CachedComponent = DialogueComponent;
	CurrentFragmentIndex = 0;

	APlayerController* PC = UGameplayStatics::GetPlayerController(DialogueComponent, 0);
	if (!PC) return;

	CurrentWidget = CreateWidget<UDialogueWidgetBase>(PC, DialogueWidgetClass);
	if (!CurrentWidget) return;

	CurrentWidget->AddToViewport();
	DialogueComponent->SetActiveDialogueWidget(CurrentWidget);
	CurrentWidget->SetOwningTextEvent(this);

	PlayNextFragment();
}

void UTextDialogueEvent::PlayNextFragment()
{
	if (CurrentFragmentIndex >= Fragments.Num() || !bIsActive || !CachedComponent || !CachedComponent->GetCurrentDialogue())
	{
		return;
	}

	const FDialogueTextFragment& Fragment = Fragments[CurrentFragmentIndex];

	if (Fragment.bIsText)
	{
		const UDialogueDataAsset* Dialogue = CachedComponent->GetCurrentDialogue();
		const FDialogueCharacter& CharData = Dialogue->GetCharacterByID(SpeakingCharacterID);
		FText SpeakerName = CharData.IsValid() ? CharData.DisplayName : FText::FromString("???");

		if (CurrentWidget)
		{
			CurrentWidget->SetSpeakerName(SpeakerName);
			CurrentWidget->AppendVisibleText(Fragment.Text);
		}
	}
	else if (Fragment.EmbeddedEvent)
	{
		Fragment.EmbeddedEvent->Execute(CachedComponent, true);

		++CurrentFragmentIndex;
		PlayNextFragment();
	}
}

void UTextDialogueEvent::NotifyTextFinished()
{
	if (!bIsActive || !CachedComponent)
	{
		return;
	}

	// Timer absichern
	CachedComponent->GetWorld()->GetTimerManager().ClearTimer(PostDelayHandle);

	++CurrentFragmentIndex;

	if (CurrentFragmentIndex >= Fragments.Num())
	{
		// Warten auf Spieler – NICHT automatisch ContinueDialogue!
		// Das Widget bleibt aktiv bis Spieler Continue drückt
		return;
	}

	// Delay nach Fragment, falls gesetzt
	const FDialogueTextFragment& Fragment = Fragments[CurrentFragmentIndex - 1];
	if (Fragment.PostDelay > 0.01f)
	{
		CachedComponent->GetWorld()->GetTimerManager().SetTimer(
			PostDelayHandle,
			this,
			&UTextDialogueEvent::PlayNextFragment,
			Fragment.PostDelay,
			false
		);
	}
	else
	{
		PlayNextFragment();
	}
}
void UTextDialogueEvent::TryContinue()
{
	if (!bIsActive || !CachedComponent) return;

	// Wenn alles bereits sichtbar ist, direkt beenden
	if (CurrentWidget && CurrentWidget->IsTextFullyDisplayed())
	{
		bIsActive = false;

		if (CurrentWidget)
		{
			CurrentWidget->RemoveFromParent();
			CurrentWidget = nullptr;
		}

		CachedComponent->ContinueDialogue();
		return;
	}

	// Noch nicht vollständig? Dann in Skip-Modus wechseln
	if (!bIsSkipping)
	{
		bIsSkipping = true;

		if (CurrentWidget)
		{
			CurrentWidget->InstantComplete();
		}

		CachedComponent->GetWorld()->GetTimerManager().ClearTimer(PostDelayHandle);

		// Manuell restliche Fragmente durchlaufen
		FText CombinedText;

		while (CurrentFragmentIndex < Fragments.Num())
		{
			const FDialogueTextFragment& Fragment = Fragments[CurrentFragmentIndex];

			if (Fragment.bIsText)
			{
				CombinedText = FText::FromString(CombinedText.ToString() + Fragment.Text.ToString());
			}
			else if (Fragment.EmbeddedEvent)
			{
				Fragment.EmbeddedEvent->Execute(CachedComponent, true);
			}

			++CurrentFragmentIndex;
		}

		if (CurrentWidget && !CombinedText.IsEmpty())
		{
			const UDialogueDataAsset* Dialogue = CachedComponent->GetCurrentDialogue();
			const FDialogueCharacter& CharData = Dialogue->GetCharacterByID(SpeakingCharacterID);
			FText SpeakerName = CharData.IsValid() ? CharData.DisplayName : FText::FromString("???");

			CurrentWidget->SetSpeakerName(SpeakerName);
			CurrentWidget->UpdateVisibleText(CombinedText);
		}

		return; // Skip-Modus aktiviert → warte auf nächsten Tastendruck
	}

	// Bereits geskippt → nun endgültig beenden
	bIsActive = false;

	if (CurrentWidget)
	{
		CurrentWidget->RemoveFromParent();
		CurrentWidget = nullptr;
	}

	CachedComponent->ContinueDialogue();
}