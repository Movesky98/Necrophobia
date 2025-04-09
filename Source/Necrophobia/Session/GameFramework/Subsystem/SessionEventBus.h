// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SessionEventBus.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnPlayerReadyStateUpdated, const FUniqueNetIdRepl& UniqueNetIdRepl, bool bIsReady);


/**
 * 
 */
UCLASS()
class NECROPHOBIA_API USessionEventBus : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	FOnPlayerReadyStateUpdated OnPlayerReadyStateUpdated;
};
