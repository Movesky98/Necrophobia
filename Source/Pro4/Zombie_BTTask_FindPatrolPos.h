// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Zombie_BTTask_FindPatrolPos.generated.h"

/**
 * 
 */
UCLASS()
class PRO4_API UZombie_BTTask_FindPatrolPos : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UZombie_BTTask_FindPatrolPos();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
