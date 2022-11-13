// Fill out your copyright notice in the Description page of Project Settings.


#include "InGamePlayerState.h"

#include "Net/UnrealNetwork.h"

AInGamePlayerState::AInGamePlayerState()
{
	PlayerKill = 0;
	ZombieKill = 0;
	IsDead = false;
}

/* 플레이어가 다른 액터(좀비, 플레이어)를 죽였을 때 기록하는 함수 */
void AInGamePlayerState::UpdatePlayerKillInfo(const FString& Target, AActor* Player)
{
	if (Target == "Player")
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Orange, TEXT("InGamePlayerState (Player) : UpdatePlayerKillInfo"));

		PlayerKill++;
	}
	else if (Target == "Zombie")
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Orange, TEXT("InGamePlayerState (Zombie) : UpdatePlayerKillInfo"));

		ZombieKill++;
	}
}

/* 서버 <-> 클라이언트 간에 복사가 필요한 변수를 설정하는 함수 */
void AInGamePlayerState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AInGamePlayerState, IsDead);
	DOREPLIFETIME(AInGamePlayerState, PlayerKill);
	DOREPLIFETIME(AInGamePlayerState, ZombieKill);
}