// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameState.h"
#include "InGamePlayerState.h"
#include "Pro4Character.h"

#include "Net/UnrealNetwork.h"

AInGameState::AInGameState()
{
	bReplicates = true;
}

/* 인게임 시간 내 분을 가져오는 함수 */
uint8 AInGameState::GetInGameMinutes() {
	return InGameMin;
}

/* 인게임 시간 내 분을 설정하는 함수 */
void AInGameState::SetInGameMinutes(uint8 min) {
	if (min < 0 || min > 2)
	{
		UE_LOG(LogTemp, Warning, TEXT("InGameMin Preprocessing Error"));
	}
	else
	{
		InGameMin = min;
	}
}

/* 인게임 시간 내 분을 추가하는 함수 */
void AInGameState::AddInGameMinutes() {
	InGameMin++;
	if (InGameMin >= 2)
	{
		InGameMin = 0;
	}
}

/* 인게임 시간 내 '일'을 가져오는 함수 */
uint8 AInGameState::GetInGameDay() {
	return InGameDay;
}

/* 인게임 시간 내 '일'을 설정하는 함수 */
void AInGameState::SetInGameDay(uint8 day) {
	if (day < 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("InGameDay Preprocessing Error"));
	}
	else
	{
		InGameDay = day;
	}
}

/* 인게임 시간 내 '일'을 추가하는 함수 */
void AInGameState::AddInGameDay()
{
	InGameDay++;
}

/* 인게임 시간 내 '초'를 가져오는 함수 */
uint8 AInGameState::GetInGameSeconds() {
	return InGameSec;
}

/* 인게임 시간 내 '초'를 설정하는 함수*/
void AInGameState::SetInGameSeconds(uint8 sec) {
	if (sec < 0 || sec > 60)
	{
		UE_LOG(LogTemp, Warning, TEXT("InGameSec Preprocessing Error"));
	}
	else
	{
		InGameSec = sec;
	}
}

/* 인게임 시간 내 '초'를 추가하는 함수 */
void AInGameState::AddInGameSeconds() {
	InGameSec += 1;
	UE_LOG(Pro4, Warning, TEXT("Seconds : %d"), GetInGameSeconds());

	if (InGameSec >= 30 && InGameMin == 1)
	{
		InGameSec = 0;
		InGameMin = 0;

		// 낮 밤 여부에 따라 캐릭터 설정
		if (GetIsNight())
		{
			UE_LOG(Pro4, Warning, TEXT("The day has passed."));
			AddInGameDay();
			SetIsNight(false);
			SetIsTimeToSpawnItem(true);

			bool isServer = true;
			
			for (APlayerState* _PlayerState : PlayerArray)
			{
				if (isServer)
				{
					isServer = false;
					continue;
				}

				AInGamePlayerState* Player = Cast<AInGamePlayerState>(_PlayerState);
				APro4Character* PlayerCharacter = Cast<APro4Character>(Player->GetPawn());
				PlayerCharacter->DetectZombieSpawner(false);
			}
		}
		else
		{
			UE_LOG(Pro4, Warning, TEXT("The night has come."));
			SetIsNight(true);

			bool isServer = true;

			for (APlayerState* _PlayerState : PlayerArray)
			{
				if (isServer)
				{
					isServer = false;
					continue;
				}

				AInGamePlayerState* Player = Cast<AInGamePlayerState>(_PlayerState);
				APro4Character* PlayerCharacter = Cast<APro4Character>(Player->GetPawn());
				PlayerCharacter->DetectZombieSpawner(true);
			}

			if (InGameDay == 2)
			{
				isTimeToSpawnBoss = true;
			}
		}
	}

	if (InGameSec >= 60)
	{
		InGameSec = 0;
		AddInGameMinutes();
		UE_LOG(Pro4, Warning, TEXT("Minutes : %d"), GetInGameMinutes());
	}

	isStateChanged = true;
}

/* 인게임 시간이 밤인지 */
bool AInGameState::GetIsNight()
{
	return isNight;
}

/* 인게임 시간을 밤으로 설정하는 함수 */
void AInGameState::SetIsNight(bool TimeState)
{
	isNight = TimeState;
}

/* 인게임 시간 내, 낮 밤이 바뀌었는지 정보를 가져오는 함수 */
bool AInGameState::GetIsStateChanged()
{
	return isStateChanged;
}

/* 인게임 시간 내, 낮 밤이 바뀌었는지 정보를 설정하는 함수 */
void AInGameState::SetIsStateChanged(bool StateChanged_)
{
	isStateChanged = StateChanged_;
}

/* 게임 시작하면서, 플레이어의 시작 위치를 설정해주는 함수 */
void AInGameState::SpawnPlayerToStartLocation(TArray<FVector> SpawnArray)
{
	// 서버를 제외하고 1부터
	int i = 0;

	for (APlayerState* PlayerState : PlayerArray)
	{
		AInGamePlayerState* InGamePlayerState = Cast<AInGamePlayerState>(PlayerState);
		APro4Character* PlayerCharacter = Cast<APro4Character>(InGamePlayerState->GetPawn());

		if (i == 0)
		{
			PlayerCharacter->SetHidden(true);
			PlayerCharacter->GetPlayerController()->SetServerToSpectator();
		}

		PlayerCharacter->SetActorLocation(SpawnArray[i]);
		i++;
	}

	InGameSec = 0;
}

/* 서버 <-> 클라이언트 간에 필요한 정보를 복사하는 것을 설정하는 함수 */
void AInGameState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AInGameState, isBossSpawn);
	DOREPLIFETIME(AInGameState, isTimeToSpawnBoss);
	DOREPLIFETIME(AInGameState, isNight);
	DOREPLIFETIME(AInGameState, InGameSec);
	DOREPLIFETIME(AInGameState, InGameMin);
	DOREPLIFETIME(AInGameState, InGameDay);
	DOREPLIFETIME(AInGameState, isStateChanged);
	DOREPLIFETIME(AInGameState, isHelicopterSpawn);
}