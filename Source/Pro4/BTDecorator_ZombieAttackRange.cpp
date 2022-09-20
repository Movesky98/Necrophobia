// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_ZombieAttackRange.h"
#include "Pro4Character.h"
#include "Pro4ZombieAI.h"
#include "Pro4Zombie.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTDecorator_ZombieAttackRange::UBTDecorator_ZombieAttackRange()
{
	NodeName = TEXT("CanAttack");
}

bool UBTDecorator_ZombieAttackRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (ControllingPawn == nullptr)
		return false;

	auto Target = Cast<APro4Character>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName(TEXT("Target"))));
	if (Target == nullptr)
		return false;
	
	return bResult && Target->GetDistanceTo(ControllingPawn) <= 200.0f;
}