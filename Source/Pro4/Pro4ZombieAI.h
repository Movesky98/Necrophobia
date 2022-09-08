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
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

private:
	void OnRepeatTimer();

	FTimerHandle RepeatTimerHandle;
	float RepeatInterval;

	UPROPERTY()
		class UBehaviorTree* BehaviorTree;

	UPROPERTY()
		class UBlackboardData* BlackboardData;
};
