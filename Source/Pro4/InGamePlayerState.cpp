// Fill out your copyright notice in the Description page of Project Settings.


#include "InGamePlayerState.h"

#include "Net/UnrealNetwork.h"

AInGamePlayerState::AInGamePlayerState()
{
	PlayerKill = 0;
	ZombieKill = 0;
	IsDead = false;
}

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

void AInGamePlayerState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AInGamePlayerState, IsDead);
	DOREPLIFETIME(AInGamePlayerState, PlayerKill);
	DOREPLIFETIME(AInGamePlayerState, ZombieKill);
}