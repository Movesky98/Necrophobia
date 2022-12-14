// Copyright Epic Games, Inc. All Rights Reserved.

#include "NecrophobiaGameMode.h"
#include "NecrophobiaCharacter.h"
#include "UObject/ConstructorHelpers.h"

ANecrophobiaGameMode::ANecrophobiaGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Characters/TestCharacters/BP_TestCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
