// Fill out your copyright notice in the Description page of Project Settings.


#include "Session/GameFramework/SessionPlayerState.h"
#include "Session/GameFramework/Subsystem/SessionEventBus.h"

#include "Net/UnrealNetwork.h"

void ASessionPlayerState::SetIsReady(bool bIsNewReady)
{
	bIsReady = bIsNewReady;
}

bool ASessionPlayerState::GetIsReady()
{
	return bIsReady;
}

void ASessionPlayerState::OnRep_IsReady()
{
	UWorld* World = GetWorld();
	if (!World) return;
	
	USessionEventBus* SessionEventBus = World->GetGameInstance()->GetSubsystem<USessionEventBus>();
	if (SessionEventBus)
	{
		SessionEventBus->OnPlayerReadyStateUpdated.Broadcast(GetUniqueId(), bIsReady);
	}
}

void ASessionPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASessionPlayerState, bIsReady);
}
