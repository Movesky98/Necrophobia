// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss_BTTask_Appear.h"
#include "Pro4BossAI.h"
#include "Pro4Boss.h"
#include "BehaviorTree/BlackboardComponent.h"

UBoss_BTTask_Appear::UBoss_BTTask_Appear()
{
	bNotifyTick = true;
}

// 블랙보드에서 실행하는 태스크
EBTNodeResult::Type UBoss_BTTask_Appear::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	// AI가 빙의한 보스 가져오기
	auto CurrentZombie = Cast<APro4Boss>(OwnerComp.GetAIOwner()->GetPawn());
	if (CurrentZombie == nullptr)
		return EBTNodeResult::Failed;

	// 공격함수 실행
	CurrentZombie->Appear();

	return EBTNodeResult::InProgress;
}