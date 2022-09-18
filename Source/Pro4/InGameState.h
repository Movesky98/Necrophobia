// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Pro4.h"
#include "GameFramework/GameState.h"
#include "InGameState.generated.h"

/**
 * 
 */
UCLASS()
class PRO4_API AInGameState : public AGameState
{
	GENERATED_BODY()
public:
	uint16 GetInGameMinutes();

	void SetInGameMinutes(uint16 min);
	void AddInGameMinutes();

	uint16 GetInGameDay();
	void SetInGameDay(uint16 day);
	void AddInGameDay();

	uint16 GetInGameSeconds();
	void SetInGameSeconds(uint16 sec);
	void AddInGameSeconds();

	bool GetIsNight();
	void SetIsNight(bool TimeState);

private:
	uint16 InGameDay = 0;
	uint16 InGameMin = 0;
	uint16 InGameSec = 0;
	bool isNight = false;
};
