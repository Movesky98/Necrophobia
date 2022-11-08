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

// 블랙보드에서 실행하는 태스크
EBTNodeResult::Type UBoss_BTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	// AI가 빙의한 보스 가져오기
	auto CurrentZombie = Cast<APro4Boss>(OwnerComp.GetAIOwner()->GetPawn());
	if (CurrentZombie == nullptr)
		return EBTNodeResult::Failed;

	// 보스가 공격할 타겟 캐릭터
	auto Target = Cast<APro4Character>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName(TEXT("Target"))));
	if (Target == nullptr)
		return EBTNodeResult::Failed;

	// 공격함수 실행
	CurrentZombie->Attack();
	IsAttacking = true;

	// 보스에서 캐릭터 공격이 종료될시 콜되는 함수
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
