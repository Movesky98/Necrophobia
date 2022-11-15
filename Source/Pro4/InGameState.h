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
	AInGameState();

	UFUNCTION(BlueprintCallable)
	uint8 GetInGameMinutes();

	void SetInGameMinutes(uint8 min);
	void AddInGameMinutes();

	UFUNCTION(BlueprintCallable)
	uint8 GetInGameDay();
	void SetInGameDay(uint8 day);
	void AddInGameDay();

	UFUNCTION(BlueprintCallable)
	uint8 GetInGameSeconds();
	void SetInGameSeconds(uint8 sec);
	void AddInGameSeconds();

	UFUNCTION(BlueprintCallable)
	bool GetIsNight();
	void SetIsNight(bool TimeState);

	bool GetIsStateChanged();
	void SetIsStateChanged(bool StateChanged_);

	void SubtractSurvivePlayer();
	void AddEscapePlayer();

	uint16 GetEscapePlayer()
	{
		return EscapePlayer;
	}

	void SetEscapePlayer(uint16 CurrentEscapePlayer)
	{
		EscapePlayer = CurrentEscapePlayer;
	}

	uint16 GetSurvivePlayer()
	{
		return SurvivePlayer;
	}

	void SetSurvivePlayer(uint16 CurrentPlayer)
	{
		SurvivePlayer = CurrentPlayer;
	}

	uint16 GetTotalPlayer()
	{
		return TotalPlayer;
	}

	void SetTotalPlayer(uint16 _TotalPlayer)
	{
		TotalPlayer = _TotalPlayer;
	}

	bool GetIsBossSpawn()
	{
		return isBossSpawn;
	}

	void SetIsBossSpawn(bool BossSpawn)
	{
		isBossSpawn = BossSpawn;
	}

	bool GetIsTimeToSpawnBoss()
	{
		return isTimeToSpawnBoss;
	}

	void SetIsTimeToSpawnBoss(bool TimetoSpawnBoss)
	{
		isTimeToSpawnBoss = TimetoSpawnBoss;
	}

	bool GetIsTimeToSpawnItem()
	{
		return bIsTimeToSpawnItem;
	}

	void SetIsTimeToSpawnItem(bool TimeToSpawnItem)
	{
		bIsTimeToSpawnItem = TimeToSpawnItem;
	}

	bool GetIsHelicopterSpawn()
	{
		return isHelicopterSpawn;
	}

	void SetIsHelicopterSpawn(bool HelicopterSpawn)
	{
		isHelicopterSpawn = HelicopterSpawn;
	}

	void SpawnPlayerToStartLocation(TArray<FVector> SpawnArray);

private:
	UPROPERTY(Replicated)
	uint8 InGameDay = 1;

	UPROPERTY(Replicated)
	uint8 InGameMin = 0;

	UPROPERTY(Replicated)
	uint8 InGameSec = 0;

	UPROPERTY(Replicated)
	bool isNight = false;

	UPROPERTY(Replicated)
	bool isStateChanged = false;

	UPROPERTY(Replicated)
	bool isBossSpawn = false;

	UPROPERTY(Replicated)
	bool isTimeToSpawnBoss = false;

	UPROPERTY(Replicated)
	uint16 SurvivePlayer;
	
	UPROPERTY(Replicated)
	uint16 TotalPlayer;

	UPROPERTY(Replicated)
	uint16 EscapePlayer;

	UPROPERTY(Replicated)
	bool isHelicopterSpawn = false;

	bool bIsTimeToSpawnItem = false;
};
