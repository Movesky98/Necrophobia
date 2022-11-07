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

	// ĳ���� ����
	void SetPlayerCharacter(class APro4Character* _Character)
	{
		PlayerCharacter = Cast<class APro4Character>(_Character);
	}

	// ���� ĳ���� ������ ���� �Լ�
	AActor* GetPlayerCharacter()
	{
		return Cast<AActor>(PlayerCharacter);
	}

	UFUNCTION(BlueprintImplementableEvent)
	void SetServerToSpectator();

protected:

	// �÷��̾ ĳ���Ϳ� �����ϱ� ���� �Լ�
	virtual void OnPossess(APawn* InPawn);

	void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	void UpdatePlayerTimeState();
	float Time = 0.0f;

	bool bIsServer = false;

	// ������ ĳ���� Ŭ������ ����� ����, ���¸� �����ϰ� ������ Ŭ���� ������
	class APro4Character* PlayerCharacter;

	class AInGameState* InGameState;
	class UPlayerMenu* PlayerMenu;
};
