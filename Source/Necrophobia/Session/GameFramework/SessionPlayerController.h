// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SessionPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class NECROPHOBIA_API ASessionPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	UFUNCTION(Server, Reliable)
	void UpdateReadyState(bool bIsReady);
};
