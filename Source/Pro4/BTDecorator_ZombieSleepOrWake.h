// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Pro4.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_ZombieSleepOrWake.generated.h"

/**
 * 
 */
UCLASS()
class PRO4_API UBTDecorator_ZombieSleepOrWake : public UBTDecorator
{
	GENERATED_BODY()

public:
	UBTDecorator_ZombieSleepOrWake();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
