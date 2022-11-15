// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Pro4.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Boss_BTTask_Appear.generated.h"

/**
 * 
 */
UCLASS()
class PRO4_API UBoss_BTTask_Appear : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBoss_BTTask_Appear();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	// 애니메이션 컨트롤을 위한 변수
	bool IsAttacking = false;
	int32 AttackNum;
};
