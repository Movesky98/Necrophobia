// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameState.h"
#include "InGamePlayerState.h"
#include "Pro4Character.h"

#include "Net/UnrealNetwork.h"

AInGameState::AInGameState()
{
	bReplicates = true;
}

/* �ΰ��� �ð� �� ���� �������� �Լ� */
uint8 AInGameState::GetInGameMinutes() {
	return InGameMin;
}

/* �ΰ��� �ð� �� ���� �����ϴ� �Լ� */
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

/* �ΰ��� �ð� �� ���� �߰��ϴ� �Լ� */
void AInGameState::AddInGameMinutes() {
	InGameMin++;
	if (InGameMin >= 2)
	{
		InGameMin = 0;
	}
}

/* �ΰ��� �ð� �� '��'�� �������� �Լ� */
uint8 AInGameState::GetInGameDay() {
	return InGameDay;
}

/* �ΰ��� �ð� �� '��'�� �����ϴ� �Լ� */
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

/* �ΰ��� �ð� �� '��'�� �߰��ϴ� �Լ� */
void AInGameState::AddInGameDay()
{
	InGameDay++;
}

/* �ΰ��� �ð� �� '��'�� �������� �Լ� */
uint8 AInGameState::GetInGameSeconds() {
	return InGameSec;
}

/* �ΰ��� �ð� �� '��'�� �����ϴ� �Լ�*/
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

/* �ΰ��� �ð� �� '��'�� �߰��ϴ� �Լ� */
void AInGameState::AddInGameSeconds() {
	InGameSec += 1;
	UE_LOG(Pro4, Warning, TEXT("Seconds : %d"), GetInGameSeconds());

	if (InGameSec >= 30 && InGameMin == 1)
	{
		InGameSec = 0;
		InGameMin = 0;

		// �� �� ���ο� ���� ĳ���� ����
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

/* �ΰ��� �ð��� ������ */
bool AInGameState::GetIsNight()
{
	return isNight;
}

/* �ΰ��� �ð��� ������ �����ϴ� �Լ� */
void AInGameState::SetIsNight(bool TimeState)
{
	isNight = TimeState;
}

/* �ΰ��� �ð� ��, �� ���� �ٲ������ ������ �������� �Լ� */
bool AInGameState::GetIsStateChanged()
{
	return isStateChanged;
}

/* �ΰ��� �ð� ��, �� ���� �ٲ������ ������ �����ϴ� �Լ� */
void AInGameState::SetIsStateChanged(bool StateChanged_)
{
	isStateChanged = StateChanged_;
}

/* ���� �����ϸ鼭, �÷��̾��� ���� ��ġ�� �������ִ� �Լ� */
void AInGameState::SpawnPlayerToStartLocation(TArray<FVector> SpawnArray)
{
	// ������ �����ϰ� 1����
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

/* ���� <-> Ŭ���̾�Ʈ ���� �ʿ��� ������ �����ϴ� ���� �����ϴ� �Լ� */
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