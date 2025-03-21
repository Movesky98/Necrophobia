// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Pro4.h"
#include "GameFramework/GameMode.h"
#include "InGameMode.generated.h"

/**
 * 인게임에서 실행되는 게임모드
 */
UCLASS()
class PRO4_API AInGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	AInGameMode();

	/* 플레이어가 세션에 입장하고 로그인 한 후 실행되는 함수 */
	virtual void PostLogin(APlayerController* NewPlayer) override;

	/* 플레이어가 세션에서 나갔을 때 실행되는 함수 */
	virtual void Logout(AController* Exiting) override;

protected:
	void BeginPlay() override;
	void Tick(float DeltaTime) override;

private:
	class AInGameState* InGameState;
	class UNecrophobiaGameInstance* InGameInstance;

	void StartGame();

	uint16 NumberOfPlayers;
	bool isGameStart;
	int CountSeconds;

	float Time;
	TArray<AActor*> ItemSpawnerArray;

	FTimerHandle GameStartTimer;

	void CountingTheSeconds();

	void UpdatePlayerTimeStete();

	void SpawnBossZombie();
};
