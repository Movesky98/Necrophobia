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

	// 빙의할 캐릭터 클래스와 사용할 변수, 상태를 저장하고 가져올 클래스 변수들
	class APro4Character* PlayerCharacter;

	class AInGameState* InGameState;
	class UPlayerMenu* PlayerMenu;
};
