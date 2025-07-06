#include "DialogueDataAsset.h"

const FDialogueCharacter& UDialogueDataAsset::GetCharacterByID(FName ID) const
{
	for (const FDialogueCharacter& Char : Characters)
	{
		if (Char.CharacterID == ID)
		{
			return Char;
		}
	}

	static const FDialogueCharacter Dummy;
	return Dummy;
}