// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "InGamePlayerState.generated.h"

/**
 * 
 */
UCLASS()
class PRO4_API AInGamePlayerState : public APlayerState
{
	GENERATED_BODY()
public:
	AInGamePlayerState();

	void UpdatePlayerKillInfo(const FString& Target, AActor* Player);
	
	bool GetIsDead()
	{
		return IsDead;
	}

	void SetIsDead(bool _IsDead)
	{
		IsDead = _IsDead;
	}

	uint16 GetPlayerKill()
	{
		return PlayerKill;
	}

	void SetPlayerKill(uint16 _PlayerKill)
	{
		PlayerKill = _PlayerKill;
	}

	uint16 GetZombieKill()
	{
		return ZombieKill;
	}

	void SetZombieKill(uint16 _ZombieKill)
	{
		ZombieKill = _ZombieKill;
	}

private:
	UPROPERTY(Replicated)
	bool IsDead;

	UPROPERTY(Replicated)
	uint16 PlayerKill;

	UPROPERTY(Replicated)
	uint16 ZombieKill;
};
