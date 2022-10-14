// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Pro4.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Zombie_BTTask_WakeUp.generated.h"

/**
 * 
 */
UCLASS()
class PRO4_API UZombie_BTTask_WakeUp : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UZombie_BTTask_WakeUp();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	bool IsDowning = false;
};
