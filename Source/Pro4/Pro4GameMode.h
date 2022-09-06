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

	/* 플레이어가 세션에 입장하고 로그인 한 후 실행되는 함수 */
	virtual void PostLogin(APlayerController* NewPlayer) override;
	/* 플레이어가 세션에서 나갔을 때 실행되는 함수 */
	virtual void Logout(AController* Exiting) override;

protected:
	void BeginPlay() override;

private:
	void StartGame();

	uint32 NumberOfPlayers = 0;
	bool isSetStartTimer = false;
	int16 CountSeconds = 30;

	FTimerHandle GameStartTimer;

	void CountingTheSeconds();
};



