// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Pro4.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_ZombieAttackRange.generated.h"

/**
 * 
 */
UCLASS()
class PRO4_API UBTDecorator_ZombieAttackRange : public UBTDecorator
{
	GENERATED_BODY()
	
public:
	UBTDecorator_ZombieAttackRange();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
