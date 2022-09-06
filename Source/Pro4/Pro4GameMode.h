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

	/* �÷��̾ ���ǿ� �����ϰ� �α��� �� �� ����Ǵ� �Լ� */
	virtual void PostLogin(APlayerController* NewPlayer) override;
	/* �÷��̾ ���ǿ��� ������ �� ����Ǵ� �Լ� */
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



