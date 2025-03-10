// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Pro4.h"
#include "GameFramework/PlayerController.h"
#include "Pro4PlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PRO4_API APro4PlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	APro4PlayerController();

	UFUNCTION(BlueprintImplementableEvent)
	void SetServerToSpectator();

	bool SetHelicopterSpawn();

	uint16 SetPlayerRankning(bool isEscape); 
	uint16 GetTotalRanking();
	
	UFUNCTION(Server, Reliable)
	void SubtractSurvivePlayerOnServer();

	UFUNCTION(Server, Reliable)
	void AddEscapePlayerOnServer();

	UFUNCTION(Server, Reliable)
	void RequestSpawnHelicopterOnServer();

	void RemoveServerUI();

protected:

	// 플레이어가 캐릭터에 빙의하기 위한 함수
	virtual void OnPossess(APawn* InPawn);

	void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	void UpdatePlayerTimeState();
	float Time = 0.0f;

	bool bIsServer = false;

	class AInGameState* InGameState;
	class UPlayerMenu* PlayerMenu;
};
