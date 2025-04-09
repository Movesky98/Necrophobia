// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/SessionGameMode.h"
#include "GameFramework/SessionGameState.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/SessionGameState.h"

#include "Session/GameFramework/SessionPlayerState.h"
#include "Session/GameFramework/SessionPlayerController.h"
#include "Session/GameFramework/Subsystem/SessionEventBus.h"

#include "GISessionSubsystem.h"
#include "OnlineSubsystemUtils.h"
#include "OnlineSessionSettings.h"

ASessionGameMode::ASessionGameMode()
{
	GameStateClass = ASessionGameState::StaticClass();
	PlayerStateClass = ASessionPlayerState::StaticClass();
	PlayerControllerClass = ASessionPlayerController::StaticClass();
}

void ASessionGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (!HasAuthority())
		return;

	ASessionGameState* SessionGS = GetGameState<ASessionGameState>();
	UGISessionSubsystem* GISSubsystem = GetGameInstance()->GetSubsystem<UGISessionSubsystem>();
	if (SessionGS && GISSubsystem)
	{
		FName SessionName = GISSubsystem->GetSessionName();
		int32 CurrentPlayer = GISSubsystem->GetCurrentPlayer();
		int32 MaxPlayer = GISSubsystem->GetMaxPlayer();

		TArray<FUniqueNetIdRef> RegisteredPlayers = GISSubsystem->GetRegisteredPlayers();
		TArray<FUniqueNetIdRepl> Players;
		for (FUniqueNetIdRef& RegisteredPlayer : RegisteredPlayers)
		{
			FUniqueNetIdRepl Player(RegisteredPlayer);
			Players.Add(Player);
		}

		SessionGS->InitializeSessionData(SessionName, Players, CurrentPlayer, MaxPlayer);
	}

	USessionEventBus* SessionEventBus = GetGameInstance()->GetSubsystem<USessionEventBus>();
	if (SessionEventBus)
	{
		SessionEventBus->OnPlayerReadyStateUpdated.AddUObject(this, &ThisClass::OnPlayerReadyStateUpdated);
	}
}

void ASessionGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if (!HasAuthority()) return;

	// TODO : 세션에 참가한 플레이어의 정보 등록 (SessionGameInstanceSubsystem에)

	UGISessionSubsystem* GISSubsystem = GetGameInstance()->GetSubsystem<UGISessionSubsystem>();
	if (GISSubsystem)
	{
		if (NewPlayer && NewPlayer->PlayerState)
		{
			RegisterPlayersCompletedDelegateHandle = GISSubsystem->OnRegisterPlayersComplete.AddUObject(this, &ThisClass::OnRegisterPlayerCompleted);

			FUniqueNetIdRepl UniqueId = NewPlayer->PlayerState->GetUniqueId();
			GISSubsystem->RegisterPlayer(UniqueId, false);
		}
	}
}

void ASessionGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);


	UGISessionSubsystem* GISSubsystem = GetGameInstance()->GetSubsystem<UGISessionSubsystem>();
	if (GISSubsystem)
	{
		if (Exiting && Exiting->PlayerState)
		{
			UnregisterPlayersCompleteDelegateHandle = GISSubsystem->OnUnregisterPlayersComplete.AddUObject(this, &ThisClass::OnUnregisterPlayerCompleted);

			FUniqueNetIdRepl UniqueNetId = Exiting->PlayerState->GetUniqueId();
			GISSubsystem->UnregisterPlayer(*UniqueNetId);
		}
	}
}

void ASessionGameMode::OnRegisterPlayerCompleted(const TArray<FUniqueNetIdRef>& PlayerList)
{
	UGISessionSubsystem* GISSubsystem = GetGameInstance()->GetSubsystem<UGISessionSubsystem>();
	if (GISSubsystem)
	{
		GISSubsystem->OnRegisterPlayersComplete.Remove(RegisterPlayersCompletedDelegateHandle);
	}

	TArray<FUniqueNetIdRepl> Players;
	for (const FUniqueNetIdRef& Player : PlayerList)
	{
		FUniqueNetIdRepl PlayerRepl(Player);
		Players.Add(PlayerRepl);
	}

	ASessionGameState* SessionGS = GetGameState<ASessionGameState>();
	if (SessionGS)
	{
		SessionGS->UpdatePlayers(Players);
	}
}

void ASessionGameMode::OnUnregisterPlayerCompleted(const TArray<FUniqueNetIdRef>& PlayerList)
{
	UGISessionSubsystem* GISSubsystem = GetGameInstance()->GetSubsystem<UGISessionSubsystem>();
	if (GISSubsystem)
	{
		GISSubsystem->OnUnregisterPlayersComplete.Remove(UnregisterPlayersCompleteDelegateHandle);
	}

	TArray<FUniqueNetIdRepl> Players;
	for (const FUniqueNetIdRef& Player : PlayerList)
	{
		FUniqueNetIdRepl PlayerRepl(Player);
		Players.Add(PlayerRepl);
	}

	ASessionGameState* SessionGS = GetGameState<ASessionGameState>();
	if (SessionGS)
	{
		SessionGS->UpdatePlayers(Players);
	}
}

// 모든 플레이어가 레디를 했는지 확인하는 함수
void ASessionGameMode::OnPlayerReadyStateUpdated(const FUniqueNetIdRepl& UniqueNetId, bool bIsReady)
{
	ASessionGameState* SessionGS = GetGameState<ASessionGameState>();
	if (SessionGS)
	{
		for (TObjectPtr<APlayerState>& PlayerState : SessionGS->PlayerArray)
		{
			ASessionPlayerState* SessionPS = Cast<ASessionPlayerState>(PlayerState);

			if (!SessionPS->GetIsReady())
			{
				// All players are not ready yet.
				return;
			}
		}

		GetWorld()->ServerTravel("/Game/ThirdPerson/Maps/ThirdPersonMap?listen");
	}
}
