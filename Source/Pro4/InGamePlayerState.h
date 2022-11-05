// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "InGamePlayerState.generated.h"

/**
 * 
 */
UCLASS()
class PRO4_API AInGamePlayerState : public APlayerState
{
	GENERATED_BODY()
public:
	void SetIsServer(bool isServer)
	{
		bIsServer = isServer;
	}

	bool GetIsServer()
	{
		return bIsServer;
	}

private:
	bool bIsServer = false;
};
