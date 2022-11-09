// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Pro4.h"
#include "AIController.h"
#include "Pro4ZombieAI.generated.h"

/**
 * 
 */
UCLASS()
class PRO4_API APro4ZombieAI : public AAIController
{
	GENERATED_BODY()
	
public:
	APro4ZombieAI();

	UFUNCTION()
	void SetZombieTarget(APawn* Player);

	APawn* GetZombieTarget()
	{
		return Target;
	}

	bool GetIsTracking()
	{
		return bIsTracking;
	}

	void SetIsTracking(bool IsTracking)
	{
		bIsTracking = IsTracking;
	}

	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

private:
	void OnRepeatTimer();

	UPROPERTY()
	bool bIsTracking;

	UPROPERTY()
	APawn* Target;

	FTimerHandle RepeatTimerHandle;
	float RepeatInterval;

	UPROPERTY()
		class UBehaviorTree* BehaviorTree;

	UPROPERTY()
		class UBlackboardData* BlackboardData;
};
