// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameFramework/OnlineReplStructs.h"
#include "GameFramework/SessionGameState.h"
#include "SessionDataProvider.generated.h"

// Add Delegate that broadcasts changed session information.
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSessionDataUpdated);

typedef FOnReadyStateUpdated::FDelegate FOnReadyStateUpdatedDelegate;

class ASessionGameState;

/**
 * 
 */
UCLASS()
class NECROPHOBIA_API USessionDataProvider : public UObject
{
	GENERATED_BODY()
	
public:
	FOnSessionDataUpdated OnSessionDataUpdated;

	void Initialize(ASessionGameState* GameState);

	FDelegateHandle AddOnReadyStateUpdatedDelegate_Handle(FOnReadyStateUpdatedDelegate Delegate);

	UFUNCTION(BlueprintCallable)
	FName GetSessionName();

	UFUNCTION(BlueprintCallable)
	int32 GetMaxPlayer();

	UFUNCTION(BlueprintCallable)
	int32 GetCurrentPlayer();

	UFUNCTION(BlueprintCallable)
	TArray<FUniqueNetIdRepl> GetPlayerList();

	UFUNCTION()
	void OnSessionUpdated();
	
	UFUNCTION()
	void OnPlayerStateUpdated();

private:
	ASessionGameState* SessionGameState;
};
