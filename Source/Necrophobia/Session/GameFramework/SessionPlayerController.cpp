// Fill out your copyright notice in the Description page of Project Settings.


#include "Session/GameFramework/SessionPlayerController.h"
#include "SessionPlayerState.h"

void ASessionPlayerController::UpdateReadyState_Implementation(bool bIsReady)
{
	ASessionPlayerState* SessionPS = GetPlayerState<ASessionPlayerState>();
	if (SessionPS)
	{
		SessionPS->SetIsReady(true);

		SessionPS->OnRep_IsReady();
	}
}
