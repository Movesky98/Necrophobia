// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Pro4.h"
#include "GameFramework/GameMode.h"
#include "InGameMode.generated.h"

/**
 * �ΰ��ӿ��� ����Ǵ� ���Ӹ��
 */
UCLASS()
class PRO4_API AInGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	AInGameMode();

	/* �÷��̾ ���ǿ� �����ϰ� �α��� �� �� ����Ǵ� �Լ� */
	virtual void PostLogin(APlayerController* NewPlayer) override;
	/* �÷��̾ ���ǿ��� ������ �� ����Ǵ� �Լ� */
	virtual void Logout(AController* Exiting) override;

protected:
	void BeginPlay() override;
	void Tick(float DeltaTime) override;

private:
	TSubclassOf<class UUserWidget> PlayerClass;

	class AInGameState* InGameState;
	class UNecrophobiaGameInstance* InGameInstance;

	void StartGame();

	uint32 NumberOfPlayers = 0;
	bool isSetStartTimer = false;
	int CountSeconds = 30;

	float Time = 0;

	FTimerHandle GameStartTimer;

	void CountingTheSeconds();
};
