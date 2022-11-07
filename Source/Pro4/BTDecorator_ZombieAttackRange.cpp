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

	// AI로 빙의한 좀비 캐릭터 가져오기
	auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (ControllingPawn == nullptr)
		return false;

	// 블랙보드에 타겟으로 지정된 캐릭터 가져오기
	auto Target = Cast<APro4Character>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName(TEXT("Target"))));
	if (Target == nullptr)
		return false;
	
	return bResult && Target->GetDistanceTo(ControllingPawn) <= 200.0f; // 타깃이 존재하고 200f 안에있으면 공격가능
}