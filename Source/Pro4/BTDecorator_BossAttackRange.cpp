// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_BossAttackRange.h"
#include "Pro4Character.h"
#include "Pro4BossAI.h"
#include "Pro4Boss.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTDecorator_BossAttackRange::UBTDecorator_BossAttackRange()
{
	// 블랙보드에서 사용할 이름
	NodeName = TEXT("CanAttack");
}

bool UBTDecorator_BossAttackRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	// AI가 빙의된 보스 캐릭터
	auto ControllingPawn = Cast<APro4Boss>(OwnerComp.GetAIOwner()->GetPawn());
	if (ControllingPawn == nullptr)
	{
		return false;
	}

	// 타겟으로 지정된 플레이어 캐릭터
	auto Target = Cast<APro4Character>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName(TEXT("Target"))));
	if (Target == nullptr) // 지정된 타겟이 없으면 종료
	{
		return false;
	}

	return bResult && (Target->GetDistanceTo(ControllingPawn) <= 500.0f);
}
