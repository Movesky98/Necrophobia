// Fill out your copyright notice in the Description page of Project Settings.

#include "Boss_BTTask_Attack.h"
#include "Pro4Character.h"
#include "Pro4BossAI.h"
#include "Pro4Boss.h"
#include "BehaviorTree/BlackboardComponent.h"

UBoss_BTTask_Attack::UBoss_BTTask_Attack()
{
	bNotifyTick = true;
	IsAttacking = false;
}

// �����忡�� �����ϴ� �½�ũ
EBTNodeResult::Type UBoss_BTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	// AI�� ������ ���� ��������
	auto CurrentZombie = Cast<APro4Boss>(OwnerComp.GetAIOwner()->GetPawn());
	if (CurrentZombie == nullptr)
		return EBTNodeResult::Failed;

	// ������ ������ Ÿ�� ĳ����
	auto Target = Cast<APro4Character>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName(TEXT("Target"))));
	if (Target == nullptr)
		return EBTNodeResult::Failed;

	// �����Լ� ����
	CurrentZombie->Attack();
	IsAttacking = true;

	// �������� ĳ���� ������ ����ɽ� �ݵǴ� �Լ�
	CurrentZombie->OnAttackEnd.AddLambda([this]() -> void
		{
			IsAttacking = false;
		});

	return EBTNodeResult::InProgress;
}

void UBoss_BTTask_Attack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	if (!IsAttacking)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
