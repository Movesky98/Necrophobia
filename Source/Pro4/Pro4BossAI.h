// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Pro4.h"
#include "AIController.h"
#include "Pro4BossAI.generated.h"

/**
 *
 */
UCLASS()
class PRO4_API APro4BossAI : public AAIController
{
	GENERATED_BODY()

public:
	APro4BossAI();
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
