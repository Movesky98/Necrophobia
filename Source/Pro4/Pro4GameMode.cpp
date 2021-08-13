// Copyright Epic Games, Inc. All Rights Reserved.

#include "Pro4GameMode.h"
#include "Pro4Character.h"
#include "UObject/ConstructorHelpers.h"

APro4GameMode::APro4GameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
