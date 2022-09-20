// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "Zombie_BTService_SearchTarget.generated.h"

/**
 * 
 */
UCLASS()
class PRO4_API UZombie_BTService_SearchTarget : public UBTService
{
	GENERATED_BODY()
	
public:
	UZombie_BTService_SearchTarget();

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
