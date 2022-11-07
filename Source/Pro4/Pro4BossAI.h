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
	// 캐릭터 클래스에 빙의하고 벗어나기 위한 함수
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

private:
	void OnRepeatTimer();

	FTimerHandle RepeatTimerHandle;
	float RepeatInterval;

	// AI가 행동을 결정하기 위해 사용할 블랙보드와 비헤이비어 트리
	UPROPERTY()
		class UBehaviorTree* BehaviorTree;

	UPROPERTY()
		class UBlackboardData* BlackboardData;
};
