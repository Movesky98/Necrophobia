// Fill out your copyright notice in the Description page of Project Settings.


#include "Pro4PlayerController.h"
#include "Pro4Character.h"
#include "InGameState.h"
#include "NecrophobiaGameInstance.h"
#include "UserInterface/PlayerMenu.h"
#include "Item/AArmor.h"
#include "Item/AWeapon.h"
#include "Item/AGrenade.h"

APro4PlayerController::APro4PlayerController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void APro4PlayerController::BeginPlay()
{
	Super::BeginPlay();
	InGameState = GetWorld()->GetGameState<AInGameState>();
	UNecrophobiaGameInstance* InGameInstance = Cast<UNecrophobiaGameInstance>(GetGameInstance());

	if (InGameInstance != nullptr)
	{
		this->PlayerMenu = InGameInstance->PlayerMenu;
	}

	if (GetWorld()->IsServer())
	{
		bIsServer = true;
	}

}

void APro4PlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// �ð� ���̰� ���� ���� �ʿ�.
	if (!bIsServer)
	{
		Time += DeltaTime;
		if (Time >= 1.0f)
		{
			Time = 0.0f;
			UpdatePlayerTimeState();
		}
	}
}

void APro4PlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

}

/* �ΰ��� �� �ð��� ������ִ� �Լ�. */
void APro4PlayerController::UpdatePlayerTimeState()
{
	if (IsLocalPlayerController())
	{
		PlayerMenu->SetTimeText(InGameState->GetInGameMinutes(), InGameState->GetInGameSeconds());

		// �ΰ��� �ð��� ���̵Ǹ� ������ ���̵Ǹ� ������
		if (InGameState->GetIsNight())
		{
			PlayerMenu->SetImage(PlayerMenu->Night);
		}
		else
		{
			PlayerMenu->SetImage(PlayerMenu->Day);
		}
	}
}

/* �︮���͸� �����ϴ� �Լ� */
bool APro4PlayerController::SetHelicopterSpawn()
{
	if (InGameState->GetIsHelicopterSpawn())
	{
		return false;
	}
	else
	{
		InGameState->SetIsHelicopterSpawn(true);
		return true;
	}
}

/* �︮���� ������ ������ ��û�ϴ� �Լ� */
void APro4PlayerController::RequestSpawnHelicopterOnServer_Implementation()
{
	InGameState->SetIsHelicopterSpawn(false);
}

/* 
* �÷��̾� ���� ���� ����
* Ż�⿡ ������ ���� �׳� ������� ���
* ���� �ٸ� ������� ������ ����.
*/
uint16 APro4PlayerController::SetPlayerRankning(bool isEscape)
{
	uint16 ThisPlayerRanking;

	if (isEscape)
	{
		// Ż������ ���
		ThisPlayerRanking = InGameState->GetEscapePlayer() + 1;

		/* �÷��̾ Ż������ ���, ������ Ż���� �÷��̾��� ���� +1 ��ŭ �÷���
		* �ٵ� �������� +1 �÷��ְ� �� ������ Ŭ���̾�Ʈ�� �ѷ��ִ� �ð��� ���� �ɸ��� ������ 
		* ������ ���� +1 ��û�� �ϰ� �ٷ� ���� �̾Ƴ��� ����ȭ���� ���� ���� ���� �� �ֱ⿡ ������ ���� �ۼ���.
		*/

		AddEscapePlayerOnServer();
	}
	else
	{
		// �÷��̾ ������� ���
		ThisPlayerRanking = InGameState->GetSurvivePlayer() + InGameState->GetEscapePlayer();
		SubtractSurvivePlayerOnServer();
	}

	return ThisPlayerRanking;
}

/* �÷��̾� ���� ���� */
uint16 APro4PlayerController::GetTotalRanking()
{
	return InGameState->GetTotalPlayer();
}

/* �����ִ� �÷��̾� ���� �����ϴ� �Լ� */
void APro4PlayerController::SubtractSurvivePlayerOnServer_Implementation()
{
	InGameState->SubtractSurvivePlayer();
}

/* Ż���� �÷��̾� ���� �����ϴ� �Լ� */
void APro4PlayerController::AddEscapePlayerOnServer_Implementation()
{
	InGameState->AddEscapePlayer();
}

void APro4PlayerController::RemoveServerUI()
{
	PlayerMenu->RemoveFromViewport();
}