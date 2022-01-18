// Copyright Epic Games, Inc. All Rights Reserved.

#include "Pro4GameMode.h"
#include "Pro4PlayerController.h"
#include "Pro4Character.h"

APro4GameMode::APro4GameMode()
{
	// set default pawn class to our Blueprinted character
	DefaultPawnClass = APro4Character::StaticClass();
	PlayerControllerClass = APro4PlayerController::StaticClass();
}
