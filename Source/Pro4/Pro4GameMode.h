// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Pro4.h"
#include "GameFramework/GameModeBase.h"
#include "Pro4GameMode.generated.h"

UCLASS()
class APro4GameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	APro4GameMode();

protected:
	void BeginPlay() override;
};



