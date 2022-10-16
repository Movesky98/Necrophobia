// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Pro4.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_BossAttackRange.generated.h"

/**
 *
 */
UCLASS()
class PRO4_API UBTDecorator_BossAttackRange : public UBTDecorator
{
	GENERATED_BODY()

public:
	UBTDecorator_BossAttackRange();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
