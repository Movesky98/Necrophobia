// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "SessionGameState.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnSessionUpdated);

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnReadyStateUpdated, const FUniqueNetIdRepl& UniqueNetIdRepl, bool bIsReady);

USTRUCT(BlueprintType)
struct FSessionData
{
	GENERATED_BODY()

public:
	FSessionData() : SessionName(FName()), Players(TArray<FUniqueNetIdRepl>()), CurrentPlayer(0), MaxPlayer(0)
	{
	}

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Info")
	FName SessionName;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Info")
	TArray<FUniqueNetIdRepl> Players;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Info")
	int32 CurrentPlayer;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Info")
	int32 MaxPlayer;
};

/**
 * 
 */
UCLASS()
class NECROPHOBIA_API ASessionGameState : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	ASessionGameState();

	virtual void BeginPlay() override;

	FOnSessionUpdated OnSessionUpdated;

	FOnReadyStateUpdated OnReadyStateUpdated;

	void InitializeSessionData(FName SessionName, const TArray<FUniqueNetIdRepl>& Players, int32 CurrentPlayer, int32 MaxPlayer);

	void UpdatePlayers(const TArray<FUniqueNetIdRepl>& NewPlayers);

	void OnPlayerReadyStateUpdated(const FUniqueNetIdRepl& UniqueNetId, bool bIsReady);
	
	FName GetSessionName();

	int32 GetMaxPlayer();

	int32 GetCurrentPlayer();

	TArray<FUniqueNetIdRepl> GetPlayers();

	UFUNCTION()
	void OnRep_SessionData();

protected:
	void PrintSessionDataLog();

	UPROPERTY(ReplicatedUsing = OnRep_SessionData)
	FSessionData SessionData;

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
