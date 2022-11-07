// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_BossAttackRange.h"
#include "Pro4Character.h"
#include "Pro4BossAI.h"
#include "Pro4Boss.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTDecorator_BossAttackRange::UBTDecorator_BossAttackRange()
{
	// �����忡�� ����� �̸�
	NodeName = TEXT("CanAttack");
}

bool UBTDecorator_BossAttackRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	// AI�� ���ǵ� ���� ĳ����
	auto ControllingPawn = Cast<APro4Boss>(OwnerComp.GetAIOwner()->GetPawn());
	if (ControllingPawn == nullptr)
	{
		return false;
	}

	// Ÿ������ ������ �÷��̾� ĳ����
	auto Target = Cast<APro4Character>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName(TEXT("Target"))));
	if (Target == nullptr) // ������ Ÿ���� ������ ����
	{
		return false;
	}

	return bResult && (Target->GetDistanceTo(ControllingPawn) <= 500.0f);
}
