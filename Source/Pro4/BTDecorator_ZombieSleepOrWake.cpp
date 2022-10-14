// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_ZombieSleepOrWake.h"
#include "Pro4ZombieAI.h"
#include "Pro4Zombie.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTDecorator_ZombieSleepOrWake::UBTDecorator_ZombieSleepOrWake()
{
	NodeName = TEXT("IsSleep");
}

bool UBTDecorator_ZombieSleepOrWake::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	auto ControllingPawn = Cast<APro4Zombie>(OwnerComp.GetAIOwner()->GetPawn());
	if (ControllingPawn == nullptr)
		return false;
	return ControllingPawn->ZombieDowning();
}