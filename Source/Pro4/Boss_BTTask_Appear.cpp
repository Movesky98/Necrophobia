// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss_BTTask_Appear.h"
#include "Pro4BossAI.h"
#include "Pro4Boss.h"
#include "BehaviorTree/BlackboardComponent.h"

UBoss_BTTask_Appear::UBoss_BTTask_Appear()
{
	bNotifyTick = true;
}

// �����忡�� �����ϴ� �½�ũ
EBTNodeResult::Type UBoss_BTTask_Appear::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	// AI�� ������ ���� ��������
	auto CurrentZombie = Cast<APro4Boss>(OwnerComp.GetAIOwner()->GetPawn());
	if (CurrentZombie == nullptr)
		return EBTNodeResult::Failed;

	// �����Լ� ����
	CurrentZombie->Appear();

	return EBTNodeResult::InProgress;
}