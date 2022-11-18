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
	
	// 시간 차이가 있음 수정 필요.
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

/* 인게임 내 시간을 출력해주는 함수. */
void APro4PlayerController::UpdatePlayerTimeState()
{
	if (IsLocalPlayerController())
	{
		PlayerMenu->SetTimeText(InGameState->GetInGameMinutes(), InGameState->GetInGameSeconds());

		// 인게임 시간이 밤이되면 밤으로 낮이되면 낮으로
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

/* 헬리콥터를 스폰하는 함수 */
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

/* 헬리콥터 스폰을 서버에 요청하는 함수 */
void APro4PlayerController::RequestSpawnHelicopterOnServer_Implementation()
{
	InGameState->SetIsHelicopterSpawn(false);
}

/* 
* 플레이어 게임 순위 설정
* 탈출에 성공할 경우와 그냥 사망했을 경우
* 서로 다른 방식으로 순위를 정함.
*/
uint16 APro4PlayerController::SetPlayerRankning(bool isEscape)
{
	uint16 ThisPlayerRanking;

	if (isEscape)
	{
		// 탈출했을 경우
		ThisPlayerRanking = InGameState->GetEscapePlayer() + 1;

		/* 플레이어가 탈출했을 경우, 서버로 탈출한 플레이어의 수를 +1 만큼 늘려줌
		* 근데 서버에서 +1 늘려주고 그 정보를 클라이언트로 뿌려주는 시간이 오래 걸리기 때문에 
		* 서버에 먼저 +1 요청을 하고 바로 값을 뽑아내면 동기화되지 않은 값을 받을 수 있기에 다음과 같이 작성함.
		*/

		AddEscapePlayerOnServer();
	}
	else
	{
		// 플레이어가 사망했을 경우
		ThisPlayerRanking = InGameState->GetSurvivePlayer() + InGameState->GetEscapePlayer();
		SubtractSurvivePlayerOnServer();
	}

	return ThisPlayerRanking;
}

/* 플레이어 게임 순위 */
uint16 APro4PlayerController::GetTotalRanking()
{
	return InGameState->GetTotalPlayer();
}

/* 남아있는 플레이어 수를 갱신하는 함수 */
void APro4PlayerController::SubtractSurvivePlayerOnServer_Implementation()
{
	InGameState->SubtractSurvivePlayer();
}

/* 탈출한 플레이어 수를 갱신하는 함수 */
void APro4PlayerController::AddEscapePlayerOnServer_Implementation()
{
	InGameState->AddEscapePlayer();
}

void APro4PlayerController::RemoveServerUI()
{
	PlayerMenu->RemoveFromViewport();
}