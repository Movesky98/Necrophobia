// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Pro4.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Boss_BTTask_Attack.generated.h"

/**
 *
 */
UCLASS()
class PRO4_API UBoss_BTTask_Attack : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBoss_BTTask_Attack();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	bool IsAttacking = false;
};
