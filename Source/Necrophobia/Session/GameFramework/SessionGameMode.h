// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SessionGameMode.generated.h"

/**
 * 
 */
UCLASS()
class NECROPHOBIA_API ASessionGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	ASessionGameMode();

protected:
	virtual void BeginPlay() override;

	virtual void PostLogin(APlayerController* NewPlayer) override;

	void Logout(AController* Exiting) override;

	void OnRegisterPlayerCompleted(const TArray<FUniqueNetIdRef>& PlayerList);

	void OnUnregisterPlayerCompleted(const TArray<FUniqueNetIdRef>& PlayerList);

	void OnPlayerReadyStateUpdated(const FUniqueNetIdRepl& UniqueNetId, bool bIsReady);

private:
	FDelegateHandle RegisterPlayersCompletedDelegateHandle;

	FDelegateHandle UnregisterPlayersCompleteDelegateHandle;

	FDelegateHandle UpdatedPlayerReadyStateDelegateHandle;
};