// Fill out your copyright notice in the Description page of Project Settings.


#include "SessionGameState.h"
#include "GameFramework/GISessionSubsystem.h"
#include "GameFramework/OnlineReplStructs.h"

#include "Session/GameFramework/Subsystem/SessionEventBus.h"

#include "Net/UnrealNetwork.h"

ASessionGameState::ASessionGameState()
{
	bReplicates = true;
}

void ASessionGameState::BeginPlay()
{
	Super::BeginPlay();

	USessionEventBus* SessionEventBus = GetGameInstance()->GetSubsystem<USessionEventBus>();
	if (SessionEventBus)
	{
		SessionEventBus->OnPlayerReadyStateUpdated.AddUObject(this, &ThisClass::OnPlayerReadyStateUpdated);
	}
}

void ASessionGameState::InitializeSessionData(FName SessionName, const TArray<FUniqueNetIdRepl>& Players, int32 CurrentPlayer, int32 MaxPlayer)
{
	SessionData.SessionName = SessionName;
	SessionData.MaxPlayer = MaxPlayer;
	SessionData.CurrentPlayer = CurrentPlayer;
	SessionData.Players = Players;
}

void ASessionGameState::UpdatePlayers(const TArray<FUniqueNetIdRepl>& NewPlayers)
{
	// Ready 상태를 업데이트할 때, 초기화되도록 되어있음 수정 필요.
	// 이미 레디한 플레이어들이 있을 때, 그 상태가 초기화되지 않도록 해야 함.

	SessionData.Players = NewPlayers;
	SessionData.CurrentPlayer = SessionData.Players.Num();

	OnRep_SessionData();
}

void ASessionGameState::OnPlayerReadyStateUpdated(const FUniqueNetIdRepl& UniqueNetId, bool bIsReady)
{
	if (OnReadyStateUpdated.IsBound())
		OnReadyStateUpdated.Broadcast(UniqueNetId, bIsReady);
}

FName ASessionGameState::GetSessionName()
{
	return SessionData.SessionName;
}

int32 ASessionGameState::GetMaxPlayer()
{
	return SessionData.MaxPlayer;
}

int32 ASessionGameState::GetCurrentPlayer()
{
	return SessionData.CurrentPlayer;
}

TArray<FUniqueNetIdRepl> ASessionGameState::GetPlayers()
{
	return SessionData.Players.Num() ? SessionData.Players : TArray<FUniqueNetIdRepl>();
}

void ASessionGameState::OnRep_SessionData()
{
	// Session 정보가 업데이터 되었을 때, Delegate 호출.
	if(OnSessionUpdated.IsBound())
		OnSessionUpdated.Broadcast();
}

void ASessionGameState::PrintSessionDataLog()
{
	FString Log = FString("SessionGameState:: PrintSessionDataLog() >> \n");
	Log += FString("Current Session Name	: ") + SessionData.SessionName.ToString() + FString("\n");
	Log += FString("Max Player				: ") + FString::FromInt(SessionData.MaxPlayer) + FString("\n");
	Log += FString("Current Player			: ") + FString::FromInt(SessionData.CurrentPlayer) + FString("\n");

	Log += FString("And Player List is \n");
	int32 Count = 1;
	for (auto Player: SessionData.Players)
	{
		if(Player.IsValid())
			Log += FString::FromInt(Count) + FString("th Player : ") + Player.GetUniqueNetId()->ToString() + FString("\n");
	}

	UE_LOG(LogOnlineSubsystem, Warning, TEXT("%s"), *Log);
}

void ASessionGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASessionGameState, SessionData);
}
