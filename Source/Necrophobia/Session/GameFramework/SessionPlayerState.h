// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "SessionPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class NECROPHOBIA_API ASessionPlayerState : public APlayerState
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintSetter)
	void SetIsReady(bool bIsNewReady);

	UFUNCTION(BlueprintGetter, BlueprintPure)
	bool GetIsReady();

	UFUNCTION()
	void OnRep_IsReady();

protected:

	UPROPERTY(ReplicatedUsing = OnRep_IsReady)
	bool bIsReady;

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
