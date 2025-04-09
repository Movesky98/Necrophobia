// Fill out your copyright notice in the Description page of Project Settings.


#include "SessionDataProvider.h"
#include "GameFramework/SessionGameState.h"

void USessionDataProvider::Initialize(ASessionGameState* GameState)
{
	if (GameState)
	{
		SessionGameState = GameState;
		SessionGameState->OnSessionUpdated.AddUObject(this, &ThisClass::OnSessionUpdated);
	}
}

FDelegateHandle USessionDataProvider::AddOnReadyStateUpdatedDelegate_Handle(FOnReadyStateUpdatedDelegate Delegate)
{
	if (SessionGameState)
	{
		return SessionGameState->OnReadyStateUpdated.Add(Delegate);
	}

	return FDelegateHandle();
}

FName USessionDataProvider::GetSessionName()
{
	if (SessionGameState)
	{
		return SessionGameState->GetSessionName();
	}

	return FName();
}

int32 USessionDataProvider::GetMaxPlayer()
{
	if (SessionGameState)
	{
		return SessionGameState->GetMaxPlayer();
	}

	return int32();
}

int32 USessionDataProvider::GetCurrentPlayer()
{
	if (SessionGameState)
	{
		return SessionGameState->GetCurrentPlayer();
	}

	return int32();
}

TArray<FUniqueNetIdRepl> USessionDataProvider::GetPlayerList()
{
	if (SessionGameState)
	{
		return SessionGameState->GetPlayers();
	}

	return TArray<FUniqueNetIdRepl>();
}

void USessionDataProvider::OnSessionUpdated()
{
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, FString("SessionDataProvider:: OnSessionUpdated() is called."));

	if (OnSessionDataUpdated.IsBound())
	{
		OnSessionDataUpdated.Broadcast();
	}
}

void USessionDataProvider::OnPlayerStateUpdated()
{

}
