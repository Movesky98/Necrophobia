// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Pro4.h"
#include "BehaviorTree/BTService.h"
#include "Boss_BTService_SearchTarget.generated.h"

/**
 *
 */
UCLASS()
class PRO4_API UBoss_BTService_SearchTarget : public UBTService
{
	GENERATED_BODY()

public:
	UBoss_BTService_SearchTarget();

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
